/*******************************************************************

MinimOSD-ng
Copyright (C) 2014  Luis Alves

This file is part of MinimOSD-ng.

MinimOSD-ng is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

MinimOSD-ng is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MinimOSD-ng.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************/

#include "config.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>
#include "widgets.h"
#include "max7456.h"
#include "timer.h"
#include "mavlink.h"

#define DEBUG_WIDGET_TIMMNIG 1

/* import widgets */
WIDGET_IMPORT(pitch_widget);
WIDGET_IMPORT(roll_widget);
WIDGET_IMPORT(rssi_widget);
WIDGET_IMPORT(altitude_widget);
WIDGET_IMPORT(clock_widget);
WIDGET_IMPORT(gpscoords_widget);
WIDGET_IMPORT(gpsstats_widget);
WIDGET_IMPORT(rcchannels_widget);
WIDGET_IMPORT(batvoltage_widget);
WIDGET_IMPORT(batcurrent_widget);
WIDGET_IMPORT(batremain_widget);
WIDGET_IMPORT(startup_widget);
WIDGET_IMPORT(cog_widget);
WIDGET_IMPORT(windrose_widget);
WIDGET_IMPORT(heading_widget);
WIDGET_IMPORT(homedistance_widget);
WIDGET_IMPORT(homedirection_widget);
WIDGET_IMPORT(horizon_widget);
WIDGET_IMPORT(flightstats_widget);
WIDGET_IMPORT(radar_widget);

WIDGETS( \
  &pitch_widget,
  &roll_widget,
  &gpsstats_widget,
  &gpscoords_widget,
  &altitude_widget,
  &batvoltage_widget,
  &batcurrent_widget,
  &batremain_widget,
  &clock_widget,
  &rcchannels_widget,
  &rssi_widget,
  &startup_widget,
  &cog_widget,
  &windrose_widget,
  &heading_widget,
  &homedistance_widget,
  &homedirection_widget,
  &horizon_widget,
  &flightstats_widget,
  &radar_widget,
  NULL,
);

/* configuration burned to the eeprom */
unsigned char widget_default_config[] EEMEM = {
  STARTUP_TAB, STARTUP_WIDGET_ID, 3, 4,

  /* tab, widget_id, x, y */
  1, ROLL_WIDGET_ID,           0,  0,
  1, PITCH_WIDGET_ID,          0,  1,
  1, RSSI_WIDGET_ID,           0,  2,
  1, ALTITUDE_WIDGET_ID,       0,  3,
  1, CLOCK_WIDGET_ID,          0,  4,
  1, GPSCOORDS_WIDGET_ID,      0,  5,
  1, GPSSTATUS_WIDGET_ID,      0,  7,
  2, RCCHANNELS_WIDGET_ID,    12,  0,
  1, BATVOLTAGE_WIDGET_ID,     0,  8,
  1, BATCURRENT_WIDGET_ID,     0,  9,
  1, BATREMAIN_WIDGET_ID,      0, 10,
  1, COG_WIDGET_ID,            0, 11,
  1, WINDROSE_WIDGET_ID,       0, 12,
  1, HEADING_WIDGET_ID,        0, 13,
  1, HOMEDISTANCE_WIDGET_ID,   8,  0,
  1, HOMEDIRECTION_WIDGET_ID,  8,  1,
  1, HORIZON_WIDGET_ID,        8,  7,
  3, FLIGHTSTATS_WIDGET_ID,    0,  0,
  4, RADAR_WIDGET_ID,          0,  0,
  4, HOMEDISTANCE_WIDGET_ID,   0, 14,
  4, HOMEDIRECTION_WIDGET_ID,  0, 15,
  TAB_TABLE_END,
  };


extern struct minimosd_ng_config config;
extern struct mavlink_data mavdata;

static unsigned char current_tidx = TAB_TABLE_END;
static struct widget *rlist[WIDGETS_NUM];
static struct widget **rwid;
static unsigned char *tab_list;

static void load_widgets_tab(unsigned char tidx);

static unsigned char search_on_list(unsigned char *list, unsigned char tab)
{
  unsigned char *p = &list[0];
  char size = *p++;
  while (size-- > 0) {
    if (*p++ == tab)
      return 1;
  }
  return 0;
}

/* fill a list with the currently configured tab numbers */
/* returns a pointer to the list */
static unsigned char* init_tab_list(void)
{
  struct widget_config *addr = 0x00, cfg;
  unsigned char tlist[CONF_MAX_TABS+1];
  unsigned char *cnt = &tlist[0];
  unsigned char *p = &tlist[1];

  /* first element of the list is the length */
  *cnt = 0;

  while ((unsigned int) addr < (0x400 - 0x10)) {
    eeprom_read_block(&cfg, (const void*) addr++, sizeof(struct widget_config));
    if (cfg.tab == TAB_TABLE_END)
      break;
    if (!search_on_list(tlist, cfg.tab)) {
      (*cnt)++;
      *p++ = cfg.tab;
      if (*cnt == CONF_MAX_TABS)
        break;
    }
  };

  tab_list = malloc((*cnt) + 1);
  if (tab_list != NULL)
    memcpy(tab_list, tlist, (*cnt) + 1);
  
  return tab_list;  
}


void init_widgets(void)
{
  /* vsync trigger int on falling edge */
  EICRA |=  _BV(ISC01);
  EICRA &= ~_BV(ISC00);

  /* init widget rendering indexer */
  rlist[0] = NULL;
  rwid = rlist;

  /* init list of configured tabs */
  tab_list = init_tab_list();

  /* load initial tab */
  load_widgets_tab(STARTUP_TAB);
}

static void find_config(unsigned char tab, unsigned char id, struct widget_config *cfg)
{
  struct widget_config *addr = 0x00;
  while ((unsigned int) addr < (0x400 - 0x10)) {
    eeprom_read_block(cfg, (const void*) addr++, sizeof(struct widget_config));
    if (((cfg->id == id) && (cfg->tab == tab)) || (cfg->tab == TAB_TABLE_END))
      break;
  };
}

static void load_widgets_tab(unsigned char tidx)
{
  struct widget_config cfg;
  struct widget_state s;
  struct widget **w, **r;
  unsigned char tab = tab_list[tidx+1];

  /* disable refresh interrupt */
  EIMSK &= ~_BV(INT0);
  max7456_clr();

  w = all_widgets;
  r = rlist;
  do {
    find_config(tab, (*w)->id, &cfg);
    if (cfg.tab != 0xff) {
      s.props = WIDGET_ENABLED | WIDGET_INIT;
      s.x = cfg.x;
      s.y = cfg.y;
      *r++ = *w;
      (*w)->do_state(&s);
    }
  } while ((*++w) != NULL);
  *r = NULL;

  rwid = rlist;
  current_tidx = tidx;
  /* re-enable interrupt if we have at least 1 widget to render */
  if ((*rwid) != NULL)
    EIMSK |= _BV(INT0);
}


#define CH_DELTA (CONF_TABSWITCH_CH_MAX - CONF_TABSWITCH_CH_MIN)


void widgets_process(void)
{
  struct tab_switch *ts = &config.tab_sw;
  unsigned char nr_tabs = tab_list[0] - 1;
  int raw = (int) mavdata.ch_raw[ts->ch];
  unsigned char new_tidx = current_tidx;
  static unsigned char prev_v;
  static unsigned int timer;
  int v;

  switch (ts->method) {
  case TAB_SWITCH_PERCENT:
  /* percent method will split evenly a 
     channel in the number of tabs */
    v = ((raw - CONF_TABSWITCH_CH_MIN) / (CH_DELTA / (int) nr_tabs));
    /* trim value */
    if (v < 0)
      v = 0;
    if (v >= nr_tabs)
      v = nr_tabs - 1;

    new_tidx = (unsigned char) v + 1;
    break;
  case TAB_SWITCH_TOGGLE:
  /* toggle method will cycle tabs when a
     channel switches from min to max */
    v = ((raw - CONF_TABSWITCH_CH_MIN) << 1) / CH_DELTA;
    if (v < 0)
      v = 0;
    if (v > 1)
      v = 1;
    if (v != prev_v) {
      /* cycle */
      new_tidx = current_tidx + 1;
      if (new_tidx > nr_tabs)
        new_tidx = 1;
      prev_v = (unsigned char) v;
    }
    break;
  case TAB_SWITCH_PUSH:
  /* push method will cycle tabs when a
     channel switches from min to max and then returns to min */
    v = ((raw - CONF_TABSWITCH_CH_MIN) << 1) / CH_DELTA;
    if (v < 0)
      v = 0;
    if (v > 1)
      v = 1;

    if ((prev_v == 1) && (v == 0)) {
      prev_v = 0;
    } else if ((v == 1) && (prev_v == 0)) {
      /* cycle */
      new_tidx = current_tidx + 1;
      if (new_tidx > nr_tabs)
        new_tidx = 1;
      prev_v = 1;
    }
    break;
  case TAB_SWITCH_DEMO:
  default:
  /* default tab switch is demo mode */
  /* cycles through all available tabs with a 5 sec delay */
    if ((now() - timer) > 5000) {
      timer += 5000;
      new_tidx = current_tidx + 1;
      if (new_tidx > nr_tabs)
        new_tidx = 1;
    }
    break;
  }

  /* load if tab changed */
  if (current_tidx != new_tidx)
    load_widgets_tab(new_tidx);
}


/*
 VSYNC interrupt is used to render widgets.
 The pulse gives us aprox. 1.3 ms for rendering
 widgets without any flickering artifacts.
*/
ISR(INT0_vect)
{
  struct widget *start_rwid, *prev;
  unsigned char rendered = 0;
  unsigned int t = nnow();
  unsigned int dt = 0;

  start_rwid = *rwid;

  /* allow ~1ms to draw widgets */
  while ((dt = nnow() - t) < (700 / 125)) {
#if DEBUG_WIDGET_TIMMNIG
    prev = *rwid;
#endif
    if ((*rwid)->draw())
      rendered += 1;
    else
      continue;

    rwid++;
    /* wrap around */
    if ((*rwid) == NULL)
      rwid = rlist;

    /* check if all done */
    if ((*rwid) == start_rwid)
      break;
  }
#if DEBUG_WIDGET_TIMMNIG
  if (dt > (1400/125))
    printf("(%d) id=%d %uus\n", (int) rendered, (int) prev->id, dt * 125);
#endif

}


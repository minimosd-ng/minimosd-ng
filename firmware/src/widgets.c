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
#include "widgets.h"
#include "max7456.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "timer.h"

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


unsigned char widget_default_config[] EEMEM = {
  /* tab, widget_id, x, y */
  0, STARTUP_WIDGET_ID,        3,  4,

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
  0xff
  };


static struct widget *rlist[WIDGETS_NUM];
static struct widget **rwid;

void init_widgets(void)
{
  /* init widget rendering indexer */
  rlist[0] = NULL;
  rwid = rlist;

  /* vsync trigger int on falling edge */
  EICRA |=  _BV(ISC01);
  EICRA &= ~_BV(ISC00);
}

static void find_config(unsigned char tab, unsigned char id, struct widget_config *cfg)
{
  struct widget_config *addr = 0x00;
  while ((unsigned int) addr < (0x400 - 0x10)) {
    eeprom_read_block(cfg, (const void*) addr++, sizeof(struct widget_config));
    if ((cfg->id == id) && (cfg->tab == tab))
      break;
    if (cfg->tab == 0xff)
      break;
  };
}

static unsigned char current_tab = 0xff;

void load_widgets_tab(unsigned char tab)
{
  struct widget_config cfg;
  struct widget_state s;
  struct widget **w, **r;

  if (current_tab == tab)
    return;
  current_tab = tab;

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
  /* re-enable interrupt if we have at least 1 widget to render */
  if ((*rwid) != NULL)
    EIMSK |= _BV(INT0);
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

  /* allow 1ms to draw widgets */
  while ((dt = nnow() - t) < (750 / 125)) {
    if ((*rwid)->draw())
      rendered += 1;
    else
      continue;

#if DEBUG_WIDGET_TIMMNIG
    prev = *rwid;
#endif

    rwid++;
    /* wrap around */
    if ((*rwid) == NULL)
      rwid = rlist;

    /* check if all done */
    if ((*rwid) == start_rwid)
      break;
  }
#if DEBUG_WIDGET_TIMMNIG
  if (dt > (1300/125))
    printf("(%d) id=%d %uus\n", (int) prev->id, (int) rendered, dt * 125);
#endif

}


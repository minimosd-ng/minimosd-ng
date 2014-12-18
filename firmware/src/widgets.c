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
  1, HORIZON_WIDGET_ID         8, 11,
  0xff
  };


static unsigned char widx;

void init_widgets(void)
{
  /* init widget indexer */
  widx = 0;

  /* vsync trigger int on falling edge */
  EICRA |=  _BV(ISC01);
  EICRA &= ~_BV(ISC00);

  /* enable osd refresh */
  EIMSK |= _BV(INT0);

  widx = 0;
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
  unsigned char i;
  struct widget_config cfg;
  struct widget_state s;

  if (current_tab == tab)
    return;
  current_tab = tab;

  /* disable refresh interrupt */
  EIMSK &= ~_BV(INT0);
  max7456_clr();


  for (i = 0; i < WIDGETS_NUM; i++) {
    find_config(tab, all_widgets[i]->id, &cfg);
    if (cfg.tab == 0xff) {
      s.props = WIDGET_DISABLED;
    } else {
      s.props = WIDGET_ENABLED;
      s.x = cfg.x;
      s.y = cfg.y;
    }
    all_widgets[i]->set_state(&s);
  }

  widx = 0;
  /* re-enable interrupt */
  EIMSK |= _BV(INT0);
}

/* VSYNC interrupt used to render widgets */
ISR(INT0_vect)
{
  const struct widget *w;
  struct widget_state *s;
  unsigned char rendered = 0;

  /* render at least 5 widgets per call */
  while (rendered < 5) {
    w = all_widgets[widx++];
    s = w->get_state();
    if (s->props & WIDGET_ENABLED) {
      w->draw();
      rendered++;      
    }

    if (widx >= WIDGETS_NUM) {
      widx = 0;
      break;
    }
  }
}


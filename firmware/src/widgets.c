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
//WIDGET_IMPORT(_widget);

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
);


unsigned char widget_default_config[] EEMEM = {
  /* tab, widget_id, x, y */
  0, ROLL_WIDGET_ID, 0, 0,
  0, PITCH_WIDGET_ID, 0, 1,

  0xff
  };


static unsigned char widx, tab = 0;

void init_widgets(void)
{
  /* init widget indexer */
  widx = 0;

  /* vsync trigger int on falling edge */
  EICRA |=  _BV(ISC01);
  EICRA &= ~_BV(ISC00);

  /* enable osd refresh */
  EIMSK |= _BV(INT0);
}

const struct widget *get_widget(unsigned char id)
{
  unsigned char i;
  for (i = 0; i < WIDGETS_NUM; i++)
    if (all_widgets[i]->id == id)
      return all_widgets[i];
  return NULL;
}


void find_config(unsigned char tab, unsigned char id, struct widget_config *cfg)
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


void configure_widgets(void)
{
  unsigned char i;
  struct widget_config cfg;
  struct widget_state s;

  for (i = 0; i < WIDGETS_NUM; i++) {
    find_config(tab, all_widgets[i]->id, &cfg);
    printf("got cfg: tab=%d id=%d x=%d y=%d\n", cfg.tab, cfg.id, cfg.x, cfg.y);
    if (cfg.tab == 0xff) {
      s.props = WIDGET_DISABLED;
    } else {
      s.props = WIDGET_ENABLED;
      s.x = cfg.x;
      s.y = cfg.y;
    }
    all_widgets[i]->set_state(&s);
  }
}

/* VSYNC interrupt used to render widgets */
ISR(INT0_vect)
{
  const struct widget *w = all_widgets[widx++];
  struct widget_state *s = w->get_state();

  if (widx >= WIDGETS_NUM)
    widx = 0;

  if (s->props & WIDGET_ENABLED)
    w->draw();

}


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

/* widgets */
extern struct widget \
  pitch_widget,
  roll_widget,
  gpsstats_widget,
  gpscoords_widget,
  altitude_widget,
  batteryvolt_widget,
  batterycurrent_widget,
  batteryremain_widget;

WIDGETS( \
  &pitch_widget,
  &roll_widget,
  &gpsstats_widget,
  &gpscoords_widget,
  &altitude_widget,
  &batteryvolt_widget,
  &batterycurrent_widget,
  &batteryremain_widget );


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
}

void configure_widgets(void)
{
  unsigned char i;

  for (i = 0; i < WIDGETS_NUM-1; i++)
    all_widgets[i]->configure(0, 0);
}

/* VSYNC interrupt used to render widgets */
ISR(INT0_vect)
{
  all_widgets[widx++]->draw();
  if (widx >= WIDGETS_NUM-1)
    widx = 0;
}


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
#include <stdio.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

struct {
  unsigned char x, y;
  unsigned char props;
} bv, bi, br;

extern struct mavlink_data mavdata;

/* configure widget based on eeprom data */
static void configure_v(unsigned int addr, unsigned char len)
{
  bv.x = 20;
  bv.y = 0;
  bv.props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void configure_i(unsigned int addr, unsigned char len)
{
  bi.x = 20;
  bi.y = 1;
  bi.props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void configure_r(unsigned int addr, unsigned char len)
{
  br.x = 20;
  br.y = 2;
  br.props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void draw_v(void)
{
  char buf[15];
  sprintf(buf, "%5.2f%c", mavdata.bat_voltage, 0x0d);
  max7456_xy(bv.x, bv.y);
  max7456_puts(buf);
}

static void draw_i(void)
{
  char buf[15];
  sprintf(buf, "%5.2f%c", mavdata.bat_current / 10.0, 0x0e);
  max7456_xy(bi.x, bi.y);
  max7456_puts(buf);
}

static void draw_r(void)
{
  char buf[15];
  sprintf(buf, "%3d%c", mavdata.bat_remaining, 0x25);
  max7456_xy(br.x, br.y);
  max7456_puts(buf);
}

WIDGETS_WIDGET(batteryvolt_widget, "Battery voltage", configure_v, draw_v);
WIDGETS_WIDGET(batterycurrent_widget, "Battery current", configure_i, draw_i);
WIDGETS_WIDGET(batteryremain_widget, "Battery remaining", configure_r, draw_r);


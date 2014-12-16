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


static unsigned char x, y, props;
static int v;


/* configure widget based on eeprom data */
static void configure(unsigned int addr, unsigned char len)
{
  x = 13;
  y = 1;
  props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void set_mavdata(mavlink_message_t *msg)
{
  if (msg->msgid != MAVLINK_MSG_ID_GPS_RAW_INT)
    return;
  v = (unsigned int) ((long) mavlink_msg_gps_raw_int_get_alt(&msg) / 1000);
  PRINTF("absolute altitude widget: value=%d\n", v);
}

static void draw(void)
{
  char buf[10];
  sprintf(buf, "%5d%c", v, 0x0c);
  max7456_xy(x, y);
  max7456_puts(buf);
}


WIDGETS_WIDGET(altitude_widget, "Altitude", configure, set_mavdata, draw);


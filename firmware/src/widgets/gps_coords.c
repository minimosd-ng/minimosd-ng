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

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static unsigned char x, y, props;
static double lat, lon;


/* configure widget based on eeprom data */
static void configure(unsigned int addr, unsigned char len)
{
  x = 0;
  y = 1;
  props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void set_mavdata(mavlink_message_t *msg)
{
  if (msg->msgid != MAVLINK_MSG_ID_GPS_RAW_INT)
    return;
  lat = (double) mavlink_msg_gps_raw_int_get_lat(msg) / 10000000.0;
  lon = (double) mavlink_msg_gps_raw_int_get_lon(msg) / 10000000.0;

  PRINTF("gps widget: lat=%11.6f log=%11.6f\n", lat, lon);
}

static void draw(void)
{
  char buf[15];
  max7456_xy(x, y);
  sprintf(buf, "%11.6f", lat);
  max7456_puts(buf);
  
  max7456_xy(x, y+1);
  sprintf(buf, "%11.6f", lon);
  max7456_puts(buf);
}


WIDGETS_WIDGET(gpscoords_widget, "Gps coords", configure, set_mavdata, draw);


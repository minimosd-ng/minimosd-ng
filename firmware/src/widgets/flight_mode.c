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

 Radar by Luis Alves
 Initial version shows home
 Todo: Add waypoints

*********************************************************************/

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

static struct widget_state state;

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

static char draw(void)
{
  unsigned char custom_mode = mavdata.custom_mode;
  unsigned char armed = mavdata.base_mode & MAV_MODE_FLAG_SAFETY_ARMED;
  char buf[6];
  char *b = &buf[1];

  if (config.vehicle == APM_COPTER)
    custom_mode += 100;

  switch (custom_mode) {
  case PLANE_MODE_MANUAL:
    memcpy_P(b, PSTR("manu"), 4);
    break;
  case PLANE_MODE_CIRCLE:
  case COPTER_MODE_CIRCLE:
    memcpy_P(b, PSTR("circ"), 4);
    break;
  case PLANE_MODE_STABILIZE:
  case COPTER_MODE_STABILIZE:
    memcpy_P(b, PSTR("stab"), 4);
    break;
  case PLANE_MODE_TRAINING:
    memcpy_P(b, PSTR("trai"), 4);
    break;
  case PLANE_MODE_ACRO:
  case COPTER_MODE_ACRO:
    memcpy_P(b, PSTR("acro"), 4);
    break;
  case PLANE_MODE_FBWA:
    memcpy_P(b, PSTR("fbwa"), 4);
    break;
  case PLANE_MODE_FBWB:
    memcpy_P(b, PSTR("fbwb"), 4);
    break;
  case PLANE_MODE_CRUISE:
    memcpy_P(b, PSTR("crui"), 4);
    break;
  case PLANE_MODE_AUTOTUNE:
    memcpy_P(b, PSTR("tune"), 4);
    break;
  case PLANE_MODE_AUTO:
  case COPTER_MODE_AUTO:
    memcpy_P(b, PSTR("auto"), 4);
    break;
  case PLANE_MODE_RTL:
  case COPTER_MODE_RTL:
    memcpy_P(b, PSTR("retl"), 4);
    break;
  case PLANE_MODE_LOITER:
  case COPTER_MODE_LOITER:
    memcpy_P(b, PSTR("loit"), 4);
    break;
  case PLANE_MODE_GUIDED:
  case COPTER_MODE_GUIDED:
    memcpy_P(b, PSTR("guid"), 4);
    break;
  case COPTER_MODE_ALTHOLD:
    memcpy_P(b, PSTR("alth"), 4);
    break;
  case COPTER_MODE_LAND:
    memcpy_P(b, PSTR("land"), 4);
    break;
  case COPTER_MODE_DRIFT:
    memcpy_P(b, PSTR("drif"), 4);
    break;
  case COPTER_MODE_SPORT:
    memcpy_P(b, PSTR("spor"), 4);
    break;
  case COPTER_MODE_POSHOLD:
    memcpy_P(b, PSTR("phol"), 4);
    break;
  default:
    memcpy_P(b, PSTR("unkn"), 4);
    break;
  }

  buf[0] = MAX7456_FONT_MODE;
  buf[5] = armed ? MAX7456_FONT_MOTORARMED : 0x00;

  max7456_putsn(state.x, state.y, buf, 6);
  return 1;
}

WIDGET_DECLARE(flightmode_widget, FLIGHTMODE_WIDGET_ID, draw);


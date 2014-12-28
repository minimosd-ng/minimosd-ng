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
  unsigned char mode = mavdata.mode;
  char buf[10];

  if (config.vehicle == APM_COPTER)
    mode += 100;

  switch (mode) {
  case PLANE_MODE_MANUAL:
    memcpy_P(buf, PSTR("manu"), 4);
    break;
  case PLANE_MODE_CIRCLE:
  case COPTER_MODE_CIRCLE:
    memcpy_P(buf, PSTR("circ"), 4);
    break;
  case PLANE_MODE_STABILIZE:
  case COPTER_MODE_STABILIZE:
    memcpy_P(buf, PSTR("stab"), 4);
    break;
  case PLANE_MODE_TRAINING:
    memcpy_P(buf, PSTR("trai"), 4);
    break;
  case PLANE_MODE_ACRO:
  case COPTER_MODE_ACRO:
    memcpy_P(buf, PSTR("acro"), 4);
    break;
  case PLANE_MODE_FBWA:
    memcpy_P(buf, PSTR("fbwa"), 4);
    break;
  case PLANE_MODE_FBWB:
    memcpy_P(buf, PSTR("fbwb"), 4);
    break;
  case PLANE_MODE_CRUISE:
    memcpy_P(buf, PSTR("crui"), 4);
    break;
  case PLANE_MODE_AUTOTUNE:
    memcpy_P(buf, PSTR("tune"), 4);
    break;
  case PLANE_MODE_AUTO:
  case COPTER_MODE_AUTO:
    memcpy_P(buf, PSTR("auto"), 4);
    break;
  case PLANE_MODE_RTL:
  case COPTER_MODE_RTL:
    memcpy_P(buf, PSTR("retl"), 4);
    break;
  case PLANE_MODE_LOITER:
  case COPTER_MODE_LOITER:
    memcpy_P(buf, PSTR("loit"), 4);
    break;
  case PLANE_MODE_GUIDED:
  case COPTER_MODE_GUIDED:
    memcpy_P(buf, PSTR("guid"), 4);
    break;
  case COPTER_MODE_ALTHOLD:
    memcpy_P(buf, PSTR("alth"), 4);
    break;
  case COPTER_MODE_LAND:
    memcpy_P(buf, PSTR("land"), 4);
    break;
  case COPTER_MODE_DRIFT:
    memcpy_P(buf, PSTR("drif"), 4);
    break;
  case COPTER_MODE_SPORT:
    memcpy_P(buf, PSTR("spor"), 4);
    break;
  case COPTER_MODE_POSHOLD:
    memcpy_P(buf, PSTR("phol"), 4);
    break;
  default:
    memcpy_P(buf, PSTR("unkn"), 4);
    break;
  }

  max7456_putsn(state.x, state.y, buf, 4);
  return 1;
}

WIDGET_DECLARE(flightmode_widget, FLIGHTMODE_WIDGET_ID, draw);


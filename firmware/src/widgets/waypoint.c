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
#include <string.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static struct widget_state state;

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

static char draw(void)
{
  char idx = (char) ((int) ((mavdata.nav_bearing - mavdata.vfr_hud.heading) * 16) / 360);
  float distance = mavdata.nav_wp_distance;
  char buf[10];
  unsigned char mode = mavdata.mode;

  if (state.props & WIDGET_INIT) {
    max7456_printf(state.x, state.y, "WP");
    if (config.units & LENGTH_UNITS_IMPERIAL)
      max7456_putc(state.x+4, state.y+1, 'f');
    else
      max7456_putc(state.x+4, state.y+1, 'm');

    state.props &= ~WIDGET_INIT;
  }

  idx = (idx & 0xf) << 1;

  if (config.units & LENGTH_UNITS_IMPERIAL)
    distance = (unsigned int) ((float) distance * M2FEET);

  max7456_printf(state.x+2, state.y, "%3d%c%c", mavdata.wp_sequence,
        MAX7456_FONT_DIR_ARROWS + idx, MAX7456_FONT_DIR_ARROWS + idx + 1);
  max7456_printf(state.x, state.y+1, "%4d", distance);

  if (config.vehicle == APM_COPTER)
    mode += 100;

  switch (mode) {
  case (PLANE_MODE_AUTO):
  case (PLANE_MODE_GUIDED):
  case (PLANE_MODE_CRUISE):
  case (COPTER_MODE_AUTO):
    distance = mavdata.nav_xtrack_error;
    if (config.units & LENGTH_UNITS_IMPERIAL)
      distance = (unsigned int) ((float) distance * M2FEET);
    max7456_printf(state.x, state.y+2, "Xe%4d%c",
                  distance);
  default:
    memset(buf, ' ', 7);
    buf[7] = 0x00;
    max7456_putsn(state.x, state.y+2, buf, 7);
  }

  return 1;
}

WIDGET_DECLARE(waypoint_widget, WAYPOINT_WIDGET_ID, draw);


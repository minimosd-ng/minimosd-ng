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
#include "mavlink.h"

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

static struct widget_state state;

static char draw(void)
{
  int speed = mavdata.wind_speed * 3600;
  char idx = ((int) ((mavdata.wind_direction - mavdata.vfr_hud.heading) * 16) / 360) + 8;
  idx = (idx & 0xf) << 1;

  if (state.props & WIDGET_INIT) {
    max7456_putc(state.x, state.y, MAX7456_FONT_WINDSPEED);
    if (config.units & LENGTH_UNITS_IMPERIAL)
      max7456_putc(state.x+6, state.y, MAX7456_FONT_MILESHOUR);
    else
      max7456_putc(state.x+6, state.y, MAX7456_FONT_KMH);
    state.props &= ~WIDGET_INIT;
  }

  /* wind direction arrow */
  max7456_printf(state.x+1, state.y, "%c%c",
        MAX7456_FONT_DIR_ARROWS + idx, MAX7456_FONT_DIR_ARROWS + idx + 1);

  if (config.units & LENGTH_UNITS_IMPERIAL)
    speed *= M2MILE;    /* miles per hour */
  else
    speed /= 1000;      /* km per hour */

  max7456_printf(state.x+3, state.y, "%3d", (int) speed);

  return 1;
}

WIDGET_DECLARE(windspeed_widget, WINDSPEED_WIDGET_ID, draw);


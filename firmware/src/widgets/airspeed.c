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

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

static struct widget_state state;

static char draw(void)
{
  float v = (mavdata.vfr_hud.airspeed * 3600);
  if (state.props & WIDGET_INIT) {
    max7456_putc(state.x, state.y, MAX7456_FONT_AIRSPEED);
    if (config.units & LENGTH_UNITS_IMPERIAL)
      max7456_putc(state.x+4, state.y, MAX7456_FONT_MILESHOUR);
    else
      max7456_putc(state.x+4, state.y, MAX7456_FONT_KMH);
    state.props &= ~WIDGET_INIT;
  }
  if (config.units & LENGTH_UNITS_IMPERIAL)
    v *= M2MILE;
  else
    v /= 1000.0;

  max7456_printf(state.x+1, state.y, "%3d", (int) v);
  return 1;
}

WIDGET_DECLARE(airspeed_widget, AIRSPEED_WIDGET_ID, draw);


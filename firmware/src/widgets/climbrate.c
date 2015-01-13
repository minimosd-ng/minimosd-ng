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

static struct widget_state state;

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

#if CONF_CLIMBRATE_EMA > 0
#define ALPHA (1.0/CONF_CLIMBRATE_EMA)
static int avg;
#endif

static char draw(void)
{
  int climb;
  unsigned char u = MAX7456_FONT_METERMIN;
  if (state.props & WIDGET_INIT) {
    if (config.units & LENGTH_UNITS_IMPERIAL)
      u = MAX7456_FONT_FEETMIN;
    max7456_putc(state.x+5, state.y, u);
    max7456_putc(state.x, state.y, MAX7456_FONT_CLIMBRATE);
    state.props &= ~WIDGET_INIT;
  }

  if (config.units & LENGTH_UNITS_IMPERIAL)
    climb = (int) ((float) mavdata.vfr_hud.climb * M2FEET * 60.0);
  else
    climb = (int) ((float) mavdata.vfr_hud.climb * 60.0);

#if CONF_CLIMBRATE_EMA > 0
  avg = avg - (int) (((float) avg - (float) climb) * ALPHA);
  climb = avg;
#endif

  max7456_printf(state.x+1, state.y, "%4d", climb);
  return 1;
}

WIDGET_DECLARE(climbrate_widget, CLIMBRATE_WIDGET_ID,  draw);


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

static char draw(void)
{
  float distance = (float) mavdata.calcs.home_distance;

  if (state.props & WIDGET_INIT) {
    max7456_putc(state.x, state.y, MAX7456_FONT_HOME);
    if (config.units & LENGTH_UNITS_IMPERIAL)
      max7456_putc(state.x+6, state.y, 'f');
    else
      max7456_putc(state.x+6, state.y, 'm');
    state.props &= ~WIDGET_INIT;
  }

  if (config.units & LENGTH_UNITS_IMPERIAL)
    distance *= M2FEET;

  if (distance > 99999)
    distance = 99999;

  max7456_printf(state.x+1, state.y, "%5lu", (unsigned long) distance);
  return 1;
}

WIDGET_DECLARE(homedistance_widget, HOMEDISTANCE_WIDGET_ID, draw);


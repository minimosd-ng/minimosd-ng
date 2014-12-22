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

extern struct mavlink_data mavdata;

static struct widget_state state;

static char draw(void)
{
  if (state.props & WIDGET_INIT) {
    max7456_putc(state.x+5, state.y, 0x0c);
    state.props &= ~WIDGET_INIT;
  }
  max7456_printf(state.x, state.y, "%5d", (long) mavdata.gps_altitude);
  return 1;
}

WIDGET_DECLARE(altitude_widget, ALTITUDE_WIDGET_ID, draw);


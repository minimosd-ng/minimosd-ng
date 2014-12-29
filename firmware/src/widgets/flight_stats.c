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
#include "timer.h"

static struct widget_state state;

extern struct mavlink_data mavdata;

static char draw(void)
{
  char buf[20];
  struct flight_stats *s = &mavdata.stats;
  struct time t;

  if (state.props & WIDGET_INIT) {
    max7456_printf(state.x, state.y, P("Flight stats"));
    max7456_printf(state.x+1, state.y+2, P("Total flight time:"));
    max7456_printf(state.x+1, state.y+3, P("Total distance:"));
    max7456_printf(state.x+1, state.y+4, P("Used battery:"));
    max7456_printf(state.x+1, state.y+5, P("Max home distance:"));
    max7456_printf(state.x+1, state.y+6, P("Max home altitude:"));
    max7456_printf(state.x+1, state.y+7, P("Max airspeed:"));
    max7456_printf(state.x+1, state.y+8, P("Max groundspeed:"));

    state.props &= ~WIDGET_INIT;
  }

  get_time(s->flight_end - s->flight_start, &t);

  max7456_printf(state.x+20, state.y+2, "%d:%02d:%02d", t.h, t.m, t.s);
  max7456_printf(state.x+22, state.y+3, "%5d", s->total_distance);
  max7456_printf(state.x+23, state.y+4, "%4d", s->used_battery);
  max7456_printf(state.x+22, state.y+5, "%5d", s->max_home_distance);
  max7456_printf(state.x+23, state.y+6, "%4d", s->max_home_altitude);
  max7456_printf(state.x+23, state.y+7, "%4d", s->max_airspeed);
  max7456_printf(state.x+23, state.y+8, "%4d", s->max_groundspeed);


  return 1;
}

WIDGET_DECLARE(flightstats_widget, FLIGHTSTATS_WIDGET_ID, draw);


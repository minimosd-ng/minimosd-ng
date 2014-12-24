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

static char draw(void)
{
  char buf[2] = {MAX7456_FONT_DEGREE, MAX7456_FONT_PITCH};
  if (state.props & WIDGET_INIT) {
    max7456_putsn(state.x+4, state.y, buf, 2);
    state.props &= ~WIDGET_INIT;
  }
  max7456_printf(state.x, state.y, "%4d", mavdata.pitch);
  return 1;
}

WIDGET_DECLARE(pitch_widget, PITCH_WIDGET_ID,  draw);


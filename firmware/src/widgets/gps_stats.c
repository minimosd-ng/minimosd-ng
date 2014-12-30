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
#include "timer.h"

static struct widget_state state;

extern struct mavlink_data mavdata;

static char draw(void)
{
  char s;

  switch (mavdata.gps_fix_type) {
  default:
  case 0:
  case 1:
    s = get_toogle() ? '*' : 0x20;
    break;
  case 2:
    s = 0x1f;
    break;
  case 3:
    s = 0x0f;
    break;
  }

  if (mavdata.gps_eph >= 200)
    s = get_toogle() ? s : 0x20;

  max7456_printf(state.x, state.y, "%c%2d", s, mavdata.gps_nrsats);
  return 1;
}

WIDGET_DECLARE(gpsstats_widget, GPSSTATUS_WIDGET_ID, draw);


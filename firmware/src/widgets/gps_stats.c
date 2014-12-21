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

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

WIDGET_STATE(0, 0, WIDGET_DISABLED);

extern struct mavlink_data mavdata;

static char draw(void)
{
  char buf[10];

  switch (mavdata.gps_fix_type) {
  case 0:
  case 1:
    buf[0] = get_toogle() ? 0x2a : 0x20;
    break;
  case 2:
    buf[0] = 0x1f;
    break;
  case 3:
    buf[0] = 0x0f;
    break;
  }

  if (mavdata.gps_eph >= 200)
    buf[0] = get_toogle() ? 0x2a : 0x20;

  sprintf(&buf[1], "%2d", mavdata.gps_nrsats);
  max7456_puts(state.x, state.y, buf);
  return 1;
}

WIDGET_DECLARE(gpsstats_widget, GPSSTATUS_WIDGET_ID, draw);


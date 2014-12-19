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

WIDGET_STATE(0, 0, WIDGET_DISABLED);

extern struct mavlink_data mavdata;

static void draw(void)
{
  const char full_rose[] = {
    0x82,0x80,0x81,0x80,
    0x84,0x80,0x81,0x80,
    0x83,0x80,0x81,0x80,
    0x85,0x80,0x81,0x80,
  };
  char buf[8];
  char idx = ((int) ((mavdata.vfr_hud.heading * 16) / 360) - 3) & 0x0f;
  unsigned char i;

  for (i = 1; i < 6; i++) {
    buf[i] = full_rose[(i+idx) & 0x0f];
  }
  buf[0] = 0xc3;
  buf[6] = 0x87;
  buf[7] = '\0';
  max7456_puts(state.x, state.y, buf);
}

WIDGET_DECLARE(windrose_widget, WINDROSE_WIDGET_ID,  draw);


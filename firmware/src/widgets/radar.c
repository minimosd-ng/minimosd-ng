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

 Radar by Luis Alves
 Initial version shows home
 Todo: Add waypoints

*********************************************************************/

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

static struct widget_state state;

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

#define SCALE1 200.0
#define SCALE2 500.0
#define SCALE3 1000.0

#define XSIZE 22
#define YSIZE 11
#define XCENTER 10
#define YCENTER 5

struct xy {
  unsigned char x, y;
};

static struct xy prev_home;

static char draw(void)
{
  float direction_rad, s;
  unsigned int scale, distance = mavdata.calcs.home_distance;
  char x, y, u;

  if (state.props & WIDGET_INIT) {
    for (x = 0; x < XSIZE; x++) {
      max7456_putc(state.x+x, state.y-1, 0xc7);
      max7456_putc(state.x+x, state.y+YSIZE, 0xcf);
    }
    for (y = 0; y < YSIZE; y++) {
      max7456_putc(state.x-1, state.y+y, 0xc4);
      max7456_putc(state.x+XSIZE, state.y+y, 0xbf);
    }

    state.props &= ~WIDGET_INIT;
  }

  if (config.units & LENGTH_UNITS_IMPERIAL) {
    distance *= (unsigned int) ((float) distance * M2FEET);
    u = 'f';
  } else {
    u = 'm';
  }

  if (distance < SCALE1)
    scale = SCALE1;
  else if (distance < SCALE2)
    scale = SCALE2;
  else
    scale = SCALE3;

  if (distance > scale)
    distance = scale;

  direction_rad = DEG2RAD(mavdata.calcs.home_direction);
  s = sin(direction_rad) * distance + scale;
  x = (char) ((int) (s * XSIZE)/(scale*2));
  if (x >= XSIZE)
    x = XSIZE-1;

  s = cos(direction_rad) * distance + scale;
  y = YSIZE - 1 - ((int) (s * YSIZE)/(scale*2));
  if (y == 255)
    y = 0;

  //max7456_putc(state.x + XCENTER, state.y + YCENTER, MAX7456_FONT_PLANE);
  max7456_putc(state.x + prev_home.x, state.y + prev_home.y, ' ');
  max7456_putc(state.x + x, state.y + y, MAX7456_FONT_HOME);

  max7456_printf(state.x+1, state.y-1, "%4d%c", scale, u);

  prev_home.x = x;
  prev_home.y = y;
  return 1;
}

WIDGET_DECLARE(radar_widget, RADAR_WIDGET_ID, draw);


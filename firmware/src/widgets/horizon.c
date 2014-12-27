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

 Horizon widget for minimosd-ng
 Based on smooth horizon by Jörg Rothfuchs from minimosd-extra
 Optimization and cleanup by Luis Alves

*********************************************************************/

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"


#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static struct widget_state state;

extern struct mavlink_data mavdata;

/* configurable parameters */

#define COLS  12      /* artificial horizon columns */
#define ROWS  5       /* artificial horizon rows    */

/* change factor to adapt do different cam optics */
#define PITCH_FACTOR  0.6
#define ROLL_FACTOR   1

/* end of configurable parameters */

#define NUM_FONT_CHARS  9
#define YSIZE  (ROWS * MAX7456_FONT_ROWS)

/* straight line */
#define LINE_A0	      (0xC7)      // first straight char
#define LINE_A0_OVF	  (0xD0 - 2)  // first straight overflow char -2

/* ANGLE_1 line */
#define ANGLE_1       (9)
#define LINE_A1P	    (0xD1)      // first positive staggered set 1 char
#define LINE_A1P_OVF	(0xF5 - 1)  // first positive overflow staggered set 1 char -1
#define LINE_A1N	    (0xE3)      // first negative staggered set 1 char
#define LINE_A1N_OVF	(0xF7 - 1)  // first negative overflow staggered set 1 char -1

/* ANGLE_2 line */
#define ANGLE_2       (25)
#define LINE_A2P	    (0xDA)      // first positive staggered set 2 char
#define LINE_A2P_OVF	(0xF9)      // first positive overflow staggered set 2 char
#define LINE_A2N	    (0xEC)      // first negative staggered set 2 char
#define LINE_A2N_OVF	(0xFC)      // first negative overflow staggered set 2 char

#define OVF_CHAR_OFFSET  6   // offset for the overflow subvals


struct ah_vars {
  unsigned char line_set, line_set_ovf, subval_ovf;
  int pitch_offset;
  float roll_ratio;
};

static void get_vars(struct ah_vars *vars)
{
  unsigned char roll;
  unsigned char neg = 0;

  if (mavdata.roll < 0)
    roll = (unsigned char) mavdata.roll + 180;
  else
    roll = (unsigned char) mavdata.roll;

  if (roll > 90) {
    neg = 1;
    roll += (90-roll) << 1;
  }

  if (roll > ANGLE_2) {
    vars->line_set = neg ? LINE_A2N : LINE_A2P;
    vars->line_set_ovf = neg ? LINE_A2N_OVF : LINE_A2P_OVF;
    vars->subval_ovf = 5;
  } else if (roll > ANGLE_1) {
    vars->line_set = neg ? LINE_A1N : LINE_A1P;
    vars->line_set_ovf = neg ? LINE_A1N_OVF : LINE_A1P_OVF;
    vars->subval_ovf = 6;
  } else {
    vars->line_set = LINE_A0;
    vars->line_set_ovf = LINE_A0_OVF;
    vars->subval_ovf = 7;
  }

  vars->pitch_offset = (int) (tan(DEG2RAD(mavdata.pitch) * PITCH_FACTOR * (-1)) * YSIZE) + YSIZE / 2;
  vars->roll_ratio = tan(DEG2RAD(mavdata.roll) * ROLL_FACTOR);
}

static void get_horizon(char *buf, struct ah_vars *vars)
{
  unsigned char row, col;
  int y, x;
  unsigned char subval;

  for (col = 0; col < COLS; col++) {
    x = col * MAX7456_FONT_COLS - (COLS + 1) * MAX7456_FONT_COLS / 2 + MAX7456_FONT_COLS;
    y = (int) (vars->roll_ratio * x) + vars->pitch_offset;
    if ((y < 0) || (y >= YSIZE))
      continue;
    row = y / MAX7456_FONT_ROWS;
    subval = (y % MAX7456_FONT_ROWS) / (MAX7456_FONT_ROWS / NUM_FONT_CHARS);
    buf[col + row * COLS] = vars->line_set + subval;
    if (subval > vars->subval_ovf && row < 4)
      buf[col + (row + 1) * COLS] = vars->line_set_ovf + subval - OVF_CHAR_OFFSET;
  }
}

#define WIDGET_DRAW   (0x80)

static char draw(void)
{
  static char *buf;
  struct ah_vars vars;
  unsigned char i;
  char ret = 1;

  if (state.props & WIDGET_INIT) {
    /* draw level arrows */
    max7456_putc(state.x, state.y+2, 0xc6);
    max7456_putc(state.x+13, state.y+2, 0xc5);
    state.props &= ~WIDGET_INIT;
  }

  if ((state.props & WIDGET_DRAW) == 0) {
    buf = (char *) calloc(ROWS*COLS, sizeof(char));
    if (buf == NULL)
      goto err;
    get_vars(&vars);
    get_horizon(buf, &vars);
    state.props |= WIDGET_DRAW;
    ret = 0;
  } else {
    for (i = 0; i < ROWS; i++) {
      max7456_putsn(state.x+1, state.y + (ROWS - 1) - i, &buf[COLS * i], COLS);
    }
    free(buf);
    state.props &= ~WIDGET_DRAW;
  }
err:
  return ret;
}

WIDGET_DECLARE(horizon_widget, HORIZON_WIDGET_ID, draw);


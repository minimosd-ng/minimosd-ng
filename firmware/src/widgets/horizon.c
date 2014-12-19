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
 Based on smooth horizon by Jörg Rothfuchs 
 Optimization by Luis Alves

*********************************************************************/

#include "config.h"
#include <stdio.h>
#include <string.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"
#include <math.h>

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

WIDGET_STATE(0, 0, WIDGET_DISABLED);

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

#define LINE_SET_STRAIGHT__	(0xC7 - 1)  // code of the first MAX7456 straight char -1
#define LINE_SET_STRAIGHT_O	(0xD0 - 3)  // code of the first MAX7456 straight overflow char -3
#define LINE_SET_P___STAG_1	(0xD1 - 1)  // code of the first MAX7456 positive staggered set 1 char -1
#define LINE_SET_P___STAG_2	(0xDA - 1)  // code of the first MAX7456 positive staggered set 2 char -1
#define LINE_SET_N___STAG_1	(0xE3 - 1)  // code of the first MAX7456 negative staggered set 1 char -1
#define LINE_SET_N___STAG_2	(0xEC - 1)  // code of the first MAX7456 negative staggered set 2 char -1
#define LINE_SET_P_O_STAG_1	(0xF5 - 2)  // code of the first MAX7456 positive overflow staggered set 1 char -2
#define LINE_SET_P_O_STAG_2	(0xF9 - 1)  // code of the first MAX7456 positive overflow staggered set 2 char -1
#define LINE_SET_N_O_STAG_1	(0xF7 - 2)  // code of the first MAX7456 negative overflow staggered set 1 char -2
#define LINE_SET_N_O_STAG_2	(0xFC - 1)  // code of the first MAX7456 negative overflow staggered set 2 char -1

#define OVERFLOW_CHAR_OFFSET  6  // offset for the overflow subvals

#define ANGLE_1     9     // angle above we switch to line set 1
#define ANGLE_2     25    // angle above we switch to line set 2


/* Calculate and show artificial horizon */
/* used formula: y = m * x + n <=> y = tan(a) * x + n */
void get_horizon(char *buf)
{
  unsigned char col, row;
  int pitch_line, hit, subval, x;
  unsigned char line_set, line_set_overflow, subval_overflow, roll;
  unsigned char neg = 0;
  float roll_tan;

  if (mavdata.roll < 0)
    roll = (unsigned char) mavdata.roll + 180;
  else
    roll = (unsigned char) mavdata.roll;

  if (roll > 90) {
    neg = 1;
    roll += (90-roll) << 1;
  }

  if (roll > ANGLE_2) {
    line_set = neg ? LINE_SET_N___STAG_2 : LINE_SET_P___STAG_2;
    line_set_overflow = neg ? LINE_SET_N_O_STAG_2 : LINE_SET_P_O_STAG_2;
    subval_overflow = 7;
  } else if (roll > ANGLE_1) {
    line_set = neg ? LINE_SET_N___STAG_1 : LINE_SET_P___STAG_1;
    line_set_overflow = neg ? LINE_SET_N_O_STAG_1 : LINE_SET_P_O_STAG_1;
    subval_overflow = 8;
  } else {
    line_set = LINE_SET_STRAIGHT__;
    line_set_overflow = LINE_SET_STRAIGHT_O;
    subval_overflow = 9;
  }
 
  // 90 total lines
  pitch_line = (int) (tan(ToRad(mavdata.pitch) * PITCH_FACTOR * (-1)) * YSIZE) + YSIZE / 2;
  roll_tan = tan(ToRad(mavdata.roll) * ROLL_FACTOR);

  for (col = 0; col < COLS; col++) {
    // -66 to +66 center X point at middle of each column
    x = (col * MAX7456_FONT_COLS - (COLS / 2 * MAX7456_FONT_COLS) - MAX7456_FONT_COLS / 2) + MAX7456_FONT_COLS;
    // 1 to 90 calculating hit point on Y plus offset 
    hit = (roll_tan * x) + pitch_line;
    if ((hit > 0) && (hit <= YSIZE)) {
      // 0 to 4 bottom-up
      row = (hit-1) / MAX7456_FONT_ROWS;

      // 1 to 9
      subval = (hit - (row * MAX7456_FONT_ROWS) + 1) / (MAX7456_FONT_ROWS / NUM_FONT_CHARS);
      buf[col + row * COLS] = line_set + subval;

      // check if we have to print an overflow line char
      // only if it is a char which needs overflow and if it is not the upper most row
      if (subval >= subval_overflow && row < 4)
        buf[col + (row + 1) * COLS] = line_set_overflow + subval - OVERFLOW_CHAR_OFFSET;
    }
  }
}


static void draw(void)
{
  char buf[ROWS * COLS];
  unsigned char i;

  memset(buf, ' ', ROWS * COLS);
  get_horizon(buf);
  for (i = 0; i < ROWS; i++) {
    max7456_putsn(state.x+1, state.y + (ROWS - 1) - i, &buf[COLS * i], COLS);
  }
  max7456_putc(state.x, state.y+2, 0xc6);
  max7456_putc(state.x+13, state.y+2, 0xc5);
}

WIDGET_DECLARE(horizon_widget, "Horizon", HORIZON_WIDGET_ID, draw);


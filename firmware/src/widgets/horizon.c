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
#include <math.h>
#include <stdlib.h>

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

WIDGET_STATE(0, 0, WIDGET_DISABLED);

extern struct mavlink_data mavdata;

/* Calculates artificial horizon             */
/* Based on smooth horizon by Jörg Rothfuchs */

/* with different factors we can adapt do different cam optics */
#define AH_PITCH_FACTOR 0.010471976 // conversion factor for pitch
#define AH_ROLL_FACTOR  0.017453293 // conversion factor for roll
#define AH_COLS         12          // number of artificial horizon columns
#define AH_ROWS         5           // number of artificial horizon rows
#define CHAR_COLS       12          // number of MAX7456 char columns
#define CHAR_ROWS       18          // number of MAX7456 char rows
#define CHAR_SPECIAL    9           // number of MAX7456 special chars for the artificial horizon
#define AH_TOTAL_LINES  AH_ROWS * CHAR_ROWS // helper define

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
  int pitch_line, middle, hit, subval;
  int roll;
  unsigned char line_set = LINE_SET_STRAIGHT__;
  unsigned char line_set_overflow = LINE_SET_STRAIGHT_O;
  unsigned char subval_overflow = 9;
  
  /* preset the line char attributes */
  roll = mavdata.roll;

  if ((roll >= 0 && roll < 90) || (roll >= -179 && roll < -90)) {
    /* positive angle line chars */
    if (roll < 0)
      roll += 180;
    if (roll > ANGLE_2) {
      line_set = LINE_SET_P___STAG_2;
      line_set_overflow = LINE_SET_P_O_STAG_2;
      subval_overflow = 7;
    } else if (roll > ANGLE_1) {
      line_set = LINE_SET_P___STAG_1;
      line_set_overflow = LINE_SET_P_O_STAG_1;
      subval_overflow = 8;
    }
  } else {
    /* negative angle line chars */
    if (roll > 90)
      roll -= 180;
      if (abs(roll) > ANGLE_2) {
        line_set = LINE_SET_N___STAG_2;
        line_set_overflow = LINE_SET_N_O_STAG_2;
        subval_overflow = 7;
      } else if (abs(roll) > ANGLE_1) {
        line_set = LINE_SET_N___STAG_1;
        line_set_overflow = LINE_SET_N_O_STAG_1;
        subval_overflow = 8;
      }
  }
 
  // 90 total lines
  pitch_line = (int) ((float) tan(-AH_PITCH_FACTOR * mavdata.pitch) * AH_TOTAL_LINES) + AH_TOTAL_LINES/2;

  for (col = 1; col <= AH_COLS; col++) {
    // -66 to +66 center X point at middle of each column
    middle = col * CHAR_COLS - (AH_COLS/2 * CHAR_COLS) - CHAR_COLS/2;
    // 1 to 90 calculating hit point on Y plus offset 
    hit = tan(AH_ROLL_FACTOR * mavdata.roll) * middle + pitch_line;
    if (hit >= 1 && hit <= AH_TOTAL_LINES) {
      // 0 to 4 bottom-up
      row = (hit-1) / CHAR_ROWS;

      // 1 to 9
      subval = (hit - (row * CHAR_ROWS) + 1) / (CHAR_ROWS / CHAR_SPECIAL);
   
      buf[(col-1) + (AH_ROWS - row - 1) * AH_COLS] = line_set + subval;

      // check if we have to print an overflow line char
      // only if it is a char which needs overflow and if it is not the upper most row
      if (subval >= subval_overflow && row < 4) {
        buf[(col-1) + (AH_ROWS - row - 2) * AH_COLS] = line_set_overflow + subval - OVERFLOW_CHAR_OFFSET;
      }
    }
  }
}



static void draw(void)
{
  char buf[AH_ROWS * AH_COLS];
  unsigned char i;

  memset(buf, ' ', AH_ROWS * AH_COLS);
  get_horizon(buf);
  for (i = 0; i < AH_ROWS; i++) {
    max7456_putsn(state.x+1, state.y + i, &buf[AH_COLS * i], AH_COLS);
  }
  max7456_putc(state.x, state.y+2, 0xc6);
  max7456_putc(state.x+13, state.y+2, 0xc5);
}

WIDGET_DECLARE(horizon_widget, "Horizon", HORIZON_WIDGET_ID, draw);


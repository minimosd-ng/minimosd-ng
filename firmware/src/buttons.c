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

#if CONF_RCCONFIG_ENABLE

#include "config.h"
#include "buttons.h"
#include "mavlink.h"

extern struct mavlink_data mavdata;

unsigned char get_buttons(void)
{
  unsigned char res = 0;
  int v;

  /* left right */
  v = mavdata.ch_raw[CONF_LEFTRIGHT_CH] - CONF_CH_CENTER;
  if (CONF_LEFTRIGHT_INV)
    v = -v;
  if (v > CH_HIGH)
    res |= BUTTON_RIGHT;
  else if (v < CH_LOW)
    res |= BUTTON_LEFT;

  /* up down */
  v = mavdata.ch_raw[CONF_UPDOWN_CH] - CONF_CH_CENTER;
  if (CONF_UPDOWN_INV)
    v = -v;
  if (v > CH_HIGH)
    res |= BUTTON_UP;
  else if (v < CH_LOW)
    res |= BUTTON_DOWN;

  /* back enter */
  v = mavdata.ch_raw[CONF_BACKENTER_CH] - CONF_CH_CENTER;
  if (CONF_BACKENTER_INV)
    v = -v;
  if (v > CH_HIGH)
    res |= BUTTON_ENTER;
  else if (v < CH_LOW)
    res |= BUTTON_BACK;

#if 0
  printf("0x%02x ", res);
  if (res & BUTTON_LEFT)  printf("LEFT  ");
  if (res & BUTTON_RIGHT) printf("RIGHT ");
  if (res & BUTTON_UP)    printf("UP    ");
  if (res & BUTTON_DOWN)  printf("DOWN  ");
  if (res & BUTTON_BACK)  printf("BACK  ");
  if (res & BUTTON_ENTER) printf("ENTER ");
  printf("\n");
#endif
  return res;
}

#endif

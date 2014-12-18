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

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "max7456.h"
#include "mavlink.h"
#include "widgets.h"
#include "timer.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


int main(void)
{
  /* init serial port */
  init_uart(19200);

  PRINTF("\nRESET!\n\n");
  PRINTF("MinimOSD-ng\n");

  /* init timer */
  init_timer();

  /* init max7456 */
  init_max7456();

  /* init mavlink stuff */
  init_mavlink();

  /* init widget stuff */
  init_widgets();

  /* TODO: load user settings */
  
  /* load welcome tab */
  load_widgets_tab(0);

  /* global enable interrupt */
  sei();

  while (1)
  {
    mavlink_process();
    clock_process();
  }
}


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
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "max7456.h"
#include "mavlink.h"
#include "widgets.h"
#include "timer.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

extern struct minimosd_ng_config config;
extern struct mavlink_data mavdata;
 
int main(void)
{
  unsigned char STATE = 0, c;
  unsigned int n, t = 0;
#if DEBUG
  char buf[10];
#endif

  /* load global config from eeprom */
  load_config();

  /* init serial port */
  init_uart(config.mavlink_baudrate);

  PRINTF(P("\nRESET!\n"));
  PRINTF(P("MinimOSD-ng\n"));

  /* init timer */
  init_timer();

  /* init max7456 */
  init_max7456();

  /* init mavlink stuff */
  init_mavlink();

  /* init widget stuff */
  init_widgets();

  /* global enable interrupt */
  sei();

  mavdata.calcs.home_lat = 41.290543;
  mavdata.calcs.home_lon = -8.568684;

  while (1)
  {
    n = now();
    if ((n - t) > 200) {
      t += 200;

      switch (STATE) {
      case 0:
        if (n > 3000) {
          STATE = 1;
        }
        break;
      case 1:
        widgets_process();
        break;
      default:
        break;
      }
      calc_process();
      clock_process();
    };

    if (uart_getc(&c)) {
      /* check font upload */
      max7456_process(c);
      /* parse mavlink data stream */
      mavlink_process(c);
    }
  }
}


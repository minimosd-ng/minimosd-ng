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
#include "config.h"
#include "timer.h"

/* counter that increments each 125us */
volatile unsigned char cnt125;

static unsigned int last_tick;
static unsigned char toogle = 0;

/* seconds since system startup */
/* 16bit should be enough for up to 18 hours */
static unsigned int uptime = 0;

/* counter that increments each 1ms */
volatile unsigned int jiffies;

ISR(TIMER0_COMPA_vect)
{
	if (++cnt125 == 8) {
		jiffies++;
		cnt125 = 0;
	}
}


void init_timer(void)
{
	/* Setup Timer 0 */
	/* prescaler/8 */
	TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01);
	TCNT0 = 0;

	/* top value for the timer */
	OCR0A = 249;

	/* Enable timer0 interrupt */
	TIMSK0 |= _BV(OCIE0A);
}

unsigned int nnow(void)
{
	unsigned int j, c;
	cli();
  c = cnt125;
  j = jiffies;
  sei();
  return (unsigned int) (j << 3) + c;
}

unsigned int now(void)
{
	unsigned int n;
	cli();
	n = jiffies;
	sei();
	return n;
}

void clock_process(void)
{
  unsigned int dt = now() - last_tick;

  if (dt > 500) {
    last_tick += 500;
    toogle = ~toogle;
    if (toogle)
      return;
    uptime++;
  }
}


unsigned int get_uptime(void)
{
  return uptime;
}

void get_time(unsigned int seconds, struct time *t)
{
  unsigned int i = seconds;
  t->s = i % 60;
  i /= 60;
  t->m = i % 60;
  i /= 60;
  t->h = i;
  return;
}

unsigned char get_toogle(void)
{
  return toogle;
}


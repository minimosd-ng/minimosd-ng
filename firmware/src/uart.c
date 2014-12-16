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
#include "config.h"
#include "uart.h"


static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}


static FILE uartstdout = FDEV_SETUP_STREAM(uart_putchar, NULL,
				_FDEV_SETUP_WRITE);

 
void init_uart(unsigned int b)
{
  set_baudrate(b);

	/* TX and RX enabled */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);

	/* 8 bit, no parity, 1 stop bit */
	UCSR0C = (3<<UCSZ00);

	/* redirect stdout to the uart */
	stdout = &uartstdout;
}

void set_baudrate(unsigned int b)
{
	/* baud rate select register */
	UBRR0 = (F_CPU/16)/b - 1;
}

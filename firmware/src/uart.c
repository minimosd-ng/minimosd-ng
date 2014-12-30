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
#include "uart.h"

#define FIFO_MASK (0x0f)

struct uart_fifo {
  char buf[FIFO_MASK+1];
  unsigned char rd, wr;
};

static struct uart_fifo rx_fifo;

void (* input_func)(unsigned char c);
void uart_rx(unsigned char c);

ISR(USART_RX_vect)
{
  unsigned char n_wr = (rx_fifo.wr + 1) & FIFO_MASK;
  if (n_wr == rx_fifo.rd) {
    n_wr = UDR0;
    //printf("OVR\n");
  } else {
    rx_fifo.buf[rx_fifo.wr] = UDR0;
    rx_fifo.wr = n_wr;
  }
}

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

  /* reset fifo */
  rx_fifo.wr = 0;
  rx_fifo.rd = 0;

	/* 8 bit, no parity, 1 stop bit */
	UCSR0C = (3<<UCSZ00);

	/* TX and RX enabled */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);

	/* redirect stdout to the uart */
	stdout = &uartstdout;
}

void set_baudrate(unsigned int b)
{
	/* baud rate select register */
	UBRR0 = (F_CPU/16)/b - 1;
}

unsigned char uart_getc(unsigned char *c)
{
  unsigned char ret = (rx_fifo.rd != rx_fifo.wr);
  if (ret) {
    *c = rx_fifo.buf[rx_fifo.rd++];
    rx_fifo.rd &= FIFO_MASK;
  }
  return ret;
}


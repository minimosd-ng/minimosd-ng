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
#include <util/delay.h>
#include "max7456.h"

#define SPI_START 0x01
#define SPI_END   0x02

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static unsigned int addr;

static void init_spi(void)
{
  /* setup SPI output pins */
  DDRB = (1<<DDB2) | (1 << DDB3) | (1 << DDB5) | (1 << DDB6);

  /* enable SPI, master mode */
  SPCR = (1<<SPE) | (1<<MSTR);

  /* chipselect output, high */
  DDRD |= (1<<DDD6);
  PORTD |= (1<<PD6);
}

static unsigned char spi_transfer(unsigned char b,
    unsigned char cs_ctrl)
{
  if (cs_ctrl & SPI_START)
    PORTD &= ~(1<<PD6);
  SPDR = b;
  while (!(SPSR & (1<<SPIF)));
  if (cs_ctrl & SPI_END)
    PORTD |= (1<<PD6);
  return SPDR;
}  

static unsigned char max7456_rd(unsigned char reg, unsigned char flags)
{
  spi_transfer(reg | 0x80, SPI_START & flags);
  return spi_transfer(0xff, SPI_END & flags);
}

static void max7456_wr(max7456_reg_t reg, unsigned char b, unsigned char flags)
{
  spi_transfer(reg, SPI_START & flags);
  spi_transfer(b, SPI_END & flags);
}

void max7456_putc(char c)
{
  /* set addr msb */
  max7456_wr(MAX7456_REG_DMAH, (unsigned char) (addr >> 8), SPI_START);
  /* set start addr lsb */
  max7456_wr(MAX7456_REG_DMAL, (unsigned char) addr, 0);
  max7456_wr(MAX7456_REG_DMDI, c, SPI_END);
}

void max7456_puts(char *s)
{
  max7456_wr(MAX7456_REG_DMM, MAX7456_DMM_AINC, SPI_START);
  max7456_wr(MAX7456_REG_DMAH, (unsigned char) (addr >> 8), 0);
  max7456_wr(MAX7456_REG_DMAL, (unsigned char) addr, 0);
  do {
    max7456_wr(MAX7456_REG_DMDI, *s++, 0);
  } while (*s != '\0');
  max7456_wr(MAX7456_REG_DMDI, 0xff, SPI_END);
}


void max7456_xy(unsigned char x, unsigned char y)
{
  addr = y * 30 + x;
}

void max7456_clr(void)
{
  max7456_wr(MAX7456_REG_DMM, MAX7456_DMM_VSCLR | MAX7456_DMM_CLR,
              SPI_START | SPI_END);
}

void init_max7456(void)
{
  unsigned char b;

  /* init SPI port */
  init_spi();

  /* VSYNC is connected to PD2, set as input with pullup */
  DDRD &= ~(1<<DDD2);
  PORTD |= (1<<PD2);

  /* soft-reset chip */
  max7456_wr(MAX7456_REG_VM0, MAX7456_VM0_RESET, SPI_START | SPI_END);
  _delay_ms(100);

  /* auto-detect tv standard mode */
  b = max7456_rd(MAX7456_REG_STAT, SPI_START | SPI_END);
  if (b & (MAX7456_STAT_PAL | MAX7456_STAT_NTSC)) {
    PRINTF("Video mode detected - reg=%02x\n", b);
    b &= MAX7456_STAT_PAL;
  } else {
    /* manual mode */
    /* TODO: add user setting */
    b = MAX7456_VM0_PAL;
  }

  if (b) {
    PRINTF("PAL video mode\n");
  } else { 
    PRINTF("NTSC video mode\n");
  }

  /* set auto sync mode and enable display */
  b &= ~MAX7456_VM0_SYNC;
  b |= MAX7456_VM0_VSYNC | MAX7456_VM0_ENABLE;
  max7456_wr(MAX7456_REG_VM0, b, SPI_START | SPI_END);

  /* enable auto black-level control */
  b = max7456_rd(MAX7456_REG_OSDBL, SPI_START | SPI_END);
  max7456_wr(MAX7456_REG_OSDBL, b & ~MAX7456_OSDBL_AUTO, SPI_START | SPI_END);

  /* set same brightness for all rows */
  for(b = MAX7456_REG_RB0; b <= MAX7456_REG_RB15; b++)
    max7456_wr(b, MAX7456_RB_WH90 | MAX7456_RB_BL0, SPI_START | SPI_END);

}





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
#include <avr/interrupt.h>
#include "max7456.h"
#include "widgets.h"
#include "uart.h"

#define SPI_START 0x01
#define SPI_END   0x02

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

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

void max7456_putc(unsigned char x, unsigned char y, char c)
{
  unsigned int addr = y * 30 + x;
  max7456_wr(MAX7456_REG_DMAH, (unsigned char) (addr >> 8), SPI_START);
  max7456_wr(MAX7456_REG_DMAL, (unsigned char) addr, 0);
  max7456_wr(MAX7456_REG_DMDI, c, SPI_END);
}

static unsigned char cmode = 0;
void max7456_setmode(unsigned char m)
{
  cmode = m;
}

void max7456_puts(unsigned char x, unsigned char y, char *s)
{
  unsigned int addr = y * 30 + x;
  max7456_wr(MAX7456_REG_DMM, cmode | MAX7456_DMM_AINC, SPI_START);
  max7456_wr(MAX7456_REG_DMAH, (unsigned char) (addr >> 8), 0);
  max7456_wr(MAX7456_REG_DMAL, (unsigned char) addr, 0);
  do {
    max7456_wr(MAX7456_REG_DMDI, *s++, 0);
  } while (*s != '\0');
  max7456_wr(MAX7456_REG_DMDI, 0xff, SPI_END);
}

void max7456_putsn(unsigned char x, unsigned char y, char *s, unsigned char n)
{
  unsigned int addr = y * 30 + x;
  max7456_wr(MAX7456_REG_DMM, cmode | MAX7456_DMM_AINC, SPI_START);
  max7456_wr(MAX7456_REG_DMAH, (unsigned char) (addr >> 8), 0);
  max7456_wr(MAX7456_REG_DMAL, (unsigned char) addr, 0);
  do {
    max7456_wr(MAX7456_REG_DMDI, *s++, 0);
  } while (--n > 0);
  max7456_wr(MAX7456_REG_DMDI, 0xff, SPI_END);
}

void max7456_printf(unsigned char x, unsigned char y, char *format, ...)
{
  char buf[50];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(buf, format, argptr);
  va_end(argptr);
  max7456_puts(x, y, buf);
}

void max7456_clr(void)
{
  /* clear display memory on the next VSYNC */
  max7456_wr(MAX7456_REG_DMM, MAX7456_DMM_VSCLR | MAX7456_DMM_CLR,
              SPI_START | SPI_END);
  /* wait for the operation to be completed */
  while(max7456_rd(MAX7456_REG_DMM, SPI_START | SPI_END) & MAX7456_DMM_CLR);
}

void max7456_nvmwr(unsigned char addr, unsigned char *bitmap)
{
  unsigned char i, *b = bitmap;

  /* set character address */
  max7456_wr(MAX7456_REG_CMAH, addr, 0);

  /* send 54 bytes */
  for (i = 0; i < MAX7456_BITMAP_SIZE; i++) {
    /* send pixel addr */
    max7456_wr(MAX7456_REG_CMAL, i, 0);
    /* send pixel data */
    max7456_wr(MAX7456_REG_CMDI, *b++, 0);
  }

  /* send write cmd */
  max7456_wr(MAX7456_REG_CMM, MAX7456_CMM_WRITE, 0);

  /* wait until done - bit 5 in the status register returns to 0 (12ms) */
  while (max7456_rd(MAX7456_REG_STAT, 0) & MAX7456_STAT_CMS);
}

void max7456_nvmrd(unsigned char addr, unsigned char *bitmap)
{
  unsigned char i, *b = bitmap, vm0;

  /* disable display */
  vm0 = max7456_rd(MAX7456_REG_VM0, SPI_START);
  max7456_wr(MAX7456_REG_VM0, vm0 & ~MAX7456_VM0_ENABLE, 0);

  /* set character address */
  max7456_wr(MAX7456_REG_CMAH, addr, 0);

  /* send read nvm cmd */
  max7456_wr(MAX7456_REG_CMM, MAX7456_CMM_READ, 0);

  /* wait until done - bit 5 in the status register returns to 0 (0.5us) */
  while (max7456_rd(MAX7456_REG_STAT, 0) & MAX7456_STAT_CMS);

  /* read 54 bytes */
  for (i = 0; i < MAX7456_BITMAP_SIZE; i++) {
    /* send pixel addr */
    max7456_wr(MAX7456_REG_CMAL, i, 0);
    /* send pixel data */
    *b++ = max7456_rd(MAX7456_REG_CMDO, 0);
  }

  /* enable display */
  max7456_wr(MAX7456_REG_VM0, vm0, SPI_END);
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
  _delay_ms(200);

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

#if DEBUG
  if (b) {
    PRINTF("PAL video mode\n");
  } else { 
    PRINTF("NTSC video mode\n");
  }
#endif

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

void upload_font(void)
{
  unsigned char csum, i, addr = 0, vm0;
  char buf[64];

  /* disable refresh interrupt */
  EIMSK &= ~_BV(INT0);

  /* disable display */
  vm0 = max7456_rd(MAX7456_REG_VM0, SPI_START);
  max7456_wr(MAX7456_REG_VM0, 0, 0);

  /* send ok to flag data receive ready */
  printf(P("OK\n"));

  /* expect 256 chars */
  do {
    csum = 0;
    /* expect 64 bytes per char although only 56 are useful */
    for (i = 0; i < 64; i++) {
      while(!uart_getc((unsigned char*) &buf[i]));
      csum += buf[i];
    }
    max7456_nvmwr(addr, (unsigned char*) buf);
    /* reply with the char index */
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = addr;
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = csum;
  } while (++addr != 0);
  printf(P("OK\n"));

  /* enable display */
  max7456_wr(MAX7456_REG_VM0, vm0, SPI_END);

  EIMSK |= _BV(INT0);
}

static unsigned char cnt;
void max7456_process(unsigned char c)
{
  if (c == 0xfa)
    cnt++;
  else
    cnt = 0;

  if (cnt == 10)
    upload_font();
}


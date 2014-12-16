/* basic SPI communication routines */

#include "config.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "max7456.h"

#define SPI_START 0x01
#define SPI_END   0x02

#define DEBUG 1
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

unsigned char max7456_rd(unsigned char reg)
{
  spi_transfer(reg | 0x80, SPI_START);
  return spi_transfer(0xff, SPI_END);
}

void max7456_wr(max7456_reg_t reg, unsigned char b)
{
  spi_transfer(reg, SPI_START);
  spi_transfer(b, SPI_END);
}

void max7456_putc(char c)
{
  max7456_wr(MAX7456_REG_DMDI, c);
}

void max7456_xy(unsigned char x, unsigned char y)
{
  unsigned int addr = y * 30 + x;

  /* set addr msb */
  max7456_wr(MAX7456_REG_DMAH, (unsigned char) (addr >> 8));

  /* set start addr lsb */
  max7456_wr(MAX7456_REG_DMAL, (unsigned char) addr);
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
  max7456_wr(MAX7456_REG_VM0, MAX7456_VM0_RESET);
  _delay_ms(100);

  /* auto-detect tv standard mode */
  b = max7456_rd(MAX7456_REG_STAT);
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
  max7456_wr(MAX7456_REG_VM0, b);

  /* enable auto black-level control */
  b = max7456_rd(MAX7456_REG_OSDBL);
  max7456_wr(MAX7456_REG_OSDBL, b & ~MAX7456_OSDBL_AUTO);

  /* set same brightness for all rows */
  for(b = MAX7456_REG_RB0; b <= MAX7456_REG_RB15; b++)
    max7456_wr(b, MAX7456_RB_WH90 | MAX7456_RB_BL0);

}





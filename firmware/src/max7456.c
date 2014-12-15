/* basic SPI communication routines */

#include <avr/io.h>
#include "config.h"
#include "max7456.h"

#define SPI_START 0x01
#define SPI_END   0x02

static void init_spi(void)
{
  /* setup SPI pins - outputs */
  DDRB = (1 << DDB2) | (1 << DDB3) | (1 << DDB5) | (1 << DDB6);

  /* enable SPI, master mode */
  SPCR = (1<<SPE) | (1<<MSTR);

  /* chipselect high */
  PORTB |= (1<<PB6);
}

unsigned char spi_transfer(unsigned char b,
    unsigned char cs_ctrl)
{
  if (cs_ctrl & SPI_START)
    PORTB &= ~(1<<PB6);
  SPDR = b;
  while (!(SPSR & (1<<SPIF)));
  if (cs_ctrl & SPI_END)
    PORTB |= (1<<PB6);
  return SPDR;
}  


void init_max7456(void)
{
  init_spi();
}





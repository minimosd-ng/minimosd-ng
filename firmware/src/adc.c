#include <avr/io.h>
#include "adc.h"

static unsigned char open = 0;

void open_adc(unsigned char prescale, unsigned char ref)
{
  if (open)
    return;
  open = 1;

  /* Configure clock prescale and voltage reference */
  ADCSRA = prescale;
  ADMUX = ref;

  /* Disable ADC interrupts. */
  ADCSRA &= ~( _BV(ADIE) | _BV(ADIF) );

  /* Enable ADC */
  ADCSRA |= _BV(ADEN);
}

unsigned int read_adc(unsigned char chan)
{
  /* Setup input channel */
  ADMUX &= 0xf0;
  ADMUX |= chan;

  /* Start conversion. */
  ADCSRA |= _BV(ADSC);

  /* Wait until conversion is completed. */
  while ( ADCSRA & _BV(ADSC) );

  /* Read ADC output */
  return (unsigned int) ADC;
}

void close_adc(void)
{
  open = 0;
  /* Disable ADC. */
  ADCSRA &= ~_BV(ADEN);
}


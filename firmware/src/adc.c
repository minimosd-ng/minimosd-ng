#include <avr/io.h>
#include "adc.h"

static unsigned char open = 0;

unsigned char open_adc(unsigned char prescale, unsigned char ref)
{
  if (open)
    return 1;
  else
    open = 1;

  /* Configure clock prescale and voltage reference */
  ADCSRA = prescale;
  ADMUX = ref;

  /* Disable ADC interrupts. */
  ADCSRA &= ~( _BV(ADIE) | _BV(ADIF) );

  /* Enable ADC */
  ADCSRA |= _BV(ADEN);
  return 0;
}

void start_adc(unsigned char chan)
{
  /* Setup input channel */
  ADMUX &= 0xf0;
  ADMUX |= chan;

  /* Start conversion. */
  ADCSRA |= _BV(ADSC);
}

unsigned char read_adc(unsigned int *value)
{
  /* non-block wait until adc conversion is complete */
  if (ADCSRA & _BV(ADSC))
    return 1;

  /* Read ADC output */
  *value = (unsigned int) ADC;

  return 0;
}

void close_adc(void)
{
  open = 0;
  /* Disable ADC. */
  ADCSRA &= ~_BV(ADEN);
}


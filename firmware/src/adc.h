#ifndef __ADC_H__
#define __ADC_H__

#include <avr/io.h>

enum {
    ADC_PS_2   = ((0<<ADPS2)|(0<<ADPS1)|(1<<ADPS0)),
    ADC_PS_4   = ((0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0)),
    ADC_PS_8   = ((0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)),
    ADC_PS_16  = ((1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0)),
    ADC_PS_32  = ((1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0)),
    ADC_PS_64  = ((1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0)),
    ADC_PS_128 = ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0))
};


enum {
    ADC_REF_AREF  = ((0<<REFS1)|(0<<REFS0)),
    ADC_REF_AVDD  = ((0<<REFS1)|(1<<REFS0)),
    ADC_REF_INT11 = ((1<<REFS1)|(1<<REFS0))
};


enum {
    ADC_CHAN_ADC0 = ((0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0)),
    ADC_CHAN_ADC1 = ((0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0)),
    ADC_CHAN_ADC2 = ((0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(0<<MUX0)),
    ADC_CHAN_ADC3 = ((0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0)),
    ADC_CHAN_ADC4 = ((0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(0<<MUX0)),
    ADC_CHAN_ADC5 = ((0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0)),
    ADC_CHAN_ADC6 = ((0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0)),
    ADC_CHAN_ADC7 = ((0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)),
    ADC_CHAN_TEMP = ((1<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0)),
    ADC_CHAN_VBG  = ((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0)),
    ADC_CHAN_GND  = ((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)),
};


unsigned char open_adc(unsigned char prescale, unsigned char ref);
void start_adc(unsigned char chan);
unsigned char read_adc(unsigned int *value);
void close_adc(void);

#endif /* __ADC_H__ */


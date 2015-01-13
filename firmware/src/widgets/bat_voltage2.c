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
#include "widgets.h"
#include "max7456.h"
#include "adc.h"

#define CH_VOLTAGE (ADC_CHAN_ADC0)
#define VREF       (5.0)
#define GAIN       (2.0)

static struct widget_state state;

static char draw(void)
{
  char STATE = (state.props & 0xf0) >> 4;
  unsigned int raw;
  float v;

  if (state.props & WIDGET_INIT) {
    max7456_putc(state.x, state.y, MAX7456_FONT_BATVOLTS);
    max7456_putc(state.x+6, state.y, MAX7456_FONT_VOLTS);
    state.props &= ~WIDGET_INIT;
  }

  switch (STATE) {
  default:
  case 0:
    /* open adc and start conversion */
    if (open_adc(ADC_PS_128, ADC_REF_AVDD))
      return 0;
    start_adc(CH_VOLTAGE);
    STATE = 1;
    break;
  case 1:
    if (read_adc(&raw))
      return 0;
    close_adc();

    v = ((raw * VREF) / (1<<10)) * GAIN;

    max7456_printf(state.x+1, state.y, "%5.2f", v);
    STATE = 0;
    break;
  }
  state.props &= 0x0f;
  state.props |= (STATE << 4);
  return 1;
}

WIDGET_DECLARE(batvoltage2_widget, BATVOLTAGE2_WIDGET_ID, draw);


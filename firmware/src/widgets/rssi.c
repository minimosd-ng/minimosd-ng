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
#include "mavlink.h"
#include "adc.h"

extern struct mavlink_data mavdata;
extern struct minimosd_ng_config config;

static struct widget_state state;

static char draw(void)
{
  int v;

  if (state.props & WIDGET_INIT) {
    max7456_putc(state.x, state.y, MAX7456_FONT_RADIO);
    state.props &= ~WIDGET_INIT;
  }

  /* get raw value */
  switch (config.rssi.source) {
  case RSSI_SOURCE_CHANNEL:
    v = mavdata.ch_raw[config.rssi.ch];
    break;
  case RSSI_SOURCE_ADC:
    open_adc(ADC_PS_128, ADC_REF_AVDD);
    start_adc(config.rssi.ch);
    while (read_adc(&v));
    close_adc();
    break;
  case RSSI_SOURCE_RSSI:
  default:
    v = mavdata.rssi;
    break;
  }

  switch (config.rssi.units) {
  case RSSI_PERCENT:
    v = ((v - config.rssi.min) * 100)/(config.rssi.max - config.rssi.min);
    if (v > 100)
      v = 100;
    else if (v < 0)
      v = 0;
    max7456_printf(state.x+1, state.y, "%3d%c", v, '%');
    break;
  case RSSI_RAW:
  default:
    max7456_printf(state.x+1, state.y, "%4d", v);
    break;
  }
  return 1;
}

WIDGET_DECLARE(rssi_widget, RSSI_WIDGET_ID, draw);


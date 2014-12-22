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
#include <avr/eeprom.h>


struct minimosd_ng_config config = {
  .mavlink_baudrate = 19200,

  /* tab switching */
  .tab_sw = {
    .method = CONF_TABSWITCH_METHOD,
    .ch     = CONF_TABSWITCH_CH,
  },
};

void load_config(void)
{
  // TODO: load global settings from eeprom
}


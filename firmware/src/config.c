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
#include <stdio.h>
#include <avr/eeprom.h>


/* default configuration optionally burned in the eeprom */
unsigned char widget_default_config[] EEMEM = {
  STARTUP_TAB, STARTUP_WIDGET_ID, 1, 3,

  /* tab, widget_id, x, y */

  /* tab 1 - main tab */
  1, AIRSPEED_WIDGET_ID,       1,  1,
  1, GROUNDSPEED_WIDGET_ID,    1,  2,
  1, THROTTLE_WIDGET_ID,       1,  3,
  1, RSSI_WIDGET_ID,           1,  4,

  1, ROLL_WIDGET_ID,          11,  1,
  1, PITCH_WIDGET_ID,         22, 10,
  1, HORIZON_WIDGET_ID,        8,  6,
  1, HOMEDIRECTION_WIDGET_ID, 14,  3,

  1, GPSSTATUS_WIDGET_ID,      1,  9,
  1, BATVOLTAGE_WIDGET_ID,     1, 12,
  1, BATCURRENT_WIDGET_ID,     1, 13,
  1, GPSCOORDS_WIDGET_ID,      1, 14,

  1, HEADING_WIDGET_ID,       19, 15,
  1, FLIGHTMODE_WIDGET_ID,    24, 14,
  1, WINDROSE_WIDGET_ID,      23, 15,

  1, HOMEDISTANCE_WIDGET_ID,  22,  1,
  1, RELALTITUDE_WIDGET_ID,   22,  2,
  1, ALTITUDE_WIDGET_ID,      22,  3,
  1, FLIGHTTIMER_WIDGET_ID,   21,  4,

  1, WINDSPEED_WIDGET_ID,     22,  7,
  1, CLIMBRATE_WIDGET_ID,      1,  7,

  /* tab 2 - radar */
  2, RADAR_WIDGET_ID,          4, 3,
  2, HOMEDISTANCE_WIDGET_ID,   0, 0,
  2, HOMEDIRECTION_WIDGET_ID,  7, 0,
  2, RELALTITUDE_WIDGET_ID,    0, 1,


//  11, BATREMAIN_WIDGET_ID,      0, 10,
//  11, COG_WIDGET_ID,            0, 11,


  /* tab 3 - mission */
  3, WAYPOINT_WIDGET_ID,      22,  10,

  /* tab 4 - flight stats */
  4, FLIGHTSTATS_WIDGET_ID,    0,  0,

  /* tab 5 - debug */
  5, RCCHANNELS_WIDGET_ID,    12,  0,
  5, TEMPERATURE_WIDGET_ID,   15,  3,

  ALL_TABS, TABINFO_WIDGET_ID, 27, 0,
  TAB_TABLE_END,
};


struct minimosd_ng_config config = {
  .vehicle = CONF_VEHICLE,
  .mavlink_baudrate = CONF_MAVLINK_BAUDRATE,

  .units = CONF_DEFAULT_UNITS,

  .rssi = {
    .source = CONF_RSSI_SOURCE,
    .min = CONF_RSSI_MIN,
    .max = CONF_RSSI_MAX,
    .units = CONF_RSSI_UNITS,
  },

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


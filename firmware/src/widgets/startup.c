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
#include <string.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

WIDGET_STATE(0, 0, WIDGET_DISABLED);

extern struct mavlink_data mavdata;
extern mavlink_status_t status;
extern struct minimosd_ng_config config;


static void draw(void)
{
  char buf[30];
  sprintf(buf, "MinimOSD-ng %dv%d", VERSION_MAJOR, VERSION_MINOR);
  max7456_puts(state.x, state.y, buf);
  sprintf(buf, "Mavlink packets: %d", status.packet_rx_success_count);
  max7456_puts(state.x, state.y+2, buf);
  sprintf(buf, "Mavlink baudrate: %d", config.mavlink_baudrate);
  max7456_puts(state.x, state.y+3, buf);
}

WIDGET_DECLARE(startup_widget, STARTUP_WIDGET_ID,  draw);


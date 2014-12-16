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
#include "uart.h"
#include "widgets.h"
#include <stdio.h>
#include "mavlink/v1.0/ardupilotmega/mavlink.h"


#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static mavlink_message_t msg; 
static mavlink_status_t status;

void init_mavlink(void)
{

}

void mavlink_process(void)
{
  unsigned char c;

  if (!uart_getc(&c))
    return;

  if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
    set_widget_mavdata(&msg);
  }
}


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

/* widgets */
extern struct widget \
  pitch_widget,
  roll_widget;

WIDGETS( \
  &pitch_widget,
  &roll_widget );

void configure_widgets(void)
{
  unsigned char i;

  for (i = 0; i < WIDGETS_NUM-1; i++)
    all_widgets[i]->configure(0, 0);
}


void set_widget_mavdata(mavlink_message_t *msg)
{
  unsigned char i;

  for (i = 0; i < WIDGETS_NUM-1; i++) {
    if (all_widgets[i]->set_mavdata) {
      all_widgets[i]->set_mavdata(msg);
    }
  }
}




void render_widgets(void)
{
  unsigned char i;

  for (i = 0; i < WIDGETS_NUM-1; i++)
    all_widgets[i]->draw();
}




#include "config.h"
#include <stdio.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static unsigned char x, y, props;
static int v;


/* configure widget based on eeprom data */
static void configure(unsigned int addr, unsigned char len)
{
  x = 0;
  y = 0;
  props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void set_mavdata(mavlink_message_t *msg)
{
  if (msg->msgid != MAVLINK_MSG_ID_ATTITUDE)
    return;
  v = (int) ToDeg(mavlink_msg_attitude_get_pitch(msg));
  PRINTF("pitch widget: value=%d\n", pitch);
}

static void draw(void)
{
  char buf[10];
  sprintf(buf, "%4d%c%c", v, 0x05, 0x07);
  max7456_xy(x, y);
  max7456_puts(buf);
}


WIDGETS_WIDGET(pitch_widget, "Pitch", configure, set_mavdata, draw);


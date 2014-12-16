#include "config.h"
#include <stdio.h>
#include "widgets.h"
#include "max7456.h"
#include "mavlink.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static unsigned char x, y, props;
static char nr_sats, fix_type;
static int eph;


/* configure widget based on eeprom data */
static void configure(unsigned int addr, unsigned char len)
{
  x = 15;
  y = 0;
  props = WIDGET_ENABLED | WIDGET_VISIBLE;
}

static void set_mavdata(mavlink_message_t *msg)
{
  if (msg->msgid != MAVLINK_MSG_ID_GPS_RAW_INT)
    return;

  nr_sats = (unsigned char) mavlink_msg_gps_raw_int_get_satellites_visible(msg);
  fix_type = (unsigned char) mavlink_msg_gps_raw_int_get_fix_type(msg);
  eph = (int) mavlink_msg_gps_raw_int_get_eph(msg);

  PRINTF("gps widget: nr_sats=%d fix_type=%d eph=%d\n", nr_sats, fix_type, eph);
}

static void draw(void)
{
  char buf[10];

  switch (fix_type) {
  case 0:
  case 1:
    buf[0] = 0x20; // 0x2a
    break;
  case 2:
    buf[0] = 0x1f;
    break;
  case 3:
    buf[0] = 0x0f;
    break;
  }

  if (eph >= 200)
    buf[0] = 0x20;

  sprintf(&buf[1], "%d", nr_sats);
  max7456_xy(x, y);
  max7456_puts(buf);
}


WIDGETS_WIDGET(gpsstat_widget, "Gps stats", configure, set_mavdata, draw);


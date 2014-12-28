#ifndef __WIDGETS_H__
#define __WIDGETS_H__
#include <string.h>

/* Widgets unique ID */
enum {
  PITCH_WIDGET_ID = 0,
  ROLL_WIDGET_ID,
  RSSI_WIDGET_ID,
  ALTITUDE_WIDGET_ID,
  FLIGHTTIMER_WIDGET_ID,
  GPSCOORDS_WIDGET_ID,
  GPSSTATUS_WIDGET_ID,
  RCCHANNELS_WIDGET_ID,
  BATVOLTAGE_WIDGET_ID,
  BATCURRENT_WIDGET_ID,
  BATREMAIN_WIDGET_ID,
  STARTUP_WIDGET_ID,
  COG_WIDGET_ID,
  WINDROSE_WIDGET_ID,
  HEADING_WIDGET_ID,
  HOMEDISTANCE_WIDGET_ID,
  HOMEDIRECTION_WIDGET_ID,
  HORIZON_WIDGET_ID,
  FLIGHTSTATS_WIDGET_ID,
  RADAR_WIDGET_ID,
  WAYPOINT_WIDGET_ID,
  RELALTITUDE_WIDGET_ID,
  GROUNDSPEED_WIDGET_ID,
  AIRSPEED_WIDGET_ID,
  THROTTLE_WIDGET_ID,
  TEMPERATURE_WIDGET_ID,
  WINDSPEED_WIDGET_ID,
  FLIGHTMODE_WIDGET_ID,
  CLIMBRATE_WIDGET_ID,
};

/* we should always have at least one tab */
/* tab ID 0xff is reserved for end of table indicator */
#define STARTUP_TAB   0x00
#define TAB_TABLE_END 0xff

/* widget properties */
#define WIDGET_DISABLED 0x00
#define WIDGET_ENABLED  0x01
#define WIDGET_INIT     0x02

struct widget_config {
  unsigned char tab;
  unsigned char id;
  unsigned char x, y;
};

struct widget_state {
  unsigned char x, y, props;
};

/* widget */
struct widget {
  /* widget unique id */
  unsigned char id;

  /* set/get widget state */
  struct widget_state* (*do_state)(struct widget_state *s);

  /* drawing function */
  char (*draw)(void);
};


void init_widgets(void);
void widgets_process(void);


/* helper macros */

#define WIDGET_DECLARE(name, id, draw) \
static struct widget_state* do_state(struct widget_state *s) \
{ \
  struct widget_state *my_state = &state; \
  if (s) \
    memcpy(my_state, s, sizeof(struct widget_state)); \
  return my_state; \
} \
struct widget name = { id, do_state, draw }

#define WIDGETS_NUM (sizeof(all_widgets)/sizeof(struct widgets *))

#define WIDGETS(...) \
struct widget *all_widgets[] = {__VA_ARGS__}

#define WIDGET_IMPORT(name) \
extern struct widget name;

#endif


#ifndef __WIDGETS_H__
#define __WIDGETS_H__

enum {
  PITCH_WIDGET_ID = 0,
  ROLL_WIDGET_ID,
  RSSI_WIDGET_ID,
  ALTITUDE_WIDGET_ID,
  CLOCK_WIDGET_ID,
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
};


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
  void (*draw)(void);
};


void load_widgets_tab(unsigned char tab);
void init_widgets(void);


/* helper macros */

#define WIDGET_DECLARE(name, id, draw) \
static struct widget_state* do_state(struct widget_state *s) \
{ \
  struct widget_state *my_state = &state; \
  if (s) \
    memcpy(my_state, s, sizeof(struct widget_state)); \
  return my_state; \
} \
const struct widget name = { id, do_state, draw }

#define WIDGET_STATE(_x, _y, _props) \
static struct widget_state state = { .x = _x, .y = _y, .props = _props }

#define WIDGETS_NUM (sizeof(all_widgets)/sizeof(struct widgets *))

#define WIDGETS(...) \
const struct widget *all_widgets[] = {__VA_ARGS__}

#define WIDGET_IMPORT(name) \
extern struct widget name;

#endif


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
};


/* widget properties */
#define WIDGET_DISABLED 0x00
#define WIDGET_ENABLED  0x01

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
  /* widget name */
  char *name;

  /* widget unique id */
  unsigned char id;

  /* set/get widget state */
  void (*set_state)(struct widget_state *s);
  struct widget_state* (*get_state)(void);

  /* drawing function */
  void (*draw)(void);
};


void load_widgets_tab(unsigned char tab);
void init_widgets(void);


/* helper macros */

#define WIDGET_DECLARE(name, title, id, draw) \
static void set_state(struct widget_state *s) \
{ memcpy(&state, s, sizeof(struct widget_state)); } \
static struct widget_state *get_state(void) \
{ return &state; } \
const struct widget name = { title, id, set_state, get_state, draw }

#define WIDGET_STATE(_x, _y, _props) \
static struct widget_state state = { .x = _x, .y = _y, .props = _props }

#define WIDGETS_NUM (sizeof(all_widgets)/sizeof(struct widgets *))

#define WIDGETS(...) \
const struct widget *all_widgets[] = {__VA_ARGS__}

#define WIDGET_IMPORT(name) \
extern struct widget name;

#endif


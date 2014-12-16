#ifndef __WIDGETS_H__
#define __WIDGETS_H__

#include <stdio.h>
#include "mavlink/v1.0/ardupilotmega/mavlink.h"

enum {
  PITCH_WIDGET_ID,
};



/* widget properties */
#define WIDGET_ENABLED 0x80
#define WIDGET_VISIBLE 0x02


/* widget */
struct widget {
  /* widget name */
  char *name;

  /* drawing function */
  void (*configure)(unsigned int addr, unsigned char len);

  /* set_data from mavlink message */
  void (*set_mavdata)(mavlink_message_t *msg);
  /* mavlink message type filter */
  unsigned char mav_msgid;

  /* drawing function */
  void (*draw)(void);
};


void configure_widgets(void);
void render_widgets(void);
void set_widget_mavdata(mavlink_message_t *msg);


#define WIDGETS_WIDGET(name, id, configure, set_mavdata, mav_msgid, draw) \
const struct widget name = { id, configure, set_mavdata, mav_msgid, draw }

#define WIDGETS_NUM (sizeof(all_widgets)/sizeof(struct widgets *))

#define WIDGETS(...) \
const struct widget *all_widgets[] = {__VA_ARGS__, NULL}


#endif


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

  /* drawing function */
  void (*draw)(void);
};


void configure_widgets(void);
void render_widgets(void);


#define WIDGETS_WIDGET(name, id, configure, draw) \
const struct widget name = { id, configure, draw }

#define WIDGETS_NUM (sizeof(all_widgets)/sizeof(struct widgets *))

#define WIDGETS(...) \
const struct widget *all_widgets[] = {__VA_ARGS__, NULL}


#endif


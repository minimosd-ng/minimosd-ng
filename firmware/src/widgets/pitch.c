#include "config.h"
#include <stdio.h>
#include <string.h>
#include "widgets.h"
#include "max7456.h"

static unsigned char x, y, props;

void configure(unsigned int addr, unsigned char len);
void draw(void);

WIDGETS_WIDGET(pitch_widget, "Pitch", configure, draw);

/* configure widget based on eeprom data */
void configure(unsigned int addr, unsigned char len)
{
  x = 0;
  y = 0;
  props = WIDGET_ENABLED | WIDGET_VISIBLE;
}


void draw(void)
{
  max7456_xy(x, y);
  max7456_putc('P');
}



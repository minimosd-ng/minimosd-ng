#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#if CONF_RCCONFIG_ENABLE
unsigned char get_buttons(void);
#endif

#define BUTTON_LEFT   0x01
#define BUTTON_RIGHT  0x02
#define BUTTON_UP     0x04
#define BUTTON_DOWN   0x08
#define BUTTON_BACK   0x10
#define BUTTON_ENTER  0x20

#define CH_HIGH       (CONF_CH_HIGH - CONF_CH_CENTER)
#define CH_LOW        (CONF_CH_LOW - CONF_CH_CENTER)


#endif

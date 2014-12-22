#ifndef __CONFIG_H__
#define __CONFIG_H__

/* maximum number of display tabs */
#define CONF_MAX_TABS   20

#define CONF_TABSWITCH_CH       CH8
#define CONF_TABSWITCH_METHOD   TAB_SWITCH_DEMO
#define CONF_TABSWITCH_CH_MIN   (1000)
#define CONF_TABSWITCH_CH_MAX   (1950)


#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define F_CPU 16000000L

/* number of mavlink comm channels */
#define MAVLINK_COMM_NUM_BUFFERS 1



#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5
#define CH7 6
#define CH8 7



enum {
  TAB_SWITCH_PERCENT,
  TAB_SWITCH_TOGGLE,
  TAB_SWITCH_PUSH,
  TAB_SWITCH_DEMO,
};

struct tab_switch {
  unsigned char ch, method;
};

struct minimosd_ng_config {
  unsigned int mavlink_baudrate;
  struct tab_switch tab_sw;
};

void load_config(void);

#endif

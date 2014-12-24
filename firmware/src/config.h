#ifndef __CONFIG_H__
#define __CONFIG_H__

/* maximum number of display tabs */
#define CONF_MAX_TABS   20

#define CONF_TABSWITCH_CH       CH8
#define CONF_TABSWITCH_METHOD   TAB_SWITCH_DEMO
#define CONF_TABSWITCH_CH_MIN   (1000)
#define CONF_TABSWITCH_CH_MAX   (1950)

/* default unit system */
#define DEFAULT_UNITS           (LENGTH_UNITS_METRIC | TEMPERATURE_UNIT_FAHRENHEIT)




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


/* unit system definitions */
#define LENGTH_UNITS_METRIC         0x01
#define LENGTH_UNITS_IMPERIAL       0x02
#define TEMPERATURE_UNIT_CELCIUS    0x04
#define TEMPERATURE_UNIT_FAHRENHEIT 0x08


#define M2FEET    (3.2808)
#define MILE2FEET (5280)
#define M2MILE    (M2FEET / MILE2FEET)


enum {
  TAB_SWITCH_PERCENT,
  TAB_SWITCH_TOGGLE,
  TAB_SWITCH_PUSH,
  TAB_SWITCH_DEMO,
};

struct tab_switch {
  unsigned char ch, method;
};


/* RSSI configuration options */
/* source */
enum {
  RSSI_SOURCE_RSSI,
  RSSI_SOURCE_CHANNEL,
  RSSI_SOURCE_ADC
};
/* chanel */
enum {
  RSSI_CH1 = 0,
  RSSI_CH2 = 1,
  RSSI_CH3 = 2,
  RSSI_CH4 = 3,
  RSSI_CH5 = 4,
  RSSI_CH6 = 5,
  RSSI_CH7 = 6,
  RSSI_CH8 = 7,
  RSSI_ADC0 = 0,
  RSSI_ADC1 = 1,
  RSSI_ADC2 = 2,
  RSSI_ADC3 = 3,
  RSSI_ADC4 = 4,
  RSSI_ADC5 = 5,
  RSSI_ADC6 = 6,
  RSSI_ADC7 = 7,
};
/* units */
enum {
  RSSI_PERCENT,
  RSSI_RAW,
};
struct rssi_config {
  unsigned char source;
  unsigned char ch;
  unsigned int min, max;
  unsigned char units;
};

struct minimosd_ng_config {
  /* mavlink baudrate */
  unsigned int mavlink_baudrate;

  /* unit system */
  unsigned char units;

  /* rssi config */
  struct rssi_config rssi;

  /* tab switching configuration */
  struct tab_switch tab_sw;
};

void load_config(void);

#endif

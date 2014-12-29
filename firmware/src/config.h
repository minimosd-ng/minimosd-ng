#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <avr/pgmspace.h>

/* default config options */

/* default vehicle */
#define CONF_VEHICLE            APM_PLANE
/* mavlink baudrate */
#define CONF_MAVLINK_BAUDRATE   (19200)

/* config navigation control */
#define CONF_ENTERCOMB          (BUTTON_RIGHT | BUTTON_BACK | BUTTON_DOWN)
#define CONF_LEFTRIGHT_CH       CH1
#define CONF_LEFTRIGHT_INV      0
#define CONF_UPDOWN_CH          CH2
#define CONF_UPDOWN_INV         0
#define CONF_BACKENTER_CH       CH4
#define CONF_BACKENTER_INV      0
#define CONF_CH_CENTER          (1500)
#define CONF_CH_HIGH            (1700)
#define CONF_CH_LOW             (1300)

/* tab config */
/* maximum number of display tabs */
#define CONF_MAX_TABS           (20)
#define CONF_TABSWITCH_CH       CH8
#define CONF_TABSWITCH_METHOD   TAB_SWITCH_PERCENT
#define CONF_TABSWITCH_CH_MIN   (1000)
#define CONF_TABSWITCH_CH_MAX   (1950)

/* unit system */
#define CONF_DEFAULT_UNITS      (LENGTH_UNITS_IMPERIAL | TEMPERATURE_UNIT_CELCIUS)

/* rssi config */
#define CONF_RSSI_SOURCE        (RSSI_SOURCE_RSSI)
#define CONF_RSSI_MIN           (0)
#define CONF_RSSI_MAX           (255)
#define CONF_RSSI_UNITS         (RSSI_PERCENT)


/* end of default config options */
/*********************************/

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define F_CPU 16000000L

#define MAVLINK_COMM_NUM_BUFFERS 1

#define APM_PLANE 0
#define APM_COPTER 1

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

/* conversion constants */
#define M2FEET                      (3.2808)
#define MILE2FEET                   (5280)
#define M2MILE                      (M2FEET / MILE2FEET)

#define CELCIUS2FAHR(x)             ((x * 18)/10 + 3200)
#define RAD2DEG(x)                  (x*57.2957795131)
#define DEG2RAD(x)                  (x*0.0174532925199)


/* helper to use strings in prog mem */
/* assuming a local "buf" is defined */
#define P(s) (strcpy_P(buf, PSTR(s)))


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
  /* type of vehicle */
  unsigned char vehicle;

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

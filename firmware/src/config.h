#ifndef __CONFIG_H__
#define __CONFIG_H__

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define F_CPU 16000000L

/* number of mavlink comm channels */
#define MAVLINK_COMM_NUM_BUFFERS 1

struct minimosd_ng_config {
  unsigned char display_tab;
  unsigned int mavlink_baudrate;
};

#endif

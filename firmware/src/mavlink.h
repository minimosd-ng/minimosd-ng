#ifndef __MAVLINK_H__
#define __MAVLINK_H__

#include "mavlink/v1.0/ardupilotmega/mavlink.h"

void init_mavlink(void);
void mavlink_process(void);

#define ToDeg(x) (x*57.2957795131)

#endif


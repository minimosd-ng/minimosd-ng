#ifndef __MAVLINK_H__
#define __MAVLINK_H__

#include "mavlink/v1.0/ardupilotmega/mavlink.h"

void init_mavlink(void);
void mavlink_process(void);
void calc_process(void);

struct calc_data {
  unsigned char has_home;
  unsigned int home_distance;
  unsigned int home_direction;

  float home_lat, home_lon;
};


struct mavlink_data {
  int pitch;
  int roll;

  /* GPS fix type: 0-1=no fix, 2=2D, 3=3D */
  unsigned char gps_fix_type, gps_nrsats;
  double gps_lat, gps_lon, gps_altitude;
  unsigned int gps_cog, gps_eph;

  /* battery status */
  double bat_voltage;
  int bat_current;
  unsigned char bat_remaining;

  /* global */
  unsigned char mavbeat;
  unsigned char mode;
  
  /* vrf hud */
  mavlink_vfr_hud_t vfr_hud;

  /* nav controller */
  unsigned int nav_target_bearing, nav_wp_dist;
  float nav_alt_error, nav_aspd_error, nav_xtrack_error;
  
  /* mission */
  unsigned char wp_number;
  
  /* wind */
  float wind_direction, wind_speed;

  unsigned int temperature;
  
  unsigned int home_alt;

  /* raw channel pwm */
  unsigned int ch_raw[8];
  unsigned char rssi;

  struct calc_data calcs; 
};

#define ToDeg(x) (x*57.2957795131)
#define ToRad(x) (x*0.0174532925199)

#endif


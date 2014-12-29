#ifndef __MAVLINK_H__
#define __MAVLINK_H__

#include "mavlink/v1.0/ardupilotmega/mavlink.h"

void init_mavlink(void);
void mavlink_process(unsigned char c);
void calc_process(void);

struct flight_stats {
  unsigned int flight_start, flight_end;
  unsigned int max_home_distance;
  unsigned int max_airspeed, max_groundspeed;
  unsigned int max_home_altitude;
  unsigned int max_widspeed;
  unsigned int used_battery;
  unsigned long total_distance;
};

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
  unsigned char custom_mode, base_mode;

  /* vrf hud */
  mavlink_vfr_hud_t vfr_hud;

  /* nav controller */
  int nav_bearing;
  unsigned int nav_wp_distance;
  float nav_alt_error, nav_aspd_error, nav_xtrack_error;

  /* mission */
  unsigned char wp_sequence;

  /* wind */
  int wind_direction;
  float wind_speed;

  unsigned int temperature;

  /* raw channel pwm */
  unsigned int ch_raw[8];
  unsigned char rssi;

  struct calc_data calcs;
  struct flight_stats stats;

  /* TODO: unused, cleanup later */
  //unsigned int relative_alt;
};


enum {
  PLANE_MODE_MANUAL     = 0,
  PLANE_MODE_CIRCLE     = 1,
  PLANE_MODE_STABILIZE  = 2,
  PLANE_MODE_TRAINING   = 3,
  PLANE_MODE_ACRO       = 4,
  PLANE_MODE_FBWA       = 5,
  PLANE_MODE_FBWB       = 6,
  PLANE_MODE_CRUISE     = 7,
  PLANE_MODE_AUTOTUNE   = 8,
  PLANE_MODE_AUTO       = 10,
  PLANE_MODE_RTL        = 11,
  PLANE_MODE_LOITER     = 12,
  PLANE_MODE_GUIDED     = 15,
  COPTER_MODE_STABILIZE = 100,
  COPTER_MODE_ACRO      = 101,
  COPTER_MODE_ALTHOLD   = 102,
  COPTER_MODE_AUTO      = 103,
  COPTER_MODE_GUIDED    = 104,
  COPTER_MODE_LOITER    = 105,
  COPTER_MODE_RTL       = 106,
  COPTER_MODE_CIRCLE    = 107,
  COPTER_MODE_LAND      = 109,
  COPTER_MODE_DRIFT     = 111,
  COPTER_MODE_SPORT     = 113,
  COPTER_MODE_POSHOLD   = 116,
};

#endif


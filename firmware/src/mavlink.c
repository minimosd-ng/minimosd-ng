/*******************************************************************

MinimOSD-ng
Copyright (C) 2014  Luis Alves

This file is part of MinimOSD-ng.

MinimOSD-ng is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

MinimOSD-ng is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MinimOSD-ng.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************/

#include "config.h"
#include "widgets.h"
#include "mavlink.h"
#include "timer.h"
#include <stdlib.h>


mavlink_message_t msg;
mavlink_status_t status;

struct mavlink_data mavdata;

void init_mavlink(void)
{
  struct flight_stats *s = &mavdata.stats;
  mavdata.calcs.has_home = 0;
  s->flight_start = 0;
  s->flight_end = 0;
  s->total_distance = 0;

  s->max_home_distance = 0;
  s->max_airspeed = 0;
  s->max_groundspeed = 0;
  s->max_home_altitude = 0;
  s->max_widspeed = 0;
}

void mavlink_parse_msg(mavlink_message_t *msg)
{
  switch(msg->msgid) {
  case MAVLINK_MSG_ID_HEARTBEAT:
    mavdata.custom_mode = (unsigned char) mavlink_msg_heartbeat_get_custom_mode(msg);
    mavdata.base_mode = mavlink_msg_heartbeat_get_base_mode(msg);
    break;
  case MAVLINK_MSG_ID_SYS_STATUS:
    mavdata.bat_voltage = mavlink_msg_sys_status_get_voltage_battery(msg) / 1000.0;
    mavdata.bat_current = mavlink_msg_sys_status_get_current_battery(msg);
    mavdata.bat_remaining = mavlink_msg_sys_status_get_battery_remaining(msg);
    break;
  case MAVLINK_MSG_ID_GPS_RAW_INT:
    mavdata.gps_altitude = mavlink_msg_gps_raw_int_get_alt(msg) / 1000.0;
    mavdata.gps_lat = mavlink_msg_gps_raw_int_get_lat(msg) / 10000000.0;
    mavdata.gps_lon = mavlink_msg_gps_raw_int_get_lon(msg) / 10000000.0;
    mavdata.gps_fix_type = mavlink_msg_gps_raw_int_get_fix_type(msg);
    mavdata.gps_nrsats = mavlink_msg_gps_raw_int_get_satellites_visible(msg);
    mavdata.gps_cog = mavlink_msg_gps_raw_int_get_cog(msg);
    mavdata.gps_eph = mavlink_msg_gps_raw_int_get_eph(msg);
    break;
  case MAVLINK_MSG_ID_VFR_HUD:
    mavdata.vfr_hud.airspeed = mavlink_msg_vfr_hud_get_airspeed(msg);
    mavdata.vfr_hud.groundspeed = mavlink_msg_vfr_hud_get_groundspeed(msg);
    mavdata.vfr_hud.alt = mavlink_msg_vfr_hud_get_alt(msg);
    mavdata.vfr_hud.climb = mavlink_msg_vfr_hud_get_climb(msg);
    mavdata.vfr_hud.heading = mavlink_msg_vfr_hud_get_heading(msg);
    mavdata.vfr_hud.throttle = mavlink_msg_vfr_hud_get_throttle(msg);
    break;
  case MAVLINK_MSG_ID_ATTITUDE:
    mavdata.pitch = RAD2DEG(mavlink_msg_attitude_get_pitch(msg));
    mavdata.roll = RAD2DEG(mavlink_msg_attitude_get_roll(msg));
    //mavdata.yaw = RAD2DEG(mavlink_msg_attitude_get_yaw(msg));
    break;
  case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
    //nav_roll = mavlink_msg_nav_controller_output_get_nav_roll(msg);
    //nav_pitch = mavlink_msg_nav_controller_output_get_nav_pitch(msg);
    //nav_bearing = mavlink_msg_nav_controller_output_get_nav_bearing(msg);
    mavdata.nav_bearing = mavlink_msg_nav_controller_output_get_target_bearing(msg);
    mavdata.nav_wp_distance = mavlink_msg_nav_controller_output_get_wp_dist(msg);
    mavdata.nav_alt_error = mavlink_msg_nav_controller_output_get_alt_error(msg);
    mavdata.nav_aspd_error = mavlink_msg_nav_controller_output_get_aspd_error(msg);
    mavdata.nav_xtrack_error = mavlink_msg_nav_controller_output_get_xtrack_error(msg);
    break;
  case MAVLINK_MSG_ID_MISSION_CURRENT:
    mavdata.wp_sequence = mavlink_msg_mission_current_get_seq(msg);
    break;
  case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
    mavdata.ch_raw[0] = mavlink_msg_rc_channels_raw_get_chan1_raw(msg);
    mavdata.ch_raw[1] = mavlink_msg_rc_channels_raw_get_chan2_raw(msg);
    mavdata.ch_raw[2] = mavlink_msg_rc_channels_raw_get_chan3_raw(msg);
    mavdata.ch_raw[3] = mavlink_msg_rc_channels_raw_get_chan4_raw(msg);
    mavdata.ch_raw[4] = mavlink_msg_rc_channels_raw_get_chan5_raw(msg);
    mavdata.ch_raw[5] = mavlink_msg_rc_channels_raw_get_chan6_raw(msg);
    mavdata.ch_raw[6] = mavlink_msg_rc_channels_raw_get_chan7_raw(msg);
    mavdata.ch_raw[7] = mavlink_msg_rc_channels_raw_get_chan8_raw(msg);
    mavdata.rssi = mavlink_msg_rc_channels_raw_get_rssi(msg);
    break;
  case MAVLINK_MSG_ID_WIND:
    mavdata.wind_direction = (int) mavlink_msg_wind_get_direction(msg);
    mavdata.wind_speed = mavlink_msg_wind_get_speed(msg);
    break;
  case MAVLINK_MSG_ID_SCALED_PRESSURE:
    mavdata.temperature = mavlink_msg_scaled_pressure_get_temperature(msg); /* 0.01 celsius */
    break;
  case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    //mavdata.relative_alt = mavlink_msg_global_position_int_get_relative_alt(msg) * 0.001;
    break;
  default:
      break;
  }
}

void mavlink_process(unsigned char c)
{
  if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
    mavlink_parse_msg(&msg);
  }
}


void calc_process(void)
{
  struct calc_data *c = &mavdata.calcs;
  struct flight_stats *s = &mavdata.stats;

  /* calcs for home */
  float dlat = c->home_lat - mavdata.gps_lat;
  float dlon = c->home_lon - mavdata.gps_lon;
  float cos_lat = cos(DEG2RAD(fabs(c->home_lat)));

  /* calculate aprox home distance */
  float dlon_cos = dlon * cos_lat;
  float dist = DEG2RAD(EARTH_AVG_RADIUS) * sqrt(dlat*dlat + dlon_cos*dlon_cos);

  /* home initial bearing */
  float dlat_cos = dlat / cos_lat;
  float bearing = 90 + RAD2DEG(atan2(dlat_cos, -dlon));

  /* return bearing */
  bearing += 180;

  /* relative return bearing */
  bearing -= mavdata.vfr_hud.heading;
  if(bearing < 0)
    bearing += 360;

  c->home_distance = (unsigned int) dist;
  c->home_direction = (unsigned int) bearing;


  if (c->has_home < (SLOW_TICKS_PER_SECOND * 5)) {
    /* get gps lat/lon within 5 sec of having 2D fix */
    if (mavdata.gps_fix_type > 1) {
      c->has_home++;
    } else {
      c->has_home = 0;
    }
    c->home_lat = mavdata.gps_lat;
    c->home_lon = mavdata.gps_lon;
  } else if (c->has_home < (SLOW_TICKS_PER_SECOND * 10)){
    /* set home altitude when altitude is steady (delta < 1m) for 5 sec */
    if (abs((long) c->home_altitude - (long) mavdata.vfr_hud.alt) < 1) {
      c->has_home++;
    } else {
      c->has_home =  SLOW_TICKS_PER_SECOND * 5;
    }
    c->home_altitude = (unsigned int) mavdata.vfr_hud.alt;
  }

  /* guess when the flight starts */
  if ((s->flight_start == 0) && \
      (mavdata.vfr_hud.throttle > 10) && \
      (c->has_home) && (c->home_altitude > 10) && \
      (mavdata.vfr_hud.airspeed > 8)) {
    s->flight_end = s->flight_start = get_uptime();
  }

  /* guess when landing occurs */
  if ((s->flight_start != 0) && \
      ((mavdata.vfr_hud.throttle > 3) || \
       (mavdata.vfr_hud.airspeed > 3) || \
       (c->home_altitude > 10))) {
    s->flight_end = get_uptime();
  }

  /* total distance covered accumulator */
  if (c->has_home) {
    s->total_distance += mavdata.vfr_hud.groundspeed * SLOW_PROCESS_TICK / 1000;
  }

  /* store maximum values */
  if (s->flight_start != 0) {
    SET_MAX(s->max_home_distance, c->home_distance);
    SET_MAX(s->max_airspeed, mavdata.vfr_hud.airspeed);
    SET_MAX(s->max_groundspeed, mavdata.vfr_hud.groundspeed);
    SET_MAX(s->max_home_altitude, c->home_altitude);
    SET_MAX(s->max_widspeed, mavdata.wind_speed);
  }
}


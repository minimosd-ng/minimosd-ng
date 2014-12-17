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
#include "uart.h"
#include "widgets.h"
#include <stdio.h>
#include "mavlink.h"


#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static mavlink_message_t msg; 
static mavlink_status_t status;

struct mavlink_data mavdata;

void init_mavlink(void)
{

}


void mavlink_parse_msg(mavlink_message_t *msg)
{
  switch(msg->msgid) {
  case MAVLINK_MSG_ID_HEARTBEAT:
    mavdata.mavbeat = 1;
    mavdata.mode = (unsigned char) mavlink_msg_heartbeat_get_custom_mode(msg);
    break;
  case MAVLINK_MSG_ID_SYS_STATUS:
    mavdata.bat_voltage = mavlink_msg_sys_status_get_voltage_battery(msg) / 1000.0f; //Battery voltage, in millivolts (1 = 1 millivolt)
    mavdata.bat_current = mavlink_msg_sys_status_get_current_battery(msg); //Battery current, in 10*milliamperes (1 = 10 milliampere)         
    mavdata.bat_remaining = mavlink_msg_sys_status_get_battery_remaining(msg); //Remaining battery energy: (0%: 0, 100%: 100)
    //osd_mode = apm_mav_component;//Debug
    //osd_nav_mode = apm_mav_system;//Debug
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
    mavdata.hud_airspeed = mavlink_msg_vfr_hud_get_airspeed(msg);
    mavdata.hud_groundspeed = mavlink_msg_vfr_hud_get_groundspeed(msg);
    mavdata.hud_heading = mavlink_msg_vfr_hud_get_heading(msg); // 0..360 deg, 0=north
    mavdata.hud_throttle = (uint8_t)mavlink_msg_vfr_hud_get_throttle(msg);
    mavdata.hud_altitude = mavlink_msg_vfr_hud_get_alt(msg);
    mavdata.hud_climbrate = mavlink_msg_vfr_hud_get_climb(msg);
    break;
  case MAVLINK_MSG_ID_ATTITUDE:
    mavdata.pitch = ToDeg(mavlink_msg_attitude_get_pitch(msg));
    mavdata.roll = ToDeg(mavlink_msg_attitude_get_roll(msg));
    //  osd_yaw = ToDeg(mavlink_msg_attitude_get_yaw(msg));
    break;
  case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
//                  nav_roll = mavlink_msg_nav_controller_output_get_nav_roll(msg);
//                  nav_pitch = mavlink_msg_nav_controller_output_get_nav_pitch(msg);
//                  nav_bearing = mavlink_msg_nav_controller_output_get_nav_bearing(msg);
    mavdata.nav_target_bearing = mavlink_msg_nav_controller_output_get_target_bearing(msg);
    mavdata.nav_wp_dist = mavlink_msg_nav_controller_output_get_wp_dist(msg);
    mavdata.nav_alt_error = mavlink_msg_nav_controller_output_get_alt_error(msg);
    mavdata.nav_aspd_error = mavlink_msg_nav_controller_output_get_aspd_error(msg);
    mavdata.nav_xtrack_error = mavlink_msg_nav_controller_output_get_xtrack_error(msg);
    break;
  case MAVLINK_MSG_ID_MISSION_CURRENT:
    mavdata.wp_number = (uint8_t)mavlink_msg_mission_current_get_seq(msg);
    break;
  case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
//                    chan1_raw = mavlink_msg_rc_channels_raw_get_chan1_raw(msg);
//                    chan2_raw = mavlink_msg_rc_channels_raw_get_chan2_raw(msg);
//                    chan3_raw = mavlink_msg_rc_channels_raw_get_chan3_raw(msg);
//                    chan4_raw = mavlink_msg_rc_channels_raw_get_chan4_raw(msg);
       /*   chan5_raw = mavlink_msg_rc_channels_raw_get_chan5_raw(msg);
          chan6_raw = mavlink_msg_rc_channels_raw_get_chan6_raw(msg);
          chan7_raw = mavlink_msg_rc_channels_raw_get_chan7_raw(msg);
          chan8_raw = mavlink_msg_rc_channels_raw_get_chan8_raw(msg);
          osd_rssi = mavlink_msg_rc_channels_raw_get_rssi(msg);*/
    break;           
  case MAVLINK_MSG_ID_WIND:
    mavdata.wind_direction = mavlink_msg_wind_get_direction(msg); // 0..360 deg, 0=north
    mavdata.wind_speed = mavlink_msg_wind_get_speed(msg); //m/s
    break;
  case MAVLINK_MSG_ID_SCALED_PRESSURE:
    mavdata.temperature = mavlink_msg_scaled_pressure_get_temperature(msg);
    break;
  case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    mavdata.home_alt = mavdata.hud_altitude - (mavlink_msg_global_position_int_get_relative_alt(msg)*0.001);
    break;
  default:
      break;
  }
}

struct mavlink_data* get_mavdata(void)
{
  return &mavdata;
}

void mavlink_process(void)
{
  unsigned char c;

  if (!uart_getc(&c))
    return;

  if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
    mavlink_parse_msg(&msg);
  }
}


minimosd-ng
===========

MinimOSD-ng is a new firmware written from scratch for the MinimOSD hardware.<br>
It's *not* built under arduino and you will need an AVR programmer to burn it (like the cheap usbasp).<br>
The project is in an early stage but it can already be used.<br>


The concept
===========

MinimOSD-ng is based on the following keywords: tabs and widgets<br>
A tab is what you currently see on the screen.<br>
It allows you to have any number of tabs.<br>
One tab can have any number of widgets.<br>
<br>
A widget is the basic module of the OSD which can draw anything on screen.
<br>
<br>
The font for the max7456 can be uploaded using the rudimentary tool located in tools/osdtool (linux only). The font itself is also located in the same place (tools/osdtool/charset.mcm).<br>
Alternatively you can upload it in windows using the FW/tools from minimosd or minimosd-extra.<br>


Configuration
=============

Right now the configuration is done modifying the firmware/src/config.h
Available config options (most of them self descriptive):
* CONF_VEHICLE = [APM_PLANE, APM_COPTER]
* CONF_MAVLINK_BAUDRATE = <baudrate>
* CONF_MAX_TABS = <max_tabs>
* CONF_TABSWITCH_METHOD = [TAB_SWITCH_PERCENT, TAB_SWITCH_TOGGLE, TAB_SWITCH_PUSH, TAB_SWITCH_DEMO]
* CONF_TABSWITCH_CH = [RSSI_CH<1-8>, RSSI_ADC<0-7>]
* CONF_TABSWITCH_CH_MIN = <min_pwm>
* CONF_TABSWITCH_CH_MAX = <max_pwm>
* CONF_DEFAULT_UNITS = [LENGTH_UNITS_METRIC, LENGTH_UNITS_METRIC] | [TEMPERATURE_UNIT_CELCIUS, TEMPERATURE_UNIT_FAHRENHEIT]
* CONF_RSSI_SOURCE = [RSSI_SOURCE_RSSI, RSSI_SOURCE_CHANNEL, RSSI_SOURCE_ADC]
* CONF_RSSI_MIN = <min_rssi>
* CONF_RSSI_MAX = <max_rssi>
* CONF_RSSI_UNITS = [RSSI_PERCENT, RSSI_PERCENT]


At the moment widget configuration (tab and location on screen) is done directly at the code in firmware/src/config.c (sorry, no tool yet).<br>
<br>

Available widgets
=================

* AIRSPEED
* ALTITUDE
* BATTERY CURRENT
* BATTERY REMAINING
* BATTERY VOLTAGE
* CLIMB RATE
* COG
* CLOCK / TIMER
* FLIGHT MODE
* FLIGHT STATISTICS
* FLIGHT TIMER
* GPS COORDINATES
* GPS STATUS
* HEADING
* HOME ALTITUDE
* HOME DIRECTION
* HOME DISTANCE
* ARTIFICIAL HORIZON
* PITCH
* RADAR
* RC CHANNELS
* ROLL
* RSSI
* TEMPERATURE
* THROTTLE
* WAYPOINTS
* WIND ROSE
* WIND SPEED
* (more to come)


TODO (by priority)
==================

* Finish adding all the widgets available in MinimOSD-extra.
* Create a configuration tool (windows/linux).
* Fix bugs
* (...)


Building
========

1) Make sure you have avr-gcc and companions installed<br>
2) Clone git repository to your local disk<br>
3) Get to ./firmware/<br>
4) Type 'make'<br>
5) Burn the flash 'make flash'<br>
6) Burn eeprom: 'make eeprom'<br>
<br>
You may need to edit Makefile to correctly setup your AVR programmer.



Pull requests are welcome.

<br>
max7456 resolution (For reference)<br>
PAL   16 Rows x 30 Chars<br>
NTSC  13 Rows x 30 Chars<br>
<br>
<br>

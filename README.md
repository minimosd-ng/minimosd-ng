minimosd-ng
===========

This is a new firmware written from scratch for the MinimOSD hardware.<br>
It's not built under arduino and you will need an AVR programmer to burn it.<br>
The project is in an early stage but it can already be used with some limitations.<br>


The concept
===========

This firmware is based on the following keywords: tabs and widgets<br>
A tab is what you currently see on the screen.<br>
It allows you to have any number of tabs.<br>
One tab can have any number of widgets.<br>
<br>
A widget is the basic module of the OSD which can draw anything on screen.
<br>
<br>
At the moment the configuration is done directly in the code in the widgets.c file (no tool yet).<br>
<br>


TODO (by priority)
==================

* Finish adding all the widgets available in MinimOSD-extra.
* Add tab switching options (channel selection and values).
* Add support NTSC tv system and other measurement units.
* Create a configuration tool (windows/linux).
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

Current state
=============

Right now it can display the following widgets in any number of tabs:
* ALTITUDE
* BATTERY VOLTAGE
* BATTERY CURRENT
* BATTERY REMAINING
* CLOCK / TIMER
* COG
* FLIGHT STATISTICS
* GPS STATUS
* GPS COORDINATES
* HEADING
* HOME DIRECTION
* HOME DISTANCE
* ARTIFICIAL HORIZON
* PITCH
* RADAR
* RC CHANNELS
* ROLL
* RSSI
* WIND ROSE
* (more to come)


(README to be updated)


Pull requests are welcome.

<br>
Max7456 resolution: <br>
PAL   16 Rows x 30 Chars<br>
NTSC  13 Rows x 30 Chars<br>
<br>
<br>

# Tides Sensor

[![Build Status](https://travis-ci.org/mbustosorg/tides-sensor.svg?branch=master)](https://travis-ci.org/mbustosorg/tides-sensor)

## Dependencies

The current hardware design is based on the Teensy 3.2 using the https://www.pjrc.com/teensy/td_libs_Ethernet.html library.  

## Building

While not required, this project is built using https://platformio.org/.  The built in CLI facilitates building the application for later upload to the Teensy 3.2.

```bash
$ platformio run
```

Or you can build and deploy using the Platformio IDE.

## Configuration

The MAC, IP address and server IP addresses need to be updated for your particular network configuration.  These are found in ```main.cpp```.  The MAC address for the Teensy can be obtained using the forum entry https://forum.pjrc.com/threads/91-teensy-3-MAC-address.
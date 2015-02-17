Knobio - A physical volumbe knob for Volumio
===========================================

This is the code to accompany the Knobio board. The knobio_daemon is compiled and run on the Volumio server.
The .ino file is uploaded to the Knobio board/Arduino.

The alpha version of the daemon code is designed for the Raspberry Pi B, but has not been tested yet.
The alpha version of the .ino is for the prototype Knobio board and also has not been tested.

Usage
------
Clone this repository into your home folder.

For the alpha version:

Compile with: sudo gcc -o knobio-daemon-alpha knobio-daemon-alpha.c arduino-serial-lib.c -lasound

Upload knobio_alpha.ino to the Knobio prototype board using the Arduino IDE version 1.0 or later.

On the Pi run the daemon with the command: sudo ./knobio_daemon_alpha

The Makefile is non-functional at this point so you must manually compile using the above command.

Note: On the system daemon where daemon is being installed libasound2-dev must be installed first.

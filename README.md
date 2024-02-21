A digital compass that uses an LED ring to display the compass rose.

Upon startup, the compass enters magnetometer calibration mode, shown by the led chase sequence:

https://github.com/fmenozzi/digital_compass/assets/4451432/49a2e2fe-652e-430a-9320-a2c4e0b69866

During calibration, the compass should be moved around in all directions. Calibration ends after calibrated values do not change for five seconds.

Once the chase sequence ends, the compass rose will be displayed and the compass is now active, with the red LED pointing *very roughly* north:

https://github.com/fmenozzi/digital_compass/assets/4451432/9e37ce8d-d3a7-4e10-ac38-1c5cc8147863

Accuracy will suffer for a couple of reasons, such as being limited to only 24 possible directions and the magnetometer calibration being suboptimal. Regardless, this was a fun little proof-of-concept project for me, and my first non-trivial hardware project.

The microcontroller is a Seeeduino Xiao, and the magnetometer board uses the QMC5883L chip. The LED ring is an Adafruit Neopixel Ring 24, and a basic level shifter board does the conversions between the Xiao's needed 3.3V and 5V.

The software was programmed using Arduino. The circuit board was designed in KiCAD and manufactured by JLCPCB. The 3D-printed enclosure was designed in FreeCAD.

All software, schematics, and design files are licensed under the [MIT license](https://mit-license.org/).

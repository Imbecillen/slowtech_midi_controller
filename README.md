# Slowtech midi controller

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/Render_01.jpg)

// Based on https://github.com/FuzzyWobble/TeensyMonster


# Plans

- Modify to work with a Teensy LC. [check]

- Multiplex buttons via digital input from 4067. [check]

- Multiplex potentiometers via analog input from 4067. [check]

- Show potentiometer value as a graph bar at an 128x64px oled display. Show midi channel and cc number of corresponding potentiometer. [check]

- Fix led lights corresponding to button presses.

- Toggle button sending midi at release. Choosable? 

- Choose midi channel by holding one of the 16 buttons at startup?

- Possibility to set led brightness in some way. Button combination at startup to choose between 16 different levels?

- Save last set midi channel and buttonstate(?) to EEPROM for later startup. 

- Function button?

- Encoders?

- Buss PCB layout with ribbon connectors for easy customisaton?


# Design process

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/2017-02-21%2009.27.03.jpg)

![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/source/Render.JPG)

Breadboard prototype:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Breadboard.jpg)

Prototyping with 2 x 595 and one 4067:
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3610.JPG)

Buttons with 12 V LEDs.
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3611.JPG)

Replaced the leds with 2.1 V.
![front](https://github.com/Imbecillen/midi-controller-2.0/blob/master/hardware/designprocess/IMG_3682.JPG)

Testing potentiometers with Ableton Live throgh USB midi.

https://www.youtube.com/watch?v=3gG5vzuutMg

# Code

Resources:

https://www.pjrc.com/teensy/teensyduino.html

https://www.pjrc.com/teensy/td_midi.html

https://www.pjrc.com/teensy/td_libs_SPI.html

https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

# Hardware

- Teensy LC
- 595 shift registers (to be replaced with TLC5940NT or similar)
- 4067 multiplexers
- SSD 1306 128x64px oled i2c display
- Tactile non-toggle switches with LEDS. 

The two 595 shift registers will be replaced with TLC5940NT 16 channel LED PWM driver. 

Only needs one resistor to the IC instead of one per LED.

Prototype one will be made with through hole components, afterwards designed with SMD to make everything neater and smaller.


# Diagrams


Complete diagram of project, code not yet updated with I/O:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Slowtech_midi_controller_diagram_complete.jpg)


595 table over I/O and names:
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/595_tabel.png)


SPI table over Teensy LC I/O (we are using alternate pin 14 for SCK/clock): 
![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Teensy%20SPI.png)


I2C table:

![front](https://github.com/Imbecillen/slowtech_midi_controller/blob/master/hardware/Teensy%20i2c.png)


# Open source is key

Big thanks to the Swedish facebook group Arduinobubblan and the Arduino community forum for supporting and helping along the way.

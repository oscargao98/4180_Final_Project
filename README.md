# Smart Desk Gardening Device

**Team Members: Yuyan Oscar Gao and Zechuan Ding**

**Georgia Institute of Technology**

Watch our video presentation and demo:

Presentation (5min): https://youtu.be/BhbdnVkSK4Q

Demonstration (5min): https://youtu.be/sSNZYDFUwvU

## Table of Content
* [Project Idea](#project-idea)
* [Parts List](#parts-list)
* [Schematic and Connection Guide](#schematic-and-connection-guide)
* [Device Setup](#device-setup)
* [Source Code](#source-code)
* [Future Direction](#future-direction)

## Project Idea
This is a picture of an Epipremnum(a.k.a, Devil's Ivy) that we bought from Ikea to serve as the model for our project's demonstration.
![A picture of an epipremnum that we just bought from Ikea.](https://github.com/oscargao98/4180_Final_Project/blob/main/green's_new_home_small.jpg)

We love our desk plants: They make our room prettier, and they make us feel good when we look at them. However, sometimes we're simply too busy to spend time taking care of our green friends, because let's face it: If we had more time, we'd get a dog instead. Nevertheless, plants are living things so it deserves our love, or, our smart device's love.

Our idea for the ECE-4180(Fall 2020)'s final project is to build a smart desk gardening solution. This is a smart machine that takes care of user’s indoor plant. It is an automated device that it controlled by two Mbed microcontrollers: one controls two motors and the other one controls other functions. It blocks direct sunlight when the ambient light level is too intense, and provide additional light when there's not enough sunlight. It has a water level sensor attached to a water container. When the water level is low, it alerts the homeowner by sounding an alarm and light up a warning light, urging them to refill. It senses the soil moisture level twice a day: When the moisture level is low, it waters the plant by controlling a pipe attached to the water container. It has an easy-to-use user interface consisted of a LCD screen and six pushbuttons, which displays the environmental values (light level, soil moisture level, and water level) and allows users to set desired light level and moisture level based on the species of their plant.

![block diagram](https://github.com/oscargao98/4180_Final_Project/blob/main/block_final.png)
Block diagram of our device.

## Parts List

Electronics:
* 2 Mbed LPC1768, https://www.sparkfun.com/products/9564
* LCD Display: uLCD-144G2, https://www.sparkfun.com/products/11377
* Water Level Sensor: https://www.amazon.com/ARCELI-Sensor-Droplet-Detection-Arduino/dp/B07BP7B9TR
* Soil Moisture Sensor: SEN-13322, https://www.sparkfun.com/products/13322
* Ambient Light Sensor: TEMT-6000, https://www.sparkfun.com/products/8688
* LED Array: Adafruit NeoPixel NeoMatrix 8x8, https://www.adafruit.com/product/1487
* Red LED: https://www.sparkfun.com/products/10632
* 6 Pushbuttons: https://www.sparkfun.com/products/97
* Speaker: https://www.sparkfun.com/products/11089
* Transistor: 2N3904, https://www.sparkfun.com/products/521
* 2 Servos: HS-442, https://www.servocity.com/hs-422-servo
* Power Supply: HW-131, https://www.cafago.com/en/p-e8575.html
* Resistors: 330ohm, https://www.sparkfun.com/products/14490
* Jumper Wires(M/M and M/F): https://www.sparkfun.com/products/124, https://www.sparkfun.com/products/12794

Non-electronics:
* A desk
* A mini-shelf
* A water container
* A plant
* A curious mind

## Schematic and Connection Guide

#### Device Setup
Here's a few pictures of our fully assembled device.
![](https://github.com/oscargao98/4180_Final_Project/blob/main/ds1.png)
![](https://github.com/oscargao98/4180_Final_Project/blob/main/ds2.png)
![](https://github.com/oscargao98/4180_Final_Project/blob/main/ds3.png)
Two mbed LPC 1768 microcontrollers are used to control all the components. Servo 1 controls the pipe that allows water to flow through and water the flower.Servo 2 is attached to a cardboard, which functions as a shade. A water level sensor is taped onto the inner wall of a water container using waterproof tape. An ambient light sensor is placed facing the window to sense the sunlight level. A breadboard, consisting of an uLCD screen, six push buttons, and a speaker, is sticked onto the side of the self. 
#### Schematic
![](https://github.com/oscargao98/4180_Final_Project/blob/main/sch_new.png)

#### Connection Table for Uncommon Components
| Mbed1       |  Water Level | SEN-13322 | TEMT-6000 | NeoMatrix |HW-131|HS-442 #1| Mbed2 |
|-------------|--------------|-----------|-----------|-----------|------|---------|-------|
|             |              |           |           | +5V       | 5V   | red     |       |
| 3.3V = Vdd  |  +           | VCC       | VCC       |           |      |         |       |
| Gnd         |  -           | GND       | GND       | GND       | GND  | brown   |       |
| p5          |              |           |           | DIN       |      |         |       |
| p20         |  S           |           |           |           |      |         |       |
| p19         |              | SIG       |           |           |      |         |       |
| p18         |              |           | SIG       |           |      |         |       |
|             |              |           |           |           |      | yellow  |  p23  |

(Note: Use of external 5v power is REQUIRED, because we need enough current to drive servos and the LED array. Connect everything to a common ground.)

## Source Code
All source code is included in this repository. Run it by importing the repository into the MBed Cloud Compiler.

(Note: Make sure to import the code for MBed1 and Mbed2 separately.)

## Future Direction
During 2020's Black Friday, after failing to acquire a Sony Playstation 5, Yuyan decided to go on Adafruit.com and ordered a Raspberry Pi 4 and a wifi chip (ESP8266). We will likely replace Mbed1 with the Raspberry Pi, which can send users email notifications when the water level is low. By interfacing Mbed2 with the wifi chip, We will add IoT functionalities to this device, allowing users to monitor and manage their plants through a cloud server. Please check back as we continue our build.




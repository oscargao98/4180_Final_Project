# Smart Desk Gardening Device

**Team Members: Yuyan Oscar Gao and Zechuan Ding**

**Georgia Institute of Technology**

Watch our video presentation and demo:

Presentation (5min): https://youtu.be/BhbdnVkSK4Q

Demonstration (5min): https://youtu.be/sSNZYDFUwvU

![A nice picture of us](https://github.com/oscargao98/4180_Final_Project/blob/main/heying.png)


## Table of Content
* [Project Idea](#project-idea)
* [Parts List](#parts-list)
* [Schematic and Connection Guide](#schematic-and-connection-guide)
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
All source code is included in this repository. Run it by importing the repository into the MBed Cloud Compiler. Make sure to included all the libraries.

(Note: Make sure to import 1_garden for MBed1 and 2_garden for Mbed2, respectively.)

main.ccp for Mbed1

```cpp
#include "mbed.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "rtos.h"
#include "NeoMatrix.h"
#include "Speaker.h"
#define LED_COUNT 24 // uses a 24-led ring
#define YELLOW 0xFFFF00

enum Light_Mode{BRIGHT, NORMAL, SHADE};
const int offset = 3;

uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;
Serial pc(USBTX, USBRX); // tx, rx
Timeout countdown; // for alarm duration
Light_Mode mode = NORMAL;
Timeout snooze;

// pb
PinDetect button1(p12,PullUp);
PinDetect button2(p13,PullUp);
PinDetect button3(p14,PullUp);
PinDetect button4(p15,PullUp);
PinDetect button5(p16,PullUp);
PinDetect button6(p17,PullUp);

// sensors
AnalogIn waterSensor(p20);
AnalogIn moistureSensor(p19);
AnalogIn lightSensor(p18);

// outputs
Speaker speaker(p21);
PwmOut led(p25);
PwmOut led1(LED1);

// led panel
NeoArr array(p5, 1);

// global variables
Mutex myMut;
Mutex LED_mutex;
//volatile int button1_push = 0;
//volatile int button2_push = 0;
volatile float setWaterLevel = 0;
volatile float setMoistLevel = 0.0;
volatile float setLightLevelHigh = 0.5;
volatile float setLightLevelLow = 0.2;
int led_enable = 0;
//volatile float shadePosition = 0.0;

volatile float moisture_requirement = 0.6;
int x_light_low, x_light_high, x_moist;
// debug varibles

// sensor readings
volatile float water = 0.0;
volatile float light = 0.0;
volatile float moist = 0.0;

// callback functinos for 2 pushbuttons
void Button1_Callback (void){
    setLightLevelLow -= .1;
    setLightLevelLow = setLightLevelLow>0? setLightLevelLow:0;
    setLightLevelHigh = setLightLevelLow + 0.3;
}
void Button2_Callback (void){
    setLightLevelHigh += .1;
    setLightLevelHigh = setLightLevelHigh<1? setLightLevelHigh:1;
    setLightLevelLow = setLightLevelHigh - 0.3;
}

void Button3_Callback (void){
    // Increase moisture_requirement
    moisture_requirement += .1;
    moisture_requirement = (moisture_requirement < 0.9) ? moisture_requirement:0.9;
}

void Button4_Callback (void){
    // Decrease moisture_requirement
    moisture_requirement -= .1;
    moisture_requirement = (moisture_requirement > 0.1) ? moisture_requirement:0.1;
}

DigitalOut Shade(p23);
DigitalOut Pipe(p24);
void Button5_Callback (void){
    // Manually Water
    Pipe = 1;
}
void Button5_Callback2 (void){
    // Manually Water
    Pipe = 0;
}

int LED_enable = 1;
void Button6_Callback (void){
    // Turn off/on LED
    LED_enable = !LED_enable;
}

// move the servos
void motors_function(void const *argument){
    while(1){
        moist = moistureSensor.read();
        if (moist < moisture_requirement){
            Pipe = 1;
            Thread::wait(2000);
            Pipe = 0;
        }
        Thread::wait(10000);
    }    
}    

void light_function(void const *argument){
    while(1) {
        LED_mutex.lock();
        light = lightSensor.read();
        LED_mutex.unlock();
        if (light > setLightLevelHigh) {
            array.fillScreen(0,0,0,0);
            array.write();
            Shade = 1;    // Use the shade
        }
        else if (light<setLightLevelLow && LED_enable) {
            array.fillScreen(0,255,255,255);
            array.write();
            Shade = 0;     // Hide the shade
        }
        else {
            array.fillScreen(0,0,0,0);
            array.write();
            Shade = 0;     // Hide the shade
        }
        Thread::wait(2000);
    }
}

int main() {
    uLCD.printf("\n\rstart printing\n");
    uLCD.cls();
//    speaker.period(1.0/800.0);
    
    // led array init
    array.setBrightness(.2);    // set brightness to 0.1
    array.clear();

    // attach button callbacks
    button1.attach_deasserted(&Button1_Callback);
    button1.setSampleFrequency();
    button2.attach_deasserted(&Button2_Callback);
    button2.setSampleFrequency();
    button3.attach_deasserted(&Button3_Callback);
    button3.setSampleFrequency();
    button4.attach_deasserted(&Button4_Callback);
    button4.setSampleFrequency();
    button5.attach_deasserted(&Button5_Callback);
    button5.attach_asserted(&Button5_Callback2);
    button5.setSampleFrequency();
    button6.attach_deasserted(&Button6_Callback);
    button6.setSampleFrequency();

    // attach threads
    Thread motors(motors_function);
    Thread light_thread(light_function);
    
    int alert = 1;

    while(1) {
        water = waterSensor.read();
        moist = moistureSensor.read();
        LED_mutex.lock();
        light = lightSensor.read();
        LED_mutex.unlock();
        myMut.lock();

        // Draw a bar to display light level
        uLCD.locate(0,1);
        uLCD.printf("LIGHT: %f\n",light);
        uLCD.rectangle(15, 15+offset, 112, 32+offset, WHITE);
        uLCD.filled_rectangle(16, 16+offset, 111, 31+offset, BLACK);
        if (light > 0.9)
            uLCD.filled_rectangle(16, 16+offset, 16 + floor((light/1.0f) * 96), 31+offset, RED);
        else if (light > setLightLevelHigh)
            uLCD.filled_rectangle(16, 16+offset, 16 + floor((light/1.0f) * 96), 31+offset, YELLOW);
        else if (light < setLightLevelLow)
            uLCD.filled_rectangle(16, 16+offset, 16 + floor((light/1.0f) * 96), 31+offset, YELLOW);
        else
            uLCD.filled_rectangle(16, 16+offset, 16 + floor((light/1.0f) * 96), 31+offset, GREEN);
        x_light_low = 16 + floor((setLightLevelLow/1.0f) * 96);
        x_light_high = 16 + floor((setLightLevelHigh/1.0f) * 96);
        uLCD.line(x_light_low, 16+offset, x_light_low, 31+offset, WHITE);
        uLCD.line(x_light_high, 16+offset, x_light_high, 31+offset, WHITE);
        
        // Draw a bar to display moisture level  
        uLCD.locate(0,5);
        uLCD.printf("MOISTURE: %f\n", moist);
        uLCD.rectangle(15, 47+offset, 112, 64+offset, WHITE);
        uLCD.filled_rectangle(16, 48+offset, 111, 63+offset, BLACK);
        if (moist > moisture_requirement)
            uLCD.filled_rectangle(16, 48+offset, 16 + floor((moist/1.0f) * 96), 63+offset, GREEN);
        else if (moist < 0.2)
            uLCD.filled_rectangle(16, 48+offset, 16 + floor((moist/1.0f) * 96), 63+offset, RED);
        else
            uLCD.filled_rectangle(16, 48+offset, 16 + floor((moist/1.0f) * 96), 63+offset, YELLOW);
        x_moist = 16 + floor((moisture_requirement/1.0f) * 96);
        uLCD.line(x_moist, 48+offset, x_moist, 63+offset, WHITE);
        
        // Draw a bar to display water level
        uLCD.locate(0,9);
        uLCD.printf("WATER: %f\n", water);
        uLCD.rectangle(15, 79+offset, 112, 96+offset, WHITE);
        uLCD.filled_rectangle(16, 80+offset, 111, 95+offset, BLACK);
        if (water > 0.4)
            uLCD.filled_rectangle(16, 80+offset, 16 + floor((water/1.0f) * 96), 95+offset, GREEN);
        else if (water > 0.2)
            uLCD.filled_rectangle(16, 80+offset, 16 + floor((water/1.0f) * 96), 95+offset, YELLOW);
        else
            uLCD.filled_rectangle(16, 80+offset, 16 + floor((water/1.0f) * 96), 95+offset, RED);
        myMut.unlock();
        if (water<.2&&alert){
             led = 1;
             speaker.PlayNote(969.0, 2, 0.5);
             alert = 0;
        } else if (water<.2){led = !led;}
        Thread::wait(100); // every .1 second
    }
}
```

main.cpp for Mbed2

```cpp
#include "mbed.h"
#include "Servo.h"

Servo Pipe(p24);
Servo Shade(p23);
PwmOut led1(LED1);
PwmOut led2(LED2);
Serial pc(USBTX, USBRX); // tx, rx
DigitalIn Shade_sig(p21);
DigitalIn Pipe_sig(p22);


int main(){
    wait(1);
    while(1){
        if (Shade_sig) {
            Shade = 0.2;
        }
        else {
            Shade = 1.0;
        }
            
        if (Pipe_sig) {
            Pipe = 0.9;
        }
        else {
            Pipe = 0.2;
        }
        wait(1);
    }
    
}
```

## Future Direction
During 2020's Black Friday, after failing to acquire a Sony Playstation 5, Yuyan decided to go on Adafruit.com and ordered a Raspberry Pi 4 and a wifi chip (ESP8266). We will likely replace Mbed1 with the Raspberry Pi, which can send users email notifications when the water level is low. By interfacing Mbed2 with the wifi chip, We will add IoT functionalities to this device, allowing users to monitor and manage their plants through a cloud server. Please check back as we continue our build.




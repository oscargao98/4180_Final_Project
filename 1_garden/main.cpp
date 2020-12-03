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
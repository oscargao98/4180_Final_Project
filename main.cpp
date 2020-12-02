#include "mbed.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "rtos.h"
#include "Servo.h"
//#include "PololuLedStrip.h"
#include "NeoMatrix.h"
#define LED_COUNT 24 // uses a 24-led ring

uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;
Serial pc(USBTX, USBRX); // tx, rx
Timeout countdown; // for alarm duration

// pb
PinDetect button1(p7,PullDown);
PinDetect button2(p8,PullDown);

// sensors
AnalogIn waterSensor(p20);
AnalogIn moistureSensor(p19);
AnalogIn lightSensor(p18);

// outputs
PwmOut speaker(p21);
PwmOut led(p22);
PwmOut led1(LED1);
// motors
Servo Shade(p23);
Servo Pipe(p24);
// led panel
NeoArr array(p5, 1);

// global variables
Mutex myMut;
//volatile int button1_push = 0;
//volatile int button2_push = 0;
volatile float setWaterLevel = 0;
volatile float setMoistLevel = 0.0;
volatile float setLightLevelHigh = 0.5;
volatile float setLightLevelLow = 0.2;
volatile float shadePosition = 0.0;
volatile float pipePosition = 0.0;

// debug varibles
volatile int counting = 0; // keep track of how many seconds has passed

// sensor readings
volatile float water = 0.0;
volatile float light = 0.0;
volatile float moist = 0.0;

// callback functinos for 2 pushbuttons
void Button1_Callback (void){
    setLightLevelHigh -= .1;
    setLightLevelHigh = setLightLevelHigh>0? setLightLevelHigh:0;
}
void Button2_Callback (void){
    setLightLevelHigh += .1;
    setLightLevelHigh = setLightLevelHigh<1? setLightLevelHigh:1;
}

// move the servos, also light an LED
void motors_function(void const *argument){
    while(1){
        if (light>setLightLevelHigh)
        {
            array.fillScreen(0,0,0,0);
            array.write();
            /*
            code for lower the shade here
            */
            pc.printf("case 1\n\r");
        }
        else if (light<setLightLevelLow)
        {
            array.fillScreen(0,255,255,255);
            array.write();
            pc.printf("case 2\n\r");
        }
        else
        {
            array.fillScreen(0,0,0,0);
            array.write();
            Shade = 0;
            /*
            code for openning the shade here
            */
            pc.printf("case 3\n\r");

        }

        pipePosition = moist<0.6? 1:0;
        Pipe = pipePosition;
        Thread::wait(1000);
    }
}    

int main() {
    uLCD.printf("\n\rstart printing\n");
    uLCD.cls();
    speaker.period(1.0/800.0);
    
    // led init
    array.setBrightness(.1);    // set brightness to 0.1
    array.clear();

    // attach button callbacks
    button1.attach_deasserted(&Button1_Callback);
    button1.setSampleFrequency();
    button2.attach_deasserted(&Button2_Callback);
    button2.setSampleFrequency();

    // attach threads
    Thread motors(motors_function);
    
    while(1) {
        water = waterSensor.read();
        moist = moistureSensor.read();
        light = lightSensor.read();
        myMut.lock();
        uLCD.locate(0,2);
        uLCD.printf("%d:\n\r",counting);
        uLCD.printf("water: %f\n\r",water);
        uLCD.printf("moist: %f\n\r",moist);
        uLCD.printf("light: %f\n\r",light);
        
        uLCD.locate(0,10);
        uLCD.printf("SetLightHigh: %1.1f\n\r", setLightLevelHigh);
        uLCD.printf("SetLightLow: %1.1f\n\r", setLightLevelLow);
        uLCD.printf("SetMoistLevel: %1.1f\n\r", setMoistLevel);        
        myMut.unlock();
        counting++;
         
        // sound the alarm if water level too low
        speaker = moist<setMoistLevel? 0.01:0;
        Thread::wait(1000); // every one second
    }
}
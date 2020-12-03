

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

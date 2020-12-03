/**********************************************
 * NeoMatrix.cpp
 *
 * Taylor Powell
 * March 2015
 *
 *  Controls an Adafruit Neopixel NeoMatrix 8x8
 *  Because of the global nature of the IO register and bitmask variables, only one contiguous chain of NeoMatrix Arrays can be connected
 *  A large number of NeoMatrix arrays can be chained together by tying Din to Dout of sucessive arrays, but an external power sourece may be required
 *
 *  This library supports only the NXP LPC1768
 */

#include "mbed.h"
#include "NeoMatrix.h"
#include "font.h"


// FastIO register address and bitmask for the GPIO pin
// because these are imported in the assembly
uint32_t neo_fio_reg;
uint32_t neo_bitmask;

// function to write to the strip, implemented in ARM assembly
extern "C" void neo_out(NeoColor*, int);

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
     
     #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
     


NeoArr::NeoArr(PinName pin, int N) : N(N)
{
    bright = 0.5;
    Nbytes = N * 64 * 3;
    arr = (NeoColor*)malloc(N * 64 * sizeof(NeoColor));
    if (arr == NULL)
    {
        printf("NeoArr: ERROR unable to malloc pixel array data");
        N = 0;
    }
    
    gpio_init_out(&gpio, pin);              // initialize GPIO registers
    neo_fio_reg = (uint32_t)gpio.reg_dir;   // set registers and bitmask for
    neo_bitmask = 1 << ((int)pin & 0x1F);   // the assembly to use
}

void NeoArr::setBrightness(float bright)
{
    this->bright = bright;
}


void NeoArr::setPixel(int idx, int x, int y, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    setPixel(idx, x, y, red, green, blue);
}

void NeoArr::setPixel(int idx, int x, int y, uint8_t red, uint8_t green, uint8_t blue)
{
    int pixel = idx*64 + x*8 + y;   // specify pixel based on board index, x, and y values
    // modulate pixel by the total number of pixels
    arr[pixel % (N*64)].red = (uint8_t)(red * bright);
    arr[pixel % (N*64)].green = (uint8_t)(green * bright);
    arr[pixel % (N*64)].blue = (uint8_t)(blue * bright);
}

void NeoArr::drawLine(int idx, int x1, int y1, int x2, int y2, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    drawLine(idx, x1, y1, x2,y2, red, green, blue);
}

void NeoArr::drawLine(int idx, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue)
{
    float k;
    int j = rint(sqrt(pow((x1-x2),2.0) + pow((y1-y2),2.0)));    // calculates magnitude of line
    if(x1 != x2)  // handle infinite case
        k = atan2( (float)(y2-y1),(float) ( x2-x1));    // calculates angle of line
    else
        k = acos(0.0);
        
    for(float n=0; n<=j; n++)   // set a number pixels equal to the magnitude of the line along the closest (rounded) line
        if((x1+ rint(n*cos(k))) >=0 && (x1+rint( n*cos(k))) <=7 &&  (y1+rint(n*sin(k)))>=0 &&  (y1+rint(n*sin(k)))<=7)
            setPixel(idx, x1+ rint(n*cos(k)), y1+ rint(n*sin(k)), red, green, blue);
        
    
}

void NeoArr::drawRect(int idx, int x1, int y1, int x2, int y2, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    drawRect(idx, x1, y1, x2,y2, red, green, blue);
}

void NeoArr::drawRect(int idx, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue)
{
    // note: drawRect does not use drawLine function because the angles will always be 90 degrees and so for these for loops are faster
    for(int i=0; i<=(abs(x2-x1)); i++){ // draws horizontal lines
        if ((max(x1,x2)-i) >= 0 && max(x1, x2) -i <=7){
            if(max(y1,y2) <= 7)
                setPixel(idx, max(x1,x2)-i, max(y1,y2), red, green, blue);
            
            if(min(y1,y2) >= 0)
                setPixel(idx, max(x1,x2)-i, min(y1,y2), red, green, blue);
            }
        }
        
    for(int i=0; i<=(abs(y2-y1)); i++){ // draws verticle lines
        if ((max(y1,y2)-i) >= 0 && max(y1, y2) -i <=7){
            if(max(x1,x2) <= 7)
                setPixel(idx, max(x1,x2), max(y1,y2)-i, red, green, blue);
            
            if(min(x1,x2) >= 0)
                setPixel(idx, min(x1,x2), max(y1,y2)-i, red, green, blue);
            }
        }
}


void NeoArr::drawFilledRect(int idx, int x1, int y1, int x2, int y2, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    drawFilledRect(idx, x1, y1, x2,y2, red, green, blue);
}

void NeoArr::drawFilledRect(int idx, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue)
{
    for(int i=0; i<=(abs(x2-x1)); i++){
        if ((max(x1,x2)-i) >= 0 && max(x1, x2) -i <=7){
            for(int n=0; n<=(abs(y2-y1)); n++){
                if((max(y1,y2)-n) >= 0 && max(y1, y2)-n <=7){
                    setPixel(idx, max(x1,x2)-i, max(y1,y2)-n, red, green, blue);
                }
            }
        }
    }
    
}

void NeoArr::fillScreen(int idx, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    fillScreen(idx, red, green, blue);
}

void NeoArr::fillScreen(int idx,uint8_t red, uint8_t green, uint8_t blue)
{    
   for(int i=0; i<8; i++)
        for(int n=0; n<8; n++)
            setPixel(idx, i, n, red, green, blue);
}


void NeoArr::drawTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    drawTriangle(idx, x1, y1, x2,y2, x3, y3, red, green, blue);
}

void NeoArr::drawTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, uint8_t red, uint8_t green, uint8_t blue)
{
    
    drawLine(idx, x1, y1, x2, y2, red, green, blue);
    drawLine(idx, x2, y2, x3, y3, red, green, blue);
    drawLine(idx, x3, y3, x1, y1, red, green, blue);
    
}

void NeoArr::drawFilledTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    drawFilledTriangle(idx, x1, y1, x2,y2, x3, y3, red, green, blue);
}

void NeoArr::drawFilledTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, uint8_t red, uint8_t green, uint8_t blue)
{
    // note: drawFilledTriangle draws two legs of the triangle and then draws lines from their corner to each point on the opposite leg
    drawLine(idx, x1, y1, x2, y2, red, green, blue);
    drawLine(idx, x2, y2, x3, y3, red, green, blue);
    
    
    float k;
    int j = rint(sqrt(pow((x1-x3),2.0) + pow((y1-y3),2.0)));    // magnitude of opposite leg
    if(x1 != x3)
        k = atan2( (float)(y3-y1),(float) ( x3-x1));    // angle of line of opposite leg
    else
        k = acos(0.0);
        
    for(float n=0; n<=j; n++)
        if((x1+ rint(n*cos(k))) >=0 && (x1+rint( n*cos(k))) <=7 &&  (y1+rint(n*sin(k)))>=0 &&  (y1+rint(n*sin(k)))<=7)
            drawLine(idx, x1+ rint(n*cos(k)), y1+ rint(n*sin(k)), x2, y2, red, green, blue);    // draw line from corner to each point on opposite leg
}

void NeoArr::drawChar(int idx, int x, int y, char c, int color)
{    
    int red = (color & 0xFF0000) >> 16;
    int green = (color & 0x00FF00) >> 8;
    int blue = (color & 0x0000FF);
    
    drawChar(idx, x, y, c, red, green, blue);
}

void NeoArr::drawChar(int idx, int x, int y, char c, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t i,j;
    
    c = c & 0x7F;  // mask c to avoid errors
    
    if (c < ' ') {  // convert c into index of font array
        c = 0;
    } else {
        c -= ' ';
    }

    // font is BMplain, a 96x6 array stored in font.h, many free available fonts are online and can be swapped into this font
    const uint8_t* chr = font[c];

    for (j=0; j<6; j++) {   // character width is 6
        for (i=0; i<8; i++) {   // character height is 8
            if (chr[j] & (1<<i)) {  // if there is a pixel in the vertical line, set pixel on board
                if((x+j) <= 7 && (x+j)>=0 && (y+7-i)>=0 && (y+7-i) <=7)
                     setPixel(0, x+j, y+ 7-i, red,green,blue);
            }
        }
    }
}

void NeoArr::showImage(int idx, const int *img)
{
    int r, g, b;
    for (int i = 0; i < 8; i++)
        for(int n=0; n<8;n++){
        r = (img[i] & 0xFF0000) >> 16;
        g = (img[i] & 0x00FF00) >>8;
        b = img[i] & 0x0000FF;
        setPixel(idx,i,n, r, g, b);
    }
}

void NeoArr::clear()
{
    for (int i = 0; i < (N*64); i++)
    {
        arr[i].red = 0;
        arr[i].green = 0;
        arr[i].blue = 0;
    }
}

void NeoArr::write()
{
    __disable_irq();        // disable interrupts
    neo_out(arr, Nbytes); // output to the strip
    __enable_irq();         // enable interrupts
    wait_us(50);            // wait 50us for the reset pulse
}



/**
 * NeoMatrix.h
 *
 * Taylor Powell
 * March 2015
 *
 * Library for the 8x8 grid array of Neopixel LEDs
 *
 */


#ifndef NEOARRAY_H
#define NEOARRAY_H

#ifndef TARGET_LPC1768
#error NeoArray only supports the NXP LPC1768
#endif

// NeoColor struct definition to hold 24 bit
// color data for each pixel, in GRB order
typedef struct _NeoColor
{
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} NeoColor;

/**
 * NeoArr objects manage the buffering and assigning of
 * addressable NeoPixels
 */
class NeoArr
{
    public:

        /**
         * Create a NeoArr object
         *
         * @param pin The mbed data pin name
         * @param N The number of arrays chained together.
         */
        NeoArr(PinName pin, int N);

        /**
         * Sets the brightness of the entire array. All functions using set pixel are affected by this value. 
         * If a higher brightness is set, an external power supply may be necessary 
         *
         * The default brightness is 0.5
         *
         * @param bright The brightness scaled between 0 and 1.0
         */
        void setBrightness(float bright);

        /**
         * Set a single pixel in the array to a specific color.
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x The x co-ordinate of the pixel
         * @param y The y co-ordinate of the pixel
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
        void setPixel(int idx, int x, int y, int color);

        /**
         * Set a single pixel in the array to a specific color with reg, blue, and blue values in seperate arguments
         */
        void setPixel(int idx, int x, int y, uint8_t red, uint8_t green, uint8_t blue);
        
        /**
         * Draws a line of a specific color between any two points
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x1 The first x co-ordinate of the line
         * @param y1 The first y co-ordinate of the line
         * @param x2 The second x co-ordinate of the line
         * @param y2 The second y co-ordinate of the line
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
         
          void drawLine(int idx, int x1, int y1, int x2, int y2, int color);

        /**
         * Draws a line of a specific color between any two points with reg, blue, and blue values in seperate arguments
         */
        void drawLine(int idx, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue);
        /**
         * Draws a rectangle outline of a specific color given two opposite corner points
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x1 The first x co-ordinate of a corner
         * @param y1 The first y co-ordinate of a corner
         * @param x2 The second x co-ordinate of a corner
         * @param y2 The second y co-ordinate of a corner
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
         
        void drawRect(int idx, int x1, int y1, int x2, int y2, int color);

        /**
         * Draws a rectangle outline of a specific color given two opposite corner points with reg, blue, and blue values in seperate arguments
         */
        void drawRect(int idx, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue);
        
        /**
         * Draws a filled rectangle of a specific color given two opposite corner points
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x1 The first x co-ordinate of a corner
         * @param y1 The first y co-ordinate of a corner
         * @param x2 The second x co-ordinate of a corner
         * @param y2 The second y co-ordinate of a corner
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
         
        void drawFilledRect(int idx, int x1, int y1, int x2, int y2, int color);

        /**
         * Draws a filled rectangle of a specific color given two opposite corner points with reg, blue, and blue values in seperate arguments
         */
        void drawFilledRect(int idx, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue);

        /**
         * Fills the entire array screen with one color
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
        void fillScreen(int idx, int color);
        
         /**
         * Fills the entire array screen with one color with reg, blue, and blue values in seperate arguments
         *
         */
        void fillScreen(int idx, uint8_t red,uint8_t green, uint8_t blue);
        
        
        /**
         * Draws a triangle outline of a specific color given three corner points
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x1 The first x co-ordinate of a corner
         * @param y1 The first y co-ordinate of a corner
         * @param x2 The second x co-ordinate of a corner
         * @param y2 The second y co-ordinate of a corner
         * @param x3 The third x co-ordinate of a corner
         * @param y3 The third y co-ordinate of a corner
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
         void drawTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, int color);

        /**
         * Draws a triangle outline of a specific color given three corner points with reg, blue, and blue values in seperate arguments
         */
        void drawTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, uint8_t red, uint8_t green, uint8_t blue);
         /**
         * Draws a filled triangle of a specific color given three corner points
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x1 The first x co-ordinate of a corner
         * @param y1 The first y co-ordinate of a corner
         * @param x2 The second x co-ordinate of a corner
         * @param y2 The second y co-ordinate of a corner
         * @param x3 The third x co-ordinate of a corner
         * @param y3 The third y co-ordinate of a corner
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
        
        void drawFilledTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, int color);

         /**
         * Draws a filled triangle of a specific color given three corner points with reg, blue, and blue values in seperate arguments
         */
        void drawFilledTriangle(int idx, int x1, int y1, int x2, int y2, int x3, int y3, uint8_t red, uint8_t green, uint8_t blue);
        
        /**
         * Draws a single 6x8 character on the array. The character will be pulled from font.h
         *
         * @param idx The index of the array to write on. Indexing starts at 0 (use 0 if there is only one array)
         * @param x The x co-ordinate of the lower left point of the character
         * @param y The y co-ordinate of the lower left point of the character
         * @para c The character to be drawn
         * @param color Integer golding a 24 bit color using RGB hex indexing (e.g. 0xff0000 is red)
         */
        void drawChar(int idx, int x, int y, char c, int color);

        /**
         * Draws a single 6x8 character on the array. The character will be pulled from font.h with reg, blue, and blue values in seperate arguments
         */
        void drawChar(int idx, int x, int y, char c, uint8_t red, uint8_t green, uint8_t blue);

      
        /**
         * Displays a 64 bit image on board idx
         *
         * @param idx The index of the board
         * @param colors An array of length 64 containing the image to be displayed
         */
        void showImage(int idx, const int *colors);

        /**
         * Clears all pixels on all boards (sets them all to 0)
         */
        void clear();

        /**
         * Write the colors out to the strip; this method must be called
         * to see any hardware effect.
         *
         * This function disables interrupts while the strip data is being sent,
         * each pixel takes approximately 30us to send, plus a 50us reset pulse
         * at the end.
         */
        void write();


    protected:
        NeoColor *arr;    // pixel data buffer modified by setPixel() and used by neo_out()
        int N;              // the number of pixels in the strip
        int Nbytes;         // the number of bytes of pixel data (always N*3)
        float bright;       // the master strip brightness
        gpio_t gpio;        // gpio struct for initialization and getting register addresses
};

#endif



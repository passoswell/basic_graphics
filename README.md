# Basic Graphics
> A modified version of Adafruit GFX library for Arduino into a code 100% C compatible.

## Table of contents
* [General info](#general-info)
* [Screenshots](#screenshots)
* [Technologies](#technologies)
* [Setup](#setup)
* [Features](#features)
* [Status](#status)
* [Inspiration](#inspiration)
* [Contact](#contact)

## General info
A modified version of Adafruit GFX library for Arduino into a code 100% C compatible. The goal with this project is to have basic graphical functions to work with displays in C. 

## Screenshots
No screenshots available.

## Technologies
* Tech 1 - version 1.0
* Tech 2 - version 2.0
* Tech 3 - version 3.0

## Setup
This project is easy to use. Clone the repository, add "basic_graphics.h" into your project, create a display handle of the type "BGFX_Parameters_t", fill parameters such as HEIGHT and WIDTH, create a buffer big enouth to store  all the pixels of the area you want to draw, and call the functions you need from "basic_graphics.h" to write in the buffer.

The "BGFX_Parameters_t" data type contains important imformation about the display that must be supplied to the library. The fields are described in the following lines.

* WIDTH - this is the true display width, should be set once and never changed
* HEIGHT - this is the true display height, should be set once and never changed
* Width - display width as modified by current rotation, should not be set by hand
* Height - display height as modified by current rotation, should not be set by hand
* WidthPixelsOnWrite - Tells the lib the number of horizontal pixels in a byte (1 or 8), monochromatic displays only, should be set once and never changed
* HeightPixelsOnWrite - Tells the lib the number of vertical pixels in a byte (1 or 8), monochromatic displays only, should be set once and never changed
* ColorScheme - Tells to the lib how to work with the colors, should be one item from "BGFX_ColorScheme_t" list, should be set once and never changed
* Rotation - Set display rotation (0 thru 3), could be associated with an accelerometer to read the device orientation (similar to smartphones), should be set by calling "BGFX_SetRotation"
* Cp437 - Not sure about what it does to the font, should be set to NULL in this lib
* GfxFont - Pointer to an external, user supplied font, not implemented, should be set to NULL
* Buffer - A pointer to a buffer where the lib can store the results of its operatons. Its size depends on the desired draw area, and its data type depends on the ColorScheme option

ColorScheme field can be one of the following list. Please note that not all color schemes have been tested, or implemented.

* BGFX_MONOCHROMATIC - Pixels are only zeros and ones, on and off, packaged in groups of eight inside a byte. A byte might represent 8 vertical pixels and one to the horizontal, or 8 horizontal pixels and one to the vertical. This configuration is set in the library by WidthPixelsOnWrite and HeightPixelsOnWrite fields (could reduce fuse then int one field, since they cannot assume the same value, if one is 1, the other is 8). This option should be used with display controllers such as SSD1306, which drives monocromatic displays.
* BGFX_8BITS - A pixel color is represented by one byte (uint8_t).
* BGFX_16BITS - A pixel color is represented by two bytes (uint16_t). This option should be used with display controllers such as ILI9341, which drives LCD displays ans has support to 16 bits mode.
* Other options are not implemented.

If using BGFX_MONOCHROMATIC or BGFX_8BITS, Buffer must be an array of bytes (uint8_t). If using BGFX_16BITS, Buffer mustbe an array of half-words (uint16_t).




## Code Examples
All the functions and usable structures have their comments on "basic_graphics.h" file, and an example code follows. The display used for the example was an SSD1306. It was configured to use horizontal addressing mode, which means the display will increment the column  counter at every data byte received, increment the page counter when the column counter reaches its maximum value (128), and return to page 0, column zero, when the last column of last page is written. Independently of the configuration, it is assumed there is a function implemented to write all the pixels from a buffer to the display at once. For the SSD1306, a monochromatic display which is 128 pixels in width and 8 pages in hight (64 pixels divided into 8 pages), the buffer must be 128 * 8 = 8192 bytes long.

```
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "basic_graphics.h"
/* #include "your_display.h" */


/* Prototype to a possible function to write the pixels to the display */
void DISP_WritePixels(uint8_t *Buffer, uint16_t Size)
{
  /* Your display code here */
}


void Delay(void)
{
  /* Change for your system's delay */
  for(int i = 0; i < 5000000; i++);
}


void main(void)
{
  static uint8_t buffer[128 * 8]; /* Pixels buffer */
  static uint32_t buffer_size = sizeof(buffer)/sizeof(uint16_t);
  static BGFX_Parameters_t BGFX_1;
  uint16_t main_color = 1; //Turns on a pixel
  uint16_t background_color = 0; //Turns off a pixel
  uint32_t counter = 0;

  BGFX_1.HEIGHT      = 64;
  BGFX_1.WIDTH       = 128;
  BGFX_1.WidthPixelsOnWrite  = 1;
  BGFX_1.HeightPixelsOnWrite = 8; /* Number of pixels in a page */
  BGFX_1.Rotation = 0; /* No software rotation intended */
  BGFX_1.ColorScheme = BGFX_MONOCHROMATIC;
  BGFX_1.Buffer      = (void*)buffer;
  BGFX_1.DrawPixel   = NULL;
  BGFX_1.GfxFont     = NULL;

  /* Setting rotation to zero */
  BGFX_SetRotation(0, &BGFX_1);


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = background_color;}

  /* Drawing a few lines to the buffer */
  BGFX_DrawLine(1, 1, 1, 62, main_color, BGFX_1);     /* Top    */
  BGFX_DrawLine(1, 62, 126, 62, main_color, BGFX_1);  /* Right  */
  BGFX_DrawLine(126, 62, 126, 1, main_color, BGFX_1); /* Bottom */
  BGFX_DrawLine(126, 1, 1, 1, main_color, BGFX_1);    /* Left   */

  BGFX_DrawLine(0, 0, 127, 63, main_color, BGFX_1);   /* 1st diagonal */
  BGFX_DrawLine(0, 63, 127, 0, main_color, BGFX_1);   /* 2nd diagonal */

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Cleaning the buffer, a different background color */
  for(int i=0; i<buffer_size; i++) {buffer[i] = main_color;}

  /* Drawing a few lines to the buffer with different color */
  BGFX_DrawLine(1, 1, 1, 62, background_color, BGFX_1);     /* Top    */
  BGFX_DrawLine(1, 62, 126, 62, background_color, BGFX_1);  /* Right  */
  BGFX_DrawLine(126, 62, 126, 1, background_color, BGFX_1); /* Bottom */
  BGFX_DrawLine(126, 1, 1, 1, background_color, BGFX_1);    /* Left   */

  BGFX_DrawLine(0, 0, 127, 63, background_color, BGFX_1);   /* 1st diagonal */
  BGFX_DrawLine(0, 63, 127, 0, background_color, BGFX_1);   /* 2nd diagonal */

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<buffer_size; i++) {buffer[i] = background_color;}

  /* Drawing rectangles */
  BGFX_DrawRect(25, 0, 10, 64, main_color, BGFX_1);
  BGFX_DrawRectFill(51, 0, 10, 64, main_color, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<buffer_size; i++) {buffer[i] = background_color;}

  /* Drawing round rectangles */
  BGFX_DrawRoundRect(0, 0, 50, 64, 10, main_color, BGFX_1);
  BGFX_DrawRoundRectFill(128-50, 0, 50, 64, 10, main_color, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<buffer_size; i++) {buffer[i] = background_color;}

  /* Drawing triangles */
  BGFX_DrawTriangle(25, 0, 50, 63, 0, 63, main_color, BGFX_1);
  BGFX_DrawTriangleFill(127-25, 0, 127, 63, 127-50, 63, main_color, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<buffer_size; i++) {buffer[i] = background_color;}

  /* Drawing circles */
  BGFX_DrawCircleFill(60, 32, 30, main_color, BGFX_1);
  BGFX_DrawCircle(30, 32, 10, main_color, BGFX_1);
  BGFX_DrawCircle(90, 32, 10, main_color, BGFX_1);
  BGFX_DrawCircle(10, 10, 10, main_color, BGFX_1);
  BGFX_DrawCircle(60, 10, 10, main_color, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<buffer_size; i++) {buffer[i] = background_color;}

  /* Hello world message */
  BGFX_SendString(0, 0, (uint8_t*)"Hello world", 11, main_color, background_color, 1, 1, BGFX_1);
  /* A little bigger this time */
  BGFX_SendString(0, 25, (uint8_t*)"Hello world", 11, main_color, background_color, 2, 2, BGFX_1);
  /* Different color and background */
  BGFX_SendString(0, 50, (uint8_t*)"Hello world", 11, background_color, main_color, 1, 1, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, buffer_size);

  Delay();


  /* Plotting a sine wave */
  while(1)
  {
    /* Cleaning the space where the plot will be positioned */
    BGFX_DrawRectFill(0, 25, 128, 25, main_color, BGFX_1);
    for(uint32_t i = 0; i < 128; i++){
      float auxf = sinf((float)(i + counter)/5.0);
      auxf *= 12;
      auxf += 12;
      auxf += 25;
      uint16_t aux16 = auxf;
      BGFX_DrawPixel(i, aux16, background_color, BGFX_1);
    }
    counter += 2;
    DISP_WritePixels(buffer, buffer_size);
  }

}

```

## Features
List of features ready and limitations
* Draw pixels, lines, rectangles, circles and triangles to a buffer with specified color
* Fill the geometric figures listed above with an specified color
* Draw text with an default font and multiples of its size (5 x 7)
* Screen rotation


## Status
Project is: _no longer continue_. Those ara about all the features needed for now, might add things later, but not for sure.

## Inspiration
This repository is derived from [Adafruit's GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) for Arduino. More information on the original project [can be found here](https://learn.adafruit.com/adafruit-gfx-graphics-library).

The template for this readme file was created by [@flynerdpl](https://www.flynerd.pl/).

## Contact
Created by [@Adafruit Industries](https://github.com/adafruit).
Adapted by [@passoswell](https://github.com/passoswell).

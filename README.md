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
A modified version of Adafruit GFX library for Arduino into a code 100% C compatible. The goal with this project is to have basic graphical functions to work with displays. 

## Screenshots
No screenshots available.

## Technologies
* Tech 1 - version 1.0
* Tech 2 - version 2.0
* Tech 3 - version 3.0

## Setup
This project is easy to use. Clone the repository, add "basic_graphics.h" int your project, create a display handle of the type "BGFX_Parameters_t", fill parameters such as HEIGHT and WIDTH, create a buffer (an array of type uint8_t) big enouth to store  all the pixels, and call the functions you need from "basic_graphics.h".

The data type "BGFX_Parameters_t" consists of important imformation about the display that must be supplied to the library. The fields are described in the following lines.

* WIDTH - this is the true display width, should be set once and never changed
* HEIGHT - this is the true display height, should be set once and never changed
* Width - display width as modified by current rotation, should not be set by hand
* Height - display height as modified by current rotation, should not be set by hand
* WidthPixelsOnWrite - Tells the lib the number of horizontal pixels in a byte (1 or 8), monochromatic displays only, should be set once and never changed
* HeightPixelsOnWrite - Tells the lib the number of vertical pixels in a byte (1 or 8), monochromatic displays only, should be set once and never changed
* ColorScheme - Tells to the lib how to work with the colors, should be one item from "BGFX_ColorScheme_t" list, should be set once and never changed
* Rotation - Set display rotation (0 thru 3), could work with an accelerometer to read the device orientation (similar to smartphones), should be set by calling "BGFX_SetRotation"
* Cp437 - Not sure about what it does to the font, should be set to NULL
* GfxFont - Pointer to an external, user supplied font, Not implemented, should be set to NULL

ColorScheme field can be one of the following list. Please note that not all color schemes have been tested, or implemented.

* BGFX_MONOCHROMATIC - Pixels are only zeros and ones, on and off, packaged in groups of eight inside a byte. A byte might represent 8 vertical pixels and one to the horizontal, or 8 horizontal pixels and one to the vertical. This configuration is set in the library by WidthPixelsOnWrite and HeightPixelsOnWrite fields (could reduce fuse then int one field, since they cannot assume the same value, if one is 1, the other is 8)
* BGFX_8BITS - A byte represents a color
* Other options are not implemented


## Code Examples
All the functions and usable structures have their comments on "basic_graphics.h" file, and an example code follows. THe display used was SSD1306. It was configured to use horizontal addressing mode, which means the display will increment column  counter at every data byte received, increment the page counter when the column counter reaches its maximum value (128), and return to page 0, column zero, when the last column of last page is written. Independently of the configuration, it is assumed to have a function somewhere to write all the pixels from a buffer to the display at once. For the SSD1306, a monochromatic display which is 128 pixels in width and 8 pages in hight (64 pixels divided into 8 pages), the buffer must be 128 * 8 = 8192 bytes long.

```
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "basic_graphics.h"
#include "your_display.h"


/* Prototipe to a possible function to write the pixels to the display */
void DISP_WritePixels(uint8_t *Buffer, uint16_t Size);


void Delay(void){
  /* Change for your system's delay */
  for(int i = 0; i < 5000000; i++);
}


void test_bgfx(void)
{
  static uint8_t buffer[128 * 8]; /* Pixels buffer */
  uint32_t counter = 0;
  static BGFX_Parameters_t BGFX_1;

  BGFX_1.HEIGHT      = 64;
  BGFX_1.WIDTH       = 128;
  BGFX_1.WidthPixelsOnWrite  = 1;
  BGFX_1.HeightPixelsOnWrite = 8; /* Number of pixels in a page */
  BGFX_1.Rotation = 0; /* No software rotation intended */
  BGFX_1.ColorScheme = BGFX_MONOCHROMATIC;
  BGFX_1.Buffer      = buffer;
  BGFX_1.DrawPixel   = NULL;
  BGFX_1.GfxFont     = NULL;

  /* Setting rotation to zero */
  BGFX_SetRotation(0, &BGFX_1);


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0;}

  /* Drawing a few lines to the buffer */
  BGFX_DrawLine(1, 1, 1, 62, 1, BGFX_1);     /* Top    */
  BGFX_DrawLine(1, 62, 126, 62, 1, BGFX_1);  /* Right  */
  BGFX_DrawLine(126, 62, 126, 1, 1, BGFX_1); /* Bottom */
  BGFX_DrawLine(126, 1, 1, 1, 1, BGFX_1);    /* Left   */

  BGFX_DrawLine(0, 0, 127, 63, 1, BGFX_1);   /* 1st diagonal */
  BGFX_DrawLine(0, 63, 127, 0, 1, BGFX_1);   /* 2nd diagonal */

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Cleaning the buffer, a different background color */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0xFF;}

  /* Drawing a few lines to the buffer with different color */
  BGFX_DrawLine(1, 1, 1, 62, 0, BGFX_1);     /* Top    */
  BGFX_DrawLine(1, 62, 126, 62, 0, BGFX_1);  /* Right  */
  BGFX_DrawLine(126, 62, 126, 1, 0, BGFX_1); /* Bottom */
  BGFX_DrawLine(126, 1, 1, 1, 0, BGFX_1);    /* Left   */

  BGFX_DrawLine(0, 0, 127, 63, 0, BGFX_1);   /* 1st diagonal */
  BGFX_DrawLine(0, 63, 127, 0, 0, BGFX_1);   /* 2nd diagonal */

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0;}

  /* Drawing rectangles */
  BGFX_DrawRect(25, 0, 10, 64, 1, BGFX_1);
  BGFX_DrawRectFill(51, 0, 10, 64, 1, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0;}

  /* Drawing round rectangles */
  BGFX_DrawRoundRect(0, 0, 50, 64, 10, 1, BGFX_1);
  BGFX_DrawRoundRectFill(128-50, 0, 50, 64, 10, 1, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0;}

  /* Drawing triangles */
  BGFX_DrawTriangle(25, 0, 50, 63, 0, 63, 1, BGFX_1);
  BGFX_DrawTriangleFill(127-25, 0, 127, 63, 127-50, 63, 1, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0;}

  /* Drawing circles */
  BGFX_DrawCircleFill(60, 32, 30, 1, BGFX_1);
  BGFX_DrawCircle(30, 32, 10, 1, BGFX_1);
  BGFX_DrawCircle(90, 32, 10, 1, BGFX_1);
  BGFX_DrawCircle(10, 10, 10, 1, BGFX_1);
  BGFX_DrawCircle(60, 10, 10, 1, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Cleaning the buffer */
  for(int i=0; i<sizeof(buffer); i++) {buffer[i] = 0;}

  /* Hello world message */
  BGFX_SendString(0, 0, (uint8_t*)"Hello world", 11, 1, 0, 1, 1, BGFX_1);
  /* A litthe bigger this time */
  BGFX_SendString(0, 25, (uint8_t*)"Hello world", 11, 1, 0, 2, 2, BGFX_1);
  /* Different color and background */
  BGFX_SendString(0, 50, (uint8_t*)"Hello world", 11, 0, 1, 1, 1, BGFX_1);

  /* Updating the display */
  DISP_WritePixels(buffer, sizeof(buffer));

  Delay();


  /* Ploting a sine wave */
  while(1)
  {
    /* Cleaning the space where the plot will be positioned */
    BGFX_DrawRectFill(0, 25, 128, 25, 0, BGFX_1);
    for(uint32_t i = 0; i < 128; i++){
      float auxf = sinf((float)(i + counter)/5.0);
      auxf *= 12;
      auxf += 12;
      auxf += 25;
      uint16_t aux16 = auxf;
      BGFX_DrawPixel(i, aux16, 1, BGFX_1);
    }
    counter += 2;
    DISP_WritePixels(buffer, sizeof(buffer));
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

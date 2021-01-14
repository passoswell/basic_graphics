/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stddef.h>
#include "basic_graphics.h"

#ifndef min
#define min(a, b)                                      (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  do                                                                           \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }while(0);
#endif





void BGFX_DrawPixel_01(uint16_t x, uint16_t y, uint16_t color ,
    BGFX_Parameters_t Display);

void BGFX_DrawPixel_8(uint16_t x, uint16_t y, uint16_t color,
    BGFX_Parameters_t Display);

void BGFX_DrawPixel_16(uint16_t x, uint16_t y, uint16_t color,
        BGFX_Parameters_t Display);

void BGFX_DrawCircleFillHelper(uint16_t x0, uint16_t y0, uint16_t r,
    uint8_t corners, int16_t delta, uint16_t color, BGFX_Parameters_t Display);


/**************************************************************************/
/*!
    @brief  Draw a pixel to the canvas framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawPixel(uint16_t x, uint16_t y, uint16_t color,
    BGFX_Parameters_t Display)
{
  int16_t t;
    if ((x >= Display.Width) || (y >= Display.Height))
    {
      return;
    }

    switch (Display.Rotation) {
    case 1:
      t = x;
      x = Display.WIDTH - 1 - y;
      y = t;
      break;
    case 2:
      x = Display.WIDTH - 1 - x;
      y = Display.HEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = Display.HEIGHT - 1 - t;
      break;
    }

    if (Display.DrawPixel != NULL) {
      Display.DrawPixel(x, y, color);
    }else if(Display.Buffer != NULL){

      switch(Display.ColorScheme){
      case BGFX_MONOCHROMATIC:
        BGFX_DrawPixel_01(x, y, color, Display);
        break;
      case BGFX_8BITS:
        BGFX_DrawPixel_8(x, y, color, Display);
        break;
      default:
        BGFX_DrawPixel_01(x, y, color, Display);
        break;

      }
    }

}




/**************************************************************************/
/*!
    @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
    @param    Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
    uint16_t color, BGFX_Parameters_t Display)
{
  int16_t steep;
  int16_t dx, dy, err, ystep;

  steep = abs(y1 - y0) > abs(x1 - x0);

  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  dx = x1 - x0;
  dy = abs(y1 - y0);

  err = dx / 2;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      BGFX_DrawPixel(y0, x0, color, Display);
    } else {
      BGFX_DrawPixel(x0, y0, color, Display);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}




/**************************************************************************/
/*!
    @brief    Draw a rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to draw with
    @param    Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    uint16_t color, BGFX_Parameters_t Display)
{
  BGFX_DrawLine(x        , y        , x + w - 1, y        , color, Display);
  BGFX_DrawLine(x + w - 1, y        , x + w - 1, y + h - 1, color, Display);
  BGFX_DrawLine(x + w - 1, y + h - 1, x        , y + h - 1, color, Display);
  BGFX_DrawLine(x        , y + h - 1, x        , y        , color, Display);
}






/**************************************************************************/
/*!
    @brief  Fill a rectangle completely with one color. Update in subclasses if
            desired!
    @param  x   Top left corner x coordinate
    @param  y   Top left corner y coordinate
    @param  w   Width in pixels
    @param  h   Height in pixels
    @param  color 16-bit 5-6-5 Color to fill with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawRectFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    uint16_t color, BGFX_Parameters_t Display)
{
  uint16_t i;
  for (i = x; i < x + w; i++) {
    BGFX_DrawLine(i, y, i, y + h - 1, color, Display);
  }
}



/**************************************************************************/
/*!
    @brief  Draw a circle outline
    @param  x0   Center-point x coordinate
    @param  y0   Center-point y coordinate
    @param  r   Radius of circle
    @param  color 16-bit 5-6-5 Color to draw with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color,
    BGFX_Parameters_t Display)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  uint16_t x = 0;
  uint16_t y = r;

  BGFX_DrawPixel(x0, y0 + r, color, Display);
  BGFX_DrawPixel(x0, y0 - r, color, Display);
  BGFX_DrawPixel(x0 + r, y0, color, Display);
  BGFX_DrawPixel(x0 - r, y0, color, Display);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    BGFX_DrawPixel(x0 + x, y0 + y, color,  Display);
    BGFX_DrawPixel(x0 - x, y0 + y, color,  Display);
    BGFX_DrawPixel(x0 + x, y0 - y, color,  Display);
    BGFX_DrawPixel(x0 - x, y0 - y, color,  Display);
    BGFX_DrawPixel(x0 + y, y0 + x, color,  Display);
    BGFX_DrawPixel(x0 - y, y0 + x, color,  Display);
    BGFX_DrawPixel(x0 + y, y0 - x, color,  Display);
    BGFX_DrawPixel(x0 - y, y0 - x, color,  Display);
  }
}



/**************************************************************************/
/*!
    @brief  Draw a circle with filled color
    @param  x0   Center-point x coordinate
    @param  y0   Center-point y coordinate
    @param  r   Radius of circle
    @param  color 16-bit 5-6-5 Color to fill with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawCircleFill(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color,
    BGFX_Parameters_t Display)
{
  BGFX_DrawLine(x0, y0 - r, x0, y0 + r, color, Display);
  BGFX_DrawCircleFillHelper(x0, y0, r, 3, 0, color, Display);

  /* Below code using the principle of BGFX_DrawCircle, circle is different */
  /*
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  uint16_t x = 0;
  uint16_t y = r;

  BGFX_DrawLine(x0, y0 - r, x0, y0 + r, color, Display);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    BGFX_DrawLine(x0 + x, y0 - y, x0 + x, y0 + y, color, Display);
    BGFX_DrawLine(x0 - x, y0 - y, x0 - x, y0 + y, color, Display);
    BGFX_DrawLine(x0 + y, y0 - x, x0 + y, y0 + x, color, Display);
    BGFX_DrawLine(x0 - y, y0 - x, x0 - y, y0 + x, color, Display);
  }
  */
}



/**************************************************************************/
/*!
    @brief  Draw a pixel to a framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color Binary (on or off) color to fill with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawPixel_01(uint16_t x, uint16_t y, uint16_t color,
    BGFX_Parameters_t Display)
{
  uint8_t *ptr;

  if(Display.HeightPixelsOnWrite > 1 && Display.WidthPixelsOnWrite == 1)
  {
    /* There are 8 vertical pixels in one byte */
    _swap_int16_t(x, y);
  }
  else if(Display.HeightPixelsOnWrite == 1 && Display.WidthPixelsOnWrite > 1)
  {
    /* There are 8 horizontal pixels in one byte */
  }

  ptr = &Display.Buffer[(x / 8) * Display.WIDTH + y];

  if (color){
    *ptr |= 1 << (x & 7);
  }else{
    *ptr &= ~(1 << (x & 7));
  }
}


/**************************************************************************/
/*!
    @brief  Draw a pixel to the canvas framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color 8-bit Color to fill with. Only lower byte of uint16_t is used.
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawPixel_8(uint16_t x, uint16_t y, uint16_t color,
    BGFX_Parameters_t Display)
{
  Display.Buffer[x + y * Display.WIDTH] = color & 0xFF;
}


/**************************************************************************/
/*!
    @brief  Draw a pixel to the canvas framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color 16-bit 5-6-5 Color to fill with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawPixel_16(uint16_t x, uint16_t y, uint16_t color,
    BGFX_Parameters_t Display)
{
  Display.Buffer[x + y * Display.WIDTH] = color;
}




/**************************************************************************/
/*!
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawCircleFillHelper(uint16_t x0, uint16_t y0, uint16_t r,
    uint8_t corners, int16_t delta, uint16_t color, BGFX_Parameters_t Display)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  uint16_t x = 0;
  uint16_t y = r;
  uint16_t px = x;
  uint16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1){
        BGFX_DrawLine(x0 + x, y0 - y, x0 + x, y0 + y + delta, color, Display);
        //writeFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      }
      if (corners & 2){
        BGFX_DrawLine(x0 - x, y0 - y, x0 - x, y0 + y + delta, color, Display);
        //writeFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
      }
    }
    if (y != py) {
      if (corners & 1){
        BGFX_DrawLine(x0 + py, y0 - px, x0 + py, y0 + px + delta, color, Display);
        //writeFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      }
      if (corners & 2){
        BGFX_DrawLine(x0 - py, y0 - px, x0 - py, y0 + px + delta, color, Display);
        //writeFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      }
      py = y;
    }
    px = x;
  }
}


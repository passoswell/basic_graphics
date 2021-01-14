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
#include "glcdfont.h"

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

void BGFX_DrawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r,
    uint8_t cornername, uint16_t color, BGFX_Parameters_t Display);

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
    @brief  Draw a rounded rectangle with no fill color
    @param  x   Top left corner x coordinate
    @param  y   Top left corner y coordinate
    @param  w   Width in pixels
    @param  h   Height in pixels
    @param  r   Radius of corner rounding
    @param  color 16-bit 5-6-5 Color to draw with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
    int16_t r, uint16_t color, BGFX_Parameters_t Display)
{
  int16_t max_radius;

  max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axiss
  if (r > max_radius){
    r = max_radius;
  }
  // smarter version
  BGFX_DrawLine(x + r, y        , x - r + w, y        , color, Display); // Top
  BGFX_DrawLine(x + r, y + h - 1, x - r + w, y + h - 1, color, Display); // Bottom
  BGFX_DrawLine(x        , y + r, x        , y + h - r, color, Display); // Left
  BGFX_DrawLine(x + w - 1, y + r, x + w - 1, y + h - r, color, Display); // Right
  // draw four corners
  BGFX_DrawCircleHelper(x + r, y + r, r, 1, color, Display);
  BGFX_DrawCircleHelper(x + w - r - 1, y + r, r, 2, color, Display);
  BGFX_DrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color, Display);
  BGFX_DrawCircleHelper(x + r, y + h - r - 1, r, 8, color, Display);
}



/**************************************************************************/
/*!
    @brief  Draw a rounded rectangle with fill color
    @param  x   Top left corner x coordinate
    @param  y   Top left corner y coordinate
    @param  w   Width in pixels
    @param  h   Height in pixels
    @param  r   Radius of corner rounding
    @param  color 16-bit 5-6-5 Color to draw/fill with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawRoundRectFill(int16_t x, int16_t y, int16_t w, int16_t h,
    int16_t r, uint16_t color, BGFX_Parameters_t Display) {
  int16_t max_radius;

  max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius){
    r = max_radius;
  }
  // smarter version
  BGFX_DrawRectFill(x + r, y, w - 2 * r, h, color, Display);
  // draw four corners
  BGFX_DrawCircleFillHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color,
      Display);
  BGFX_DrawCircleFillHelper(x + r, y + r, r, 2, h - 2 * r - 1, color,
      Display);
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
    @brief  Draw a triangle with no fill color
    @param  x0  Vertex #0 x coordinate
    @param  y0  Vertex #0 y coordinate
    @param  x1  Vertex #1 x coordinate
    @param  y1  Vertex #1 y coordinate
    @param  x2  Vertex #2 x coordinate
    @param  y2  Vertex #2 y coordinate
    @param  color 16-bit 5-6-5 Color to draw with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    int16_t x2, int16_t y2, uint16_t color, BGFX_Parameters_t Display) {
  BGFX_DrawLine(x0, y0, x1, y1, color, Display);
  BGFX_DrawLine(x1, y1, x2, y2, color, Display);
  BGFX_DrawLine(x2, y2, x0, y0, color, Display);
}





/**************************************************************************/
/*!
   @brief     Draw a triangle with color-fill
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawTriangleFill(uint16_t x0, uint16_t y0, uint16_t x1,
    uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color,
    BGFX_Parameters_t Display)
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a){
      a = x1;
    }else if (x1 > b){
      b = x1;
    }
    if (x2 < a){
      a = x2;
    }else if (x2 > b){
      b = x2;
    }
    BGFX_DrawLine(a, y0, a + b - a + 1, y0,color, Display);
    //writeFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2){
    last = y1; // Include y1 scanline
  }else{
    last = y1 - 1; // Skip it
  }

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b){
      _swap_int16_t(a, b);
    }
    BGFX_DrawLine(a, y, a + b - a + 1, y,color, Display);
    //writeFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b){
      _swap_int16_t(a, b);
    }
    BGFX_DrawLine(a, y, a + b - a + 1, y,color, Display);
    //writeFastHLine(a, y, b - a + 1, color);
  }
}





// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void BGFX_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color,
    uint16_t bg, uint8_t size_x, uint8_t size_y, BGFX_Parameters_t Display)
{

  if (Display.GfxFont == NULL) { // 'Classic' built-in font

    if ((x >= Display.Width) ||              // Clip right
        (y >= Display.Height) ||             // Clip bottom
        ((x + 6 * size_x - 1) < 0) || // Clip left
        ((y + 8 * size_y - 1) < 0))   // Clip top
    {
      return;
    }

    if (!Display.Cp437 && (c >= 176)){
      c++; // Handle 'classic' charset behavior
    }


    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = glcdfont[c * 5 + i];
      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1){
            BGFX_DrawPixel(x + i, y + j, color, Display);
          }else{
            BGFX_DrawRectFill(x + i * size_x, y + j * size_y, size_x, size_y,
                          color, Display);
          }
        } else if (bg != color) {
          if (size_x == 1 && size_y == 1){
            BGFX_DrawPixel(x + i, y + j, bg, Display);
          }else{
            BGFX_DrawRectFill(x + i * size_x, y + j * size_y, size_x, size_y,
                bg, Display);
          }
        }
      }
    }
    if (bg != color) { // If opaque, draw vertical line for last column
      if (size_x == 1 && size_y == 1){
        BGFX_DrawLine(x + 5, y, x + 5, y + 8, bg, Display);
        //writeFastVLine(x + 5, y, 8, bg);
      }else{
        BGFX_DrawRectFill(x + 5 * size_x, y, size_x, 8 * size_y, bg, Display);
      }
    }

  }
//  else
//  { // Custom font
//
//    // Character is assumed previously filtered by write() to eliminate
//    // newlines, returns, non-printable characters, etc.  Calling
//    // drawChar() directly with 'bad' characters of font may cause mayhem!
//
//    c -= (uint8_t)pgm_read_byte(&gfxFont->first);
//    GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
//    uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);
//
//    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
//    uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height);
//    int8_t xo = pgm_read_byte(&glyph->xOffset),
//           yo = pgm_read_byte(&glyph->yOffset);
//    uint8_t xx, yy, bits = 0, bit = 0;
//    int16_t xo16 = 0, yo16 = 0;
//
//    if (size_x > 1 || size_y > 1) {
//      xo16 = xo;
//      yo16 = yo;
//    }
//
//    // Todo: Add character clipping here
//
//    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
//    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
//    // has typically been used with the 'classic' font to overwrite old
//    // screen contents with new data.  This ONLY works because the
//    // characters are a uniform size; it's not a sensible thing to do with
//    // proportionally-spaced fonts with glyphs of varying sizes (and that
//    // may overlap).  To replace previously-drawn text when using a custom
//    // font, use the getTextBounds() function to determine the smallest
//    // rectangle encompassing a string, erase the area with fillRect(),
//    // then draw new text.  This WILL infortunately 'blink' the text, but
//    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
//    // only creates a new set of problems.  Have an idea to work around
//    // this (a canvas object type for MCUs that can afford the RAM and
//    // displays supporting setAddrWindow() and pushColors()), but haven't
//    // implemented this yet.
//
//    startWrite();
//    for (yy = 0; yy < h; yy++) {
//      for (xx = 0; xx < w; xx++) {
//        if (!(bit++ & 7)) {
//          bits = pgm_read_byte(&bitmap[bo++]);
//        }
//        if (bits & 0x80) {
//          if (size_x == 1 && size_y == 1) {
//            writePixel(x + xo + xx, y + yo + yy, color);
//          } else {
//            writeFillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
//                          size_x, size_y, color);
//          }
//        }
//        bits <<= 1;
//      }
//    }
//    endWrite();
//
//  } // End classic vs custom font
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
    @brief  Quarter-circle drawer, used to do circles and roundrects
    @param  x0   Center-point x coordinate
    @param  y0   Center-point y coordinate
    @param  r   Radius of circle
    @param  cornername  Mask bit #1 or bit #2 to indicate which quarters of
            the circle we're doing
    @param  color 16-bit 5-6-5 Color to draw with
    @param  Display Structure to display parameters and functions
*/
/**************************************************************************/
void BGFX_DrawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r,
    uint8_t cornername, uint16_t color, BGFX_Parameters_t Display) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  uint16_t x = 0;
  uint16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      BGFX_DrawPixel(x0 + x, y0 + y, color, Display);
      BGFX_DrawPixel(x0 + y, y0 + x, color, Display);
    }
    if (cornername & 0x2) {
      BGFX_DrawPixel(x0 + x, y0 - y, color, Display);
      BGFX_DrawPixel(x0 + y, y0 - x, color, Display);
    }
    if (cornername & 0x8) {
      BGFX_DrawPixel(x0 - y, y0 + x, color, Display);
      BGFX_DrawPixel(x0 - x, y0 + y, color, Display);
    }
    if (cornername & 0x1) {
      BGFX_DrawPixel(x0 - y, y0 - x, color, Display);
      BGFX_DrawPixel(x0 - x, y0 - y, color, Display);
    }
  }
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


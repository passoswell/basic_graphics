#ifndef BASIC_GRAPHICS_H
#define BASIC_GRAPHICS_H


#include <stdint.h>
#include "gfxfont.h"


typedef enum
{
  BGFX_MONOCHROMATIC = 0,
  BGFX_8BITS,
  BGFX_16BITS,
  BGFX_24BITS,
  BGFX_RGB,
}BGFX_ColorScheme_t;



typedef struct
{
  uint16_t WIDTH;       /*!< This is the 'raw' display width - never changes */
  uint16_t HEIGHT;      /*!< This is the 'raw' display height - never changes */
  uint16_t Width;       /*!< Display width as modified by current rotation */
  uint16_t Height;      /*!< Display height as modified by current rotation */
  uint16_t WidthPixelsOnWrite; /*!< Number of horizontal pixels in a byte */
  uint16_t HeightPixelsOnWrite; /*!< Number of vertical pixels in a byte */
  BGFX_ColorScheme_t ColorScheme; /*!< Color configuration on display */
  int16_t CursorX;      /*!< x location to start print()ing text */
  int16_t CursorY;      /*!< y location to start print()ing text */
  uint16_t TextColor;   /*!< 16-bit background color for print() */
  uint16_t TextbgColor; /*!< 16-bit text color for print() */
  uint8_t TextSizeX;    /*!< Desired magnification in X-axis of text to print() */
  uint8_t TextSizeY;    /*!< Desired magnification in Y-axis of text to print() */
  uint8_t Rotation;     /*!< Display rotation (0 thru 3) */
  uint8_t Wrap;         /*!< If set, 'wrap' text at right edge of display */
  uint8_t Cp437;        /*!< If set, use correct CP437 charset (default is off) */
  BGFXfont *GfxFont;     /*!< Pointer to special font */

  uint8_t *Buffer;
  void (*DrawPixel)(uint16_t x, uint16_t y, uint16_t color); /*!< Writes pixel
                                                                 to a buffer */

}BGFX_Parameters_t;


void BGFX_DrawPixel(uint16_t x, uint16_t y, uint16_t color, BGFX_Parameters_t Display);


void BGFX_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
    uint16_t color, BGFX_Parameters_t Display);

void BGFX_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    uint16_t color, BGFX_Parameters_t Display);

void BGFX_DrawRectFill(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    uint16_t color, BGFX_Parameters_t Display);

void BGFX_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
    int16_t r, uint16_t color, BGFX_Parameters_t Display);

void BGFX_DrawRoundRectFill(int16_t x, int16_t y, int16_t w, int16_t h,
    int16_t r, uint16_t color, BGFX_Parameters_t Display);

void BGFX_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color,
    BGFX_Parameters_t Display);

void BGFX_DrawCircleFill(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color,
    BGFX_Parameters_t Display);

void BGFX_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    int16_t x2, int16_t y2, uint16_t color, BGFX_Parameters_t Display);

void BGFX_DrawTriangleFill(uint16_t x0, uint16_t y0, uint16_t x1,
    uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color,
    BGFX_Parameters_t Display);

void BGFX_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color,
    uint16_t bg, uint8_t size_x, uint8_t size_y, BGFX_Parameters_t Display);


#endif /* BASIC_GRAPHICS_H */

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
  /* Writing in an area with size 128 x 64, equivalent to SSD1306 */
  static uint16_t buffer[64*128]; /* Pixels buffer */
  static uint32_t buffer_size = sizeof(buffer)/sizeof(uint16_t);
  static BGFX_Parameters_t BGFX_1;
  uint16_t main_color = 0x0000; //Black
  uint16_t background_color = 0xFFFF; //White
  uint32_t counter = 0;

  BGFX_1.HEIGHT      = 64;
  BGFX_1.WIDTH       = 128;
  BGFX_1.WidthPixelsOnWrite  = 1;
  BGFX_1.HeightPixelsOnWrite = 1; /* Number of pixels in a page */
  BGFX_1.Rotation = 0; /* No software rotation intended */
  BGFX_1.ColorScheme = BGFX_16BITS;
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

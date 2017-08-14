#ifndef	_LCD_H_
#define _LCD_H_

#define RESET  PTC10_OUT
#define DC     PTC11_OUT
#define D1     PTC8_OUT
#define D0     PTC9_OUT
#define CS     PTC12_OUT
 

 extern uint8_t beyond96x64[512];
 void LCD_WrDat(uint8 data);
 void LCD_Set_Pos(uint8_t x, uint8_t y);
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void LCD_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void LCD_Print(uint8_t x, uint8_t y, uint8_t ch[]);
 void LCD_PutPixel(uint8_t x,uint8_t y);
 void LCD_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
 void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]); 
 void LCD_Fill(uint8_t dat);
 void Display(int16 i,uint8 x,uint8 y);
 void DisplayFloat(int16 i,uint8 x,uint8 y);
#endif
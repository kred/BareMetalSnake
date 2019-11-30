#ifndef _LCD_H
#define _LCD_H

void lcd_init();
void lcd_brightness(unsigned char br);

void lcd_update();
void lcd_pixel(unsigned int x, unsigned int y, unsigned char c, unsigned int dirty);
unsigned int lcd_get_pixel(unsigned int x, unsigned int y);
void lcd_putc(unsigned int x, unsigned int y, unsigned char c, unsigned int dirty);

void lcd_clear();
void lcd_draw_screen(const unsigned char* screen);

#endif
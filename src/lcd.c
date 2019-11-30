#include "lcd.h"
#include "hw.h"
#include "font.h"
#include "tasks.h"

/*
    LCD    RPi1       Function
    1      GPIO11     SCL
    2      GPIO10     MOSI
    3      3V3        3.3V supply
    4      GPIO9      A0
    5      GPIO7      RESET
    6      GPIO8      CS
    7      GND        GND
    8      -
    9      -
    10     GND        GND
    11     3V3        3.3V backlight supply
    12     -
*/

unsigned char _lcd_buffer[128 * 8]; // framebuffer
unsigned int _dirty; // dirty flag

static void lcd_command(unsigned char c)
{
    gpio_set(9, 0);
    delay_ms(1);
    spi_write(c);
}

static inline void lcd_data(unsigned char c)
{
    spi_write(c);
}

static void lcd_reset()
{
    gpio_set(7, 1);
    delay_ms(1);
    gpio_set(7, 0);
    delay_ms(10);
    gpio_set(7, 1);
    delay_ms(50);
}

void lcd_init()
{
    lcd_reset();

    // initialization sequence
    lcd_command(0xa0);
    lcd_command(0xae);
    lcd_command(0xc0);
    lcd_command(0xa3);
    lcd_command(0x2c);
    delay_ms(10);
    lcd_command(0x2e);
    delay_ms(10);
    lcd_command(0x2f);
    lcd_command(0x24); // Rb/Ra golden ratio :)
    lcd_command(0x81);
    lcd_command(0x15); // readable brightness value
    lcd_command(0xaf);
    lcd_command(0xc8); // reverse direction
    lcd_command(0x40);


    lcd_clear();

    _dirty = 1;

    tasks_set(0, lcd_update, 50); // update screen every 50ms
}

void lcd_brightness(unsigned char br)
{
    const unsigned char val[10] = { 0, 7, 14, 21, 28, 35, 42, 50, 56, 63 };

    lcd_command(0x81);
    lcd_command(val[br]);
}

void lcd_update()
{
    if (!_dirty)
        return;

    for(unsigned int j = 0; j < 8; j++)
    {
        lcd_command(0xB0 | j);
        lcd_command(0x10);
        lcd_command(0x00);

        gpio_set(9, 1);
        for(unsigned int i = 0; i < 128; i++)
            lcd_data(_lcd_buffer[j*128 + i]);
    }

    _dirty = 0;
}

void lcd_pixel(unsigned int x, unsigned int y, unsigned char c, unsigned int dirty)
{
    if (c)
        _lcd_buffer[(y >> 3) * 128 + x] |= (1 << (y % 8));
    else
        _lcd_buffer[(y >> 3) * 128 + x] &= ~(1 << (y % 8));
    
    _dirty = dirty;
}

unsigned int lcd_get_pixel(unsigned int x, unsigned int y)
{
    return _lcd_buffer[(y >> 3) * 128 + x] & (1 << (y % 8)) ? 1 : 0;
}

static inline unsigned char lcd_atascii(unsigned char c)
{
	if (c >= 0 && c < 32)
		return c + 64;
	else if (c >= 32 && c < 96)
		return c - 32;
	else if (c >= 96 && c < 128)
		return c;
	else 
        return lcd_atascii(c-128);
}

void lcd_putc(unsigned int x, unsigned int y, unsigned char c, unsigned int dirty)
{
    unsigned int _c = lcd_atascii(c);

    for (unsigned int fy = 0; fy < 8; ++fy)
        for (unsigned int fx = 0; fx < 8; ++fx)
        {
            unsigned int offset = (_c << 3) + fy;
            if (c < 128)
                lcd_pixel(x + fx, y + fy, font_array[offset] & (1 << fx), dirty);
            else
                lcd_pixel(x + fx, y + fy, !(font_array[offset] & (1 << fx)), dirty);
        }
}

void lcd_clear()
{
    for(unsigned int i = 0; i < 128*8; ++i)
        _lcd_buffer[i] = 0x00;
}

void lcd_draw_screen(const unsigned char* screen)
{
    for(unsigned int y = 0; y < 8; y++)
        for(unsigned int x = 0; x < 16; x++)
            lcd_putc(x*8, y*8, screen[y * 16 + x], 0);

    _dirty = 1;
}

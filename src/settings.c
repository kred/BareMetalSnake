#include "settings.h"
#include "hw.h"
#include "lcd.h"
#include "keys.h"

unsigned char brightness = 3;

static void draw_slider(unsigned int x, unsigned int y, unsigned char val)
{
    for(unsigned int i = 0; i < 10; i++)
        lcd_putc(x + i*8, y, ' ', 0);

    lcd_putc(x + val*8, y, '\x14', 1);
}

SCREENS screen_settings()
{
    lcd_draw_screen(settings_screen);

    while(1)
    {
        draw_slider(24, 32, brightness);

        KEYS key = keys_read();
        keys_debounce();
        if (key == KEY_LEFT && brightness > 0)
        {
            brightness--;
        }
        else if (key == KEY_RIGHT && brightness < 9)
        {
            brightness++;
        }
        else if (key == KEY_FIRE)
        {
            return SCREEN_WELCOME;
        }

        lcd_brightness(brightness);

        delay_ms(50);
    }

    return SCREEN_WELCOME;
}

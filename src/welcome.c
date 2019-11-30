#include "welcome.h"
#include "hw.h"
#include "lcd.h"
#include "keys.h"

SCREENS screen_welcome()
{
    unsigned int option = 0;

    lcd_draw_screen(welcome_screen);

    while(1)
    {
        lcd_putc(24, 32, option == 0 ? '\x1f' : ' ', 0);
        lcd_putc(24, 40, option == 1 ? '\x1f' : ' ', 1);

        KEYS key = keys_read();
        keys_debounce();
        if (key == KEY_DOWN && option == 0)
            option = 1;
        else if (key == KEY_UP && option == 1)
            option = 0;
        else if (key == KEY_FIRE)
            return option == 0 ? SCREEN_SNAKE : SCREEN_SETTINGS;

        delay_ms(50);
    }

    return SCREEN_WELCOME;
}

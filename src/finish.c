#include "finish.h"
#include "snake.h"
#include "hw.h"
#include "lcd.h"
#include "keys.h"

SCREENS screen_finish()
{
    lcd_draw_screen(finish_screen);
    
    unsigned char* score = get_score();
    lcd_putc(6*8, 4*8, 128 + score[0], 0);
    lcd_putc(7*8, 4*8, 128 + score[1], 0);
    lcd_putc(8*8, 4*8, 128 + score[2], 1);

    while(1)
    {

        KEYS key = keys_read();
        keys_debounce();
        if (key == KEY_FIRE)
            return SCREEN_WELCOME;

        delay_ms(50);
    }

    return SCREEN_WELCOME;
}
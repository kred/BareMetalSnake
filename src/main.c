#include "hw.h"
#include "keys.h"
#include "lcd.h"
#include "screens.h"
#include "tasks.h"

#include "welcome.h"
#include "settings.h"
#include "snake.h"
#include "finish.h"

void hw_init()
{
    tasks_init();
    gpio_init();
    keys_init();
    timer_init();
    spi_init();

    lcd_init();
}

void main()
{
    hw_init();

    SCREENS screen = SCREEN_WELCOME;

    while(1)
    {
        switch(screen)
        {
            case SCREEN_WELCOME: screen = screen_welcome(); break;
            case SCREEN_SETTINGS: screen = screen_settings(); break;
            case SCREEN_SNAKE: screen = screen_snake(); break;
            case SCREEN_FINISH: screen = screen_finish(); break;
        }
    }

}
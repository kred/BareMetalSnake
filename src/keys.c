#include "registers.h"
#include "hw.h"
#include "keys.h"

#define KEY_PWR_BLOCK GPFSEL0
#define KEY_PWR_BPIN 0
#define KEY_PWR_PIN 0
#define KEY_UP_BLOCK GPFSEL1
#define KEY_UP_BPIN 7
#define KEY_UP_PIN 17
#define KEY_RIGHT_BLOCK GPFSEL2
#define KEY_RIGHT_BPIN 7
#define KEY_RIGHT_PIN 27
#define KEY_DOWN_BLOCK GPFSEL2
#define KEY_DOWN_BPIN 2
#define KEY_DOWN_PIN 22
#define KEY_FIRE_BLOCK GPFSEL1
#define KEY_FIRE_BPIN 5
#define KEY_FIRE_PIN 15
#define KEY_LEFT_BLOCK GPFSEL1
#define KEY_LEFT_BPIN 4
#define KEY_LEFT_PIN 14

void keys_init()
{
    GPIOFSEL(KEY_PWR_BLOCK, KEY_PWR_BPIN, GPFSEL_OUT);
    gpio_set(0, 1);

    GPIOFSEL(KEY_UP_BLOCK, KEY_UP_BPIN, GPFSEL_IN);
    GPIOFSEL(KEY_RIGHT_BLOCK, KEY_RIGHT_BPIN, GPFSEL_IN);
    GPIOFSEL(KEY_DOWN_BLOCK, KEY_DOWN_BPIN, GPFSEL_IN);
    GPIOFSEL(KEY_FIRE_BLOCK, KEY_FIRE_BPIN, GPFSEL_IN);
    GPIOFSEL(KEY_LEFT_BLOCK, KEY_LEFT_BPIN, GPFSEL_IN);

    // enable pull-down
    REG(GPPUD) = 0x01; // enable pull-down control signal
    delay_ms(1); // wait at least 150 cycles
    REG(GPPUDCLK0) = (1 << KEY_UP_PIN) | (1 << KEY_RIGHT_PIN) | (1 << KEY_DOWN_PIN) | (1 << KEY_FIRE_PIN) | (1 << KEY_LEFT_PIN);
    delay_ms(1); // wait at least 150 cycles
    REG(GPPUD) = 0x00; // disable pull-down control signal
    REG(GPPUDCLK0) = 0x0; // remove clock
}

KEYS keys_read()
{
    if (gpio_get(KEY_FIRE_PIN))
        return KEY_FIRE;
    if (gpio_get(KEY_UP_PIN))
        return KEY_UP;
    if (gpio_get(KEY_DOWN_PIN))
        return KEY_DOWN;
    if (gpio_get(KEY_LEFT_PIN))
        return KEY_LEFT;
    if (gpio_get(KEY_RIGHT_PIN))
        return KEY_RIGHT;

    return KEY_NONE;
}

void keys_debounce()
{
    for(unsigned int i = 0; i < 10; i++)
    {
        if (keys_read() == KEY_NONE)
            break;
        delay_ms(20);
    }
}
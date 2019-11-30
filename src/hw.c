#include "hw.h"
#include "registers.h"
#include "tasks.h"

extern void enable_irq();

#define DELAY_MS 1000
volatile unsigned int rnd_seed = 0xa59c1780;

void _irq_handler()
{
    rnd_seed ^= SYSTIMCLO_VAL;
    // trigger timer every 1ms
    if (SYSTIMC1_MATCH)
    {
        for(unsigned int i = 0; i < MAX_TASKS; ++i)
        {
            if (tasks[i].fun)
            {
                if (++tasks[i].current_time == tasks[i].trigger_time)
                {
                    tasks[i].current_time = 0;
                    tasks[i].fun();
                }
            }
        }

        SYSTIMC1_THR(SYSTIMCLO_VAL + DELAY_MS);
        SYSTIMC1_CLEAR; // clear interrupt
    }
}

void _fiq_handler()
{

}

void gpio_init()
{
    GPIOFSEL(GPFSEL1, 6, GPFSEL_OUT); // GPIO16 output - ACT LED
    GPIOCLR0(16);
}

void gpio_set(unsigned char pin, unsigned char val)
{
    if (pin >= 0 && pin < 32)
    {
        if (val)
            GPIOSET0(pin);
        else
            GPIOCLR0(pin);
    }
    else if (pin >= 32 && pin < 53)
    {
        if (val)
            GPIOSET1(pin);
        else
            GPIOCLR1(pin);
    }
}

unsigned char gpio_get(unsigned char pin)
{
    if (pin >= 0 && pin < 32)
    {
        return GPIOGET0(pin) ? 1 : 0;
    }
    else if (pin >= 32 && pin < 53)
    {
        return GPIOGET1(pin) ? 1 : 0;
    }
    
    return 0;
}

void spi_init()
{
    GPIOFSEL(GPFSEL0, 9, GPFSEL_OUT); // GPIO9 output - LCD A0
    GPIOFSEL(GPFSEL0, 7, GPFSEL_OUT); // GPIO7 output - LCD RESET 
    GPIOFSEL(GPFSEL1, 0, GPFSEL_AF0); // GPIO10 alt. function: MOSI
    GPIOFSEL(GPFSEL1, 1, GPFSEL_AF0); // GPIO11 alt. function: CLK
    GPIOFSEL(GPFSEL0, 8, GPFSEL_AF0); // GPIO8  alt. function: CS0

    SPI_SETUP(SPI_CLEAR1 | SPI_CLEAR0); // clear FIFO
    SPI_CLK_DIV(64);                  // SPI CLK divider - 250MHz/64
}

void spi_write(unsigned char c)
{
    while(!SPI_TXD_READY);
    SPI_ENABLE_CS;
    SPI_WRITE(c);
    while(SPI_BUSY);
    SPI_DISABLE_CS;
}

void timer_init()
{
    SYSTIMC1_THR(SYSTIMCLO_VAL + 1000);
    SYSTIMC1_CLEAR;
    IRQ_ENABLE_SYSTIMC1;
    enable_irq();
}

void delay_ms(unsigned int ms)
{
    const volatile unsigned int base_time = SYSTIMCLO_VAL;
    unsigned volatile int expected_time = base_time + ms * 1000;

    while(1)
    {
        volatile unsigned int current_time = SYSTIMCLO_VAL;
        if (current_time >= expected_time)
            break;
    }
}

unsigned char rnd()
{
    unsigned int shift = (SYSTIMCLO_VAL) & 0xf;
    rnd_seed ^= SYSTIMCLO_VAL;
    rnd_seed ^= SYSTIMCLO_VAL;
    rnd_seed >>= shift;
    rnd_seed ^= SYSTIMCLO_VAL;

    return rnd_seed & 0xff;
}
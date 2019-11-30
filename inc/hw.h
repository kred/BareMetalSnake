#ifndef _HW_H
#define _HW_H

void gpio_init();
void gpio_set(unsigned char pin, unsigned char val);
unsigned char gpio_get(unsigned char pin);

void timer_init();

void spi_init();
void spi_write(unsigned char c);

void delay_ms(unsigned int ms);

unsigned char rnd();

#endif
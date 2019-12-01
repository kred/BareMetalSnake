# BareMetalSnake
## Intro
Implementation of fast-paced game for bare-metal RaspberryPi 1 model B. You control player nicknamed 'Snake' whose goal is to collect all hearts placed on the sophisticated playground.

## Requirements
- RaspberyPi Model 1B
- Display based on ST7565P controller connected to SPI bus
- 5 buttons keypad

## Hardware

![Table](docs/pinout1.png)

![Cables](docs/pinout2.png)

![Device](docs/board1.jpg)

![Device](docs/board2.jpg)

![Device](docs/board3.jpg)

## Software
To properly build game, following packages are needed:
- GNU Make
- arm-none-eabi- GNU C for ARM compiler
- mtools

Download or checkout sources, execute:
> make

to compile and build SD card image. You can then use *dd* to write image to the SD card:

> dd if=img/sdcard.img of=/dev/sdX bs=1M

where */dev/sdX* denotes path to the SD card device.

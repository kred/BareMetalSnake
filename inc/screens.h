#ifndef _SCREENS_H
#define _SCREENS_H

extern const unsigned char welcome_screen[16*8];
extern const unsigned char playground_screen[16*8];
extern const unsigned char settings_screen[16*8];
extern const unsigned char finish_screen[16*8];

typedef enum
{
    SCREEN_WELCOME = 0,
    SCREEN_SETTINGS,
    SCREEN_SNAKE,
    SCREEN_FINISH
} SCREENS;

#endif
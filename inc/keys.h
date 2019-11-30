#ifndef _KEYS_H
#define _KEYS_H

typedef enum
{
    KEY_NONE = 0,
    KEY_FIRE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN
} KEYS;


void keys_init();
KEYS keys_read();
void keys_debounce();


#endif
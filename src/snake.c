#include "snake.h"
#include "hw.h"
#include "lcd.h"
#include "keys.h"

#define SNAKE_MAX_LENGTH 128
#define SNAKE_INIT_LENGTH 4
#define APPLE_MAX 5

typedef enum
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
} DIRECTION;

struct SNAKE
{
    unsigned int x;
    unsigned int y;
    struct SNAKE *next;
};

struct APPLE
{
    unsigned int x;
    unsigned int y;
};

struct SNAKE snake[SNAKE_MAX_LENGTH];
struct SNAKE *snake_tail = 0;
struct APPLE apples[APPLE_MAX];

unsigned int snake_length;
unsigned int score;
DIRECTION direction;

static unsigned int find_free()
{
    for (unsigned int i = 0; i < SNAKE_MAX_LENGTH; ++i)
    {
        if (snake[i].x == 0xdeadbeef)
            return i;
    }

    return 0xdeadbeef;
}

static unsigned int is_apple(unsigned int x, unsigned int y)
{
    for (unsigned int i = 0; i < APPLE_MAX; ++i)
    {
        if (apples[i].x != 0xdeadbeef)
        {
            if (x >= apples[i].x && x < (apples[i].x + 8) && y >= apples[i].y && y < (apples[i].y + 8))
                return i + 1;
        }
    }
    
    return 0;
}

static unsigned int is_border(unsigned int x, unsigned int y)
{
    return lcd_get_pixel(x, y);
}

static unsigned int is_playground(unsigned int x, unsigned int y)
{
    return playground_screen[y * 16 + x] != ' ';
}

static void generate_apples()
{
    for (unsigned int i = 0; i < APPLE_MAX; ++i)
    {
        while(1)
        {
            unsigned int rx = 1 + rnd() % 14;
            unsigned int ry = 1 + rnd() % 4;

            if (is_apple(rx * 8, ry * 8))
                continue;

            apples[i].x = rx * 8;
            apples[i].y = ry * 8;

            if (is_playground(rx, ry))
                continue;

            unsigned int appl = 0;
            struct SNAKE *s = snake_tail;

            while (s->next)
            {
                if (is_apple(s->x, s->y))
                {
                    appl = 1;
                    break;
                }
                
                s = s->next;
            }

            if (!appl)
                break;
        }
    }
}

static void init_snake()
{
    snake_length = SNAKE_INIT_LENGTH;
    score = 0;
    direction = RIGHT;

    for (unsigned int i = 0; i < SNAKE_MAX_LENGTH; ++i)
    {
        snake[i].x = 0xdeadbeef;
    }

    for (unsigned int i = 0; i < SNAKE_INIT_LENGTH; ++i)
    {
        snake[i].x = i == 0 ? (64 - SNAKE_INIT_LENGTH - 1) : snake[i - 1].x + 1;
        snake[i].y = 32;
        snake[i].next = &snake[i + 1];
    }

    snake[SNAKE_INIT_LENGTH - 1].next = 0;
    snake_tail = &snake[0];

    generate_apples();
}

static void move_snake()
{
    struct SNAKE *s = snake_tail;
    while (s->next)
    {
        s = s->next;
    }

    // add new head
    int new_i = find_free();
    s->next = &snake[new_i];
    s->next->x = s->x;
    s->next->y = s->y;
    s = s->next;

    if (direction == UP && s->y > 0)
        s->y--;
    else if (direction == DOWN && s->y < 63)
        s->y++;
    else if (direction == LEFT && s->x > 0)
        s->x--;
    else if (direction == RIGHT && s->x < 127)
        s->x++;

    s->next = 0;

    // invalidate tail
    lcd_pixel(snake_tail->x, snake_tail->y, 0, 1);
    snake_tail->x = 0xdeadbeef;
    snake_tail = snake_tail->next;
}

static void feed_snake()
{
    struct SNAKE *s = snake_tail;
    while (s->next)
    {
        s = s->next;
    }

    unsigned int i = find_free();
    if (i == 0xdeadbeef)
        return;

    s->next = &snake[i];
    
    snake[i].x = s->x;
    snake[i].y = s->y;

    unsigned int j = find_free();
    if (j == 0xdeadbeef)
        return;

    snake[i].next = &snake[j];
    snake[j].x = s->x;
    snake[j].y = s->y;
    snake[j].next = 0;

    switch(direction)
    {
        case UP: snake[i].y -= 1; snake[j].y -= 2; break;
        case DOWN: snake[i].y += 1; snake[j].y += 2; break;
        case LEFT: snake[i].x -= 1; snake[j].x -= 2; break;
        case RIGHT: snake[i].x += 1; snake[j].x += 2; break;
    }

    return;
}

static void print_score()
{
    unsigned char* _score = get_score();
    lcd_putc(10 * 8, 56, _score[0], 0);
    lcd_putc(11 * 8, 56, _score[1], 0);
    lcd_putc(12 * 8, 56, _score[2], 1);
}

static void print_snake()
{
    struct SNAKE *s = snake_tail;
    while (s)
    {
        lcd_pixel(s->x, s->y, 1, 1);
        s = s->next;
    }
}

static void print_apples()
{
    for (unsigned int i = 0; i < APPLE_MAX; ++i)
    {
        if (apples[i].x != 0xdeadbeef)
            lcd_putc(apples[i].x, apples[i].y, '\x00', 1);
    }
}

static unsigned int handle_collision()
{
    struct SNAKE *s = snake_tail;
    while (s->next)
    {
        s = s->next;
    }

    int i = is_apple(s->x, s->y);
    if (i)
    {
        lcd_putc(apples[i - 1].x, apples[i - 1].y, ' ', 1);
        apples[i - 1].x = 0xdeadbeef;
        feed_snake();
        score++;

        unsigned int ap = 0;
        for(unsigned int j = 0; j < APPLE_MAX; ++j)
            if (apples[j].x != 0xdeadbeef)
                ap++;
        
        if (ap == 0)
            generate_apples();

        return 0;
    }
    
    if (is_border(s->x, s->y))
    {
        return 1;
    }

    return 0;
}


SCREENS screen_snake()
{
    lcd_draw_screen(playground_screen);
    init_snake();

    while (1)
    {
        print_score();
        print_apples();
        print_snake();

        unsigned int read_keys = 1;

        for (unsigned int i = 0; i < 75; ++i)
        {
            if (read_keys)
            {
                KEYS key = keys_read();
                switch (key)
                {
                case KEY_UP:
                    if (direction != DOWN)
                        direction = UP;
                    read_keys = 0;
                    break;
                case KEY_DOWN:
                    if (direction != UP)
                        direction = DOWN;
                    read_keys = 0;
                    break;
                case KEY_LEFT:
                    if (direction != RIGHT)
                        direction = LEFT;
                    read_keys = 0;
                    break;
                case KEY_RIGHT:
                    if (direction != LEFT)
                        direction = RIGHT;
                    read_keys = 0;
                    break;
                default:
                    break;
                }
            }
            delay_ms(1);
        }

        move_snake();
        if (handle_collision())
            return SCREEN_FINISH;
    }

    return SCREEN_FINISH;
}

unsigned char* get_score()
{
    static unsigned char _score[3];

    if (score < 1000)
    {
        unsigned int hu = score / 100;
        unsigned int de = (score - hu * 100) / 10;
        unsigned int di = score - hu * 100 - de * 10;

        _score[0] = '0' + hu;
        _score[1] = '0' + de;
        _score[2] = '0' + di;
    }
    else
    {
        _score[0] = 'M';
        _score[1] = 'A';
        _score[2] = 'X';
    }
    
    return _score;
}
#include "main.h"

uint8_t leds[] = {LED1, LED2, LED3, LED4};

void leds_init()
{
    // leds
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    // leds test
    int led_delay = 300;
    digitalWrite(LED1, 1);delay(led_delay);
    digitalWrite(LED2, 1);delay(led_delay);
    digitalWrite(LED3, 1);delay(led_delay);
    digitalWrite(LED4, 1);delay(led_delay);
    //digitalWrite(LED1, 0);delay(led_delay >> 1);
    digitalWrite(LED2, 0);delay(led_delay >> 1);
    digitalWrite(LED3, 0);delay(led_delay >> 1);
    digitalWrite(LED4, 0);delay(led_delay >> 1);
}

void leds_anim(uint8_t pattern1, uint8_t pattern2, int ms, int timeout)
{
    for(int c=0;c<(timeout/(ms*2));c++)
    {
        for (int p=0;p<4;p++)
            digitalWrite(leds[p], ((pattern1 & (1<<p)) == (1<<p)));
        delay(ms);
        for (int p=0;p<4;p++)
            digitalWrite(leds[p], ((pattern2 & (1<<p)) == (1<<p)));
        delay(ms);
    }
}
void leds_set(uint8_t state)
{
    for (int p=0;p<4;p++)
        digitalWrite(leds[p], state);
}
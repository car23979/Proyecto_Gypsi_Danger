/*
 * led_rgb.h
 *
 * Created: 12/05/2025 12:00:03 p. m.
 *  Author: Admin
 */ 


#ifndef LED_RGB_H_
#define LED_RGB_H_

#include <stdint.h>

void led_rgb_init();
void led_rgb_set(uint8_t r, uint8_t g, uint8_t b);

#endif /* LED_RGB_H_ */
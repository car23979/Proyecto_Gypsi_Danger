/*
 * joystick.h
 *
 * Created: 08/05/2025 03:30:20 p. m.
 *  Author: Admin
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>

typedef enum {
	JOY_HEAD_H = 0,
	JOY_HEAD_V,
	JOY_ARM_L,
	JOY_ARM_R,
	NUM_JOYSTICKS
	} joystick_channel_t;

void joystick_init();
uint16_t joystick_read(joystick_channel_t channel);
void joystick_update_servos();

#endif /* JOYSTICK_H_ */
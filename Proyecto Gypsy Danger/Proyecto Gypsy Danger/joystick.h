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
	JOY_HEAD_H = 0,		// Canal 0 - Hombro izquierdo
	JOY_HEAD_V,			// Canal 1 - Hombro derecho
	JOY_ARM_L,			// Canal 2 - Brazo izquierdo
	JOY_ARM_R,			// Canal 3 - Brazo derecho
	NUM_JOYSTICKS
	} joystick_channel_t;

void joystick_init();
uint16_t joystick_read(joystick_channel_t channel);
void joystick_update_servos();

#endif /* JOYSTICK_H_ */
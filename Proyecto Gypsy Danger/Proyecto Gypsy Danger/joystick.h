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
	NUM_JOYSTICKS		// Número total de joysticks
	} joystick_channel_t;

// Inicializa Joysticks y el ADC
void joystick_init();

// Lee el valor de un canal específico del joystick
uint16_t joystick_read(joystick_channel_t channel);

// Actualiza las posiciones de los servos de acuerdo a los joysticks
void joystick_update_servos();

#endif /* JOYSTICK_H_ */
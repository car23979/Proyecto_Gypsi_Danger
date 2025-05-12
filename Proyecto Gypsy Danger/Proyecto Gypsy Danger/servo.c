/*
 * servo.c
 *
 * Created: 08/05/2025 03:29:15 p. m.
 *  Author: Admin
 */ 

#include "servo.h"
#include <avr/interrupt.h>
#include "pin_definitions.h"

volatile uint16_t servo_head_h_pulse = 1000;  // en microsegundos
volatile uint16_t servo_head_v_pulse = 1000;

void servo_init(){
	// Timer1 para servos de brazo (PB1 y PB2)
	 
	TCCR1A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR1B = (1 << CS01) | (1 << CS00);	// Prescales 64
	
	
}

void servo_set_position(servo_channel_t channel, uint16_t position) {
	if(channel >= NUM_SERVOS) return;
	
	// Limitar rango (1000 - 2000 ?s)
	if(position < 1000) position = 1000;
	if(position > 2000) position = 2000;
	
	servo_positions[channel] = position;
	servo_update(channel);
}

uint16_t servo_get_position(servo_channel_t channel) {
	if(channel >= NUM_SERVOS) return 0;
	return servo_positions[channel];
}

void servo_update(servo_channel_t channel) {
	switch(channel) {
		case SERVO_HEAD_H:
			OCR0B = (servo_positions[channel] - 1000) >> 2;
			break;
		case SERVO_HEAD_V:
			OCR0A = (servo_positions[channel] - 1000) >> 2;
			break;
		case SERVO_ARM_L:
			OCR1A = servo_positions[channel];
			break;
		case SERVO_ARM_R:
			OCR1B = servo_positions[channel];
			break;
		default:
			break;
	}
}
 // Actualiza servos
void servo_update_all() {
	for(uint8_t i = 0; i < NUM_SERVOS; i++) {
		servo_update(i);
	}
}
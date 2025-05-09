/*
 * servo.c
 *
 * Created: 08/05/2025 03:29:15 p. m.
 *  Author: Admin
 */ 

#include "servo.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pin_definitions.h"

static volatile uint16_t servo_positions[NUM_SERVOS];

void servo_init(){
	// Configurar Timer0 para servos de hombro
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << CS01) | (1 << CS00);	// Prescales 64
	
}
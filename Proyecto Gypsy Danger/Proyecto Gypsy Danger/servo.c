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
	
	// Configurar Timer1 para servos de brazos
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM00);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescales 8, en modo Fast PWM ICR1
	ICR1 = 39999; // Periodo de 20ms
	
	// Establecer pines como salida
	DDRD |= (1 << SERVO_HEAD_H_PIN) | (1 << SERVO_HEAD_V_PIN);
	DDRB |= (1 << SERVO_ARM_L_PIN) | (1 << SERVO_ARM_R_PIN);
	
	// Posiciones iniciales (centro)
	for(uint8_t i = 0; i < NUM_SERVOS; i++) {
		servo_positions[i] = 1500; // 1.5ms (posición central)
	}
	servo_update_all();
}

void servo_set_position(servo_channel_t channel, uint16_t position) {
	if(channel >= NUM_SERVOS) return;
	
	// Limitar rango (1000 - 2000 ?s)
	if(position < 1000) position = 1000;
	if(position > 2000) position = 2000;
	
	servo_positions[channel] = position;
	servo_update(channel);
}


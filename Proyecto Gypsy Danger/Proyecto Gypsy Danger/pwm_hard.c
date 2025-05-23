/*
 * pwm_hard.c
 *
 * Created: 16/05/2025 03:25:29 p. m.
 *  Author: Admin
 */ 

#include <avr/io.h>
#include "pwm_hard.h"

void configurar_pwm_hardware(void){
	DDRB |= (1 << PB1) | (1 << PB2); // Salidas OC1A y OC1B
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // PWM rápido, modo 14
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8
	ICR1 = 39999; // TOP para 20ms a 16MHz
}

void actualizar_PB1(uint16_t us){
	OCR1A = (us * 2); // Conversión µs a ticks (16MHz / 8)
}

void actualizar_PB2(uint16_t us){
	OCR1B = (us * 2);
}

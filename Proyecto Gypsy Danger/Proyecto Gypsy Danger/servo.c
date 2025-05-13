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
	DDRB |= (1 << PB1) | (1 << PB2); 
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12); // Prescales 8
	ICR1 = 39999; // TOP = 20ms a 16MHz/8
	
	// Timer2 para PWM software en hombro
	DDRD |= (1 << PD5) | (1 << PD6);	// Salida
	TCCR2A = (1 << WGM21);				// CTC
	TCCR2B = (1 << CS22);				// Prescaler 64
	OCR2A = 250;						// Cada -1ms a 16Mhz con preacaler 64
	TIMSK2 = (1 << OCIE2A);				// Habilitar interrupción
	sei();	
}

void servo_set_position(uint8_t pin, uint16_t adc_value) {
	uint16_t pulse = 1000 + ((uint32_t)adc_value * 1000 / 1023); // 1000 a 2000 us
	
	switch(pin) {
		case SERVO_ARM_L_PIN: OCR1A = (pulse * 2); break;  // 0.5us = 1 cuenta
		case SERVO_ARM_R_PIN: OCR1B = (pulse * 2); break;
		case SERVO_HEAD_H_PIN: servo_head_h_pulse = pulse; break;
		case SERVO_HEAD_V_PIN: servo_head_v_pulse = pulse; break;
		default: break;
	}
}

// Variables auxiliares para el software PWM
volatile uint8_t software_pwm_counter = 0;

ISR(TIMER2_COMPA_vect) {
	software_pwm_counter++;
	if (software_pwm_counter >= 20) software_pwm_counter = 0;
	if (software_pwm_counter == 0) {
		PORTD |= (1 << SERVO_HEAD_H_PIN) | (1 << SERVO_HEAD_V_PIN);
	} else {
		if (software_pwm_counter * 100 >= servo_head_h_pulse)
			PORTD &= ~(1 << SERVO_HEAD_H_PIN);
		if (software_pwm_counter * 100 >= servo_head_v_pulse)
			PORTD &= ~(1 << SERVO_HEAD_H_PIN);
	}
}
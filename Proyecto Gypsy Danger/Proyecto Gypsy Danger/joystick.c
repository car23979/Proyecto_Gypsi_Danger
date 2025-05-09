/*
 * joystick.c
 *
 * Created: 08/05/2025 03:30:04 p. m.
 *  Author: Admin
 */ 

#include "joystick.h"
#include "servo.h"
#include "pin_definitions.h"
#include <avr/io.h>

void joystick_init() {
	ADMUX = (1 << REFS0); // AVcc como referencia
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable, prescaler 128
}

uint16_t joystick_read(joystick_channel_t channel) {
	if (channel >= NUM_JOYSTICKS) return 0;
	ADMUX = (ADMUX & 0xF0) | channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADC;
}

void joystick_update_servos() {
	for(uint8_t i = 0; i < NUM_JOYSTICKS; i++){
		uint16_t adc_value = joystick_read();
		// Mapear ADC (0-1023) a rango servo (1000-2000 us)
		uint16_t servo_pos = (adc_value >> 2) + 1000;
		servo_set_position(i, servo_pos);
	}
}
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

// Función para leer un canal específico del joystick
uint16_t joystick_read(joystick_channel_t channel) {
	if(channel >= NUM_JOYSTICKS) return 0;
	
	ADMUX = (ADMUX & 0xF0) | channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADC;
}

// Función para actualizar todos los servos según los joysticks
void joystick_update_servos() {
	for(uint8_t i = 0; i < NUM_JOYSTICKS && i < NUM_SERVOS; i++) {
		uint16_t adc_value = joystick_read(i); // Pasar el canal correctamente
		uint16_t servo_pos = (adc_value >> 2) + 1000;
		servo_set_position((servo_channel_t)i, servo_pos);
	}
}
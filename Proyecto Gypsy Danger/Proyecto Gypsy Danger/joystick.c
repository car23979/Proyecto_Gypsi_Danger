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

// Función para inicializar el ADC
void joystick_init() {
	// Configurar el ADC (prescaler 64)
	ADMUX = 0x00;	// Seleccionar Vref como referencia ADC0
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Habilitar ADC y prescaler 64
}

// Función para leer el canal especifico del joystick
uint16_t joystick_read(joystick_channel_t channel) {
	if(channel >= NUM_JOYSTICKS) return 0;
	
	ADMUX = (ADMUX & 0xF0) | channel;	// Selecciona el canal ADC
	ADCSRA |= (1 << ADSC);				// Iniciar conversión
	while(ADCSRA & (1 << ADSC));		// Esperar a que la conversión termine
	return ADC;							// Devolver el valor ADC
}

// Función para actualizar todos los servos según los joysticks
void joystick_update_servos() {
	for(uint8_t i = 0; i < NUM_JOYSTICKS && i < NUM_SERVOS; i++) {
		uint16_t adc_value = joystick_read(i); // Pasar el canal correctamente
		uint16_t servo_pos = (adc_value >> 2) + 1000;
		servo_set_position((servo_channel_t)i, servo_pos);
	}
}
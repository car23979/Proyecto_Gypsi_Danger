/*
 * led_rgb.c
 *
 * Created: 12/05/2025 12:00:32 p. m.
 *  Author: Admin
 */ 

#include "led_rgb.h"
#include "pin_definitions.h"
#include <avr/io.h>

void led_rgb_init() {
	// Configurar pines como salida
	DDRB |= (1 << LED_R_PIN);
	DDRC |= (1 << LED_G_PIN) | (1 << LED_B_PIN);
	
	// Inicialmente apagado
	led_rgb_set(LED_OFF);
}

void led_rgb_set(uint8_t r, uint8_t g, uint8_t b) {
	// Para cátodo común, 0 es máximo brillo
	OCR2A = 255 - r;  // Rojo (Timer2 PWM)
	// Usar PWM por software para los otros canales
	// (Implementación básica - puedes mejorar con más timers)
	if(g > 127) PORTC |= (1 << LED_G_PIN);
	else PORTC &= ~(1 << LED_G_PIN);
	
	if(b > 127) PORTC |= (1 << LED_B_PIN);
	else PORTC &= ~(1 << LED_B_PIN);
}
/*
 * adc_config.c
 *
 * Created: 16/05/2025 03:25:58 p. m.
 *  Author: Admin
 */ 

#include <avr/io.h>
#include "adc_config.h"

void configurar_ADC(void){
	ADMUX = (1 << REFS0) | (1 << ADLAR); // AVcc como ref, justificación a la izquierda
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1); // Habilita ADC, interrupción, prescaler 16
	ADCSRA |= (1 << ADSC); // Inicia primera conversión
}

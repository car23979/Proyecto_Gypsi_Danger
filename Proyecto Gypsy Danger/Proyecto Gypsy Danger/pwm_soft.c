/*
 * pwm_soft.c
 *
 * Created: 16/05/2025 03:24:54 p. m.
 *  Author: Admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm_soft.h"

volatile uint16_t pulsoPB3 = 1500;
volatile uint16_t pulsoPB4 = 1500;
volatile uint16_t pulsoPB5 = 1500;
volatile uint16_t timer_ticks = 0;

void configurar_pwm_software(void){
	DDRB |= (1 << PB3) | (1 << PB4) | (1 << PB5);
	TCCR0A = 0;
	TCCR0B = (1 << CS01); // Prescaler 8
	TIMSK0 = (1 << TOIE0); // Interrupción por overflow
	TCNT0 = 0;
}

ISR(TIMER0_OVF_vect){
	timer_ticks++;
	if (timer_ticks == 1){
		PORTB |= (1 << PB3) | (1 << PB4) | (1 << PB5);
	}
	if (timer_ticks == (pulsoPB3 / 100)){
		PORTB &= ~(1 << PB3);
	}
	if (timer_ticks == (pulsoPB4 / 100)){
		PORTB &= ~(1 << PB4);
	}
	if (timer_ticks == (pulsoPB5 / 100)){
		PORTB &= ~(1 << PB5);
	}
	if (timer_ticks >= 200){ // 20 ms
		timer_ticks = 0;
	}
}

void actualizar_PB3(uint16_t us){
	pulsoPB3 = us;
}
void actualizar_PB4(uint16_t us){
	pulsoPB4 = us;
}
void actualizar_PB5(uint16_t us){
	pulsoPB5 = us;
}

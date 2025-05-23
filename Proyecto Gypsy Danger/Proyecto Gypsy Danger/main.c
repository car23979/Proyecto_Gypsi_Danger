/*
 * Proyecto Gypsy Danger.c
 *
 * Created: 08/05/2025 12:25:21 a. m.
 * Author : David Carranza
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pwm_soft.h"
#include "pwm_hard.h"
#include "adc_config.h"
#include "EEPROM.h"
#include "uart.h"

#define BTN_MODO     PD2  // INT0 (opcional interrupción externa)
#define BTN_GUARDAR  PD3
#define BTN_EJECUTAR PD4

#define LED_R        PD5
#define LED_G        PD6
#define LED_B        PD7

// Estados del sistema
#define MODO_MANUAL  0
#define MODO_EEPROM  1
#define MODO_UART    2

// Manual
volatile uint8_t axis_value = 0;        // Canal ADC actual (0 a 4)
volatile uint8_t pos_s1 = 0;            // Servo cabeza
volatile uint8_t pos_s2 = 0;            // Servo hombro 1
volatile uint8_t pos_s3 = 0;            // Servo brazo 1
volatile uint8_t pos_s4 = 0;            // Servo brazo 2
volatile uint8_t pos_s5 = 0;            // Servo hombro 2

// EEPROM
volatile uint8_t modo_actual = MODO_MANUAL;
volatile uint8_t servo_positions[5][5]; // Hasta 5 sets de 5 servos
volatile uint8_t total_guardadas = 0;


//---------Funciones Principales --------

// Mueve todos los servos
void mover_servos(void){
	// PWM por hardware: PB1 y PB2 (Timer1)
	actualizar_PB1(500 + (pos_s1 * 2000UL / 255));  // Cabeza
	actualizar_PB2(500 + (pos_s2 * 2000UL / 255));  // Hombro 1

	// PWM por software: PB3, PB4, PB5 (Timer0)
	actualizar_PB3(500 + (pos_s3 * 2000UL / 255));  // Brazo 1
	actualizar_PB4(500 + (pos_s4 * 2000UL / 255));  // Brazo 2
	actualizar_PB5(500 + (pos_s5 * 2000UL / 255));  // Hombro 2
}

// Función para configurar botones y LEDs
void configurar_botones_y_leds(void){
	DDRD &= ~((1 << BTN_MODO) | (1 << BTN_GUARDAR) | (1 << BTN_EJECUTAR)); // Entradas
	PORTD |= (1 << BTN_MODO) | (1 << BTN_GUARDAR) | (1 << BTN_EJECUTAR);   // Pull-ups

	DDRD |= (1 << LED_R) | (1 << LED_G) | (1 << LED_B); // LEDs como salida
}

// Control de LEDs según modo
void set_led_rgb(uint8_t r, uint8_t g, uint8_t b) {
	if (r) PORTD |= (1 << LED_R); else PORTD &= ~(1 << LED_R);
	if (g) PORTD |= (1 << LED_G); else PORTD &= ~(1 << LED_G);
	if (b) PORTD |= (1 << LED_B); else PORTD &= ~(1 << LED_B);
}

void actualizar_LED_modo(void) {
	switch(modo_actual){
		case MODO_MANUAL: set_led_rgb(0, 1, 0); break;  // Verde
		case MODO_EEPROM: set_led_rgb(0, 0, 1); break;  // Azul
		case MODO_UART:   set_led_rgb(1, 0, 0); break;  // Rojo
	}
}

void indicar_guardado_exitoso(uint8_t index) {
	// Mostrar color distinto para cada posición (sin usar RGB puro)
	switch (index) {
		case 0: set_led_rgb(1, 1, 0); break; // Amarillo
		case 1: set_led_rgb(0, 1, 1); break; // Cian
		case 2: set_led_rgb(1, 0, 1); break; // Magenta
		case 3: set_led_rgb(1, 1, 1); break; // Blanco
		case 4: set_led_rgb(1, 0.5, 0); break; // Naranja (aproximado como rojo+verde)
	}
	_delay_ms(300);
	actualizar_LED_modo(); // Volver al color del modo actual
}

// --- Controla ADC según modo ---
void actualizar_adc_por_modo(void) {
	if (modo_actual == MODO_UART) {
		ADCSRA &= ~(1 << ADIE); // Deshabilita interrupción ADC
		ADCSRA &= ~(1 << ADEN); // Apaga ADC (opcional)
		} else {
		ADCSRA |= (1 << ADEN);  // Enciende ADC
		ADCSRA |= (1 << ADIE);  // Habilita interrupción ADC
		ADCSRA |= (1 << ADSC);  // Inicia conversión
	}
}

// Función para procesar comandos UART
void UART_ProcessCommand(const char *cmd) {
	set_led_rgb(0, 0, 1); // Azul
	_delay_ms(50);
	actualizar_LED_modo();

	if (strncmp(cmd, "MOVE", 4) == 0) {
		uint8_t vals[5] = {0};
		int n = sscanf(cmd, "MOVE %hhu %hhu %hhu %hhu %hhu",
		&vals[0], &vals[1], &vals[2], &vals[3], &vals[4]);
		if (n == 5) {
			pos_s1 = vals[0];
			pos_s2 = vals[1];
			pos_s3 = vals[2];
			pos_s4 = vals[3];
			pos_s5 = vals[4];
			mover_servos();
		}
	}
	else if (strstr(cmd, ":") != NULL) {
		char *servo = strtok((char*)cmd, ":");
		uint8_t valor = atoi(strtok(NULL, ":"));
		
		if (strcmp(servo, "C") == 0) pos_s1 = valor;
		else if (strcmp(servo, "H1") == 0) pos_s2 = valor;
		else if (strcmp(servo, "B1") == 0) pos_s3 = valor;
		else if (strcmp(servo, "B2") == 0) pos_s4 = valor;
		else if (strcmp(servo, "H2") == 0) pos_s5 = valor;

		mover_servos();
	}
}


ISR(ADC_vect){
	switch (axis_value){
		case 0: pos_s1 = ADCH; break;
		case 1: pos_s2 = ADCH; break;
		case 2: pos_s3 = ADCH; break;
		case 3: pos_s4 = ADCH; break;
		case 4: pos_s5 = ADCH; break;
	}
	axis_value = (axis_value + 1) % 5;
	ADMUX = (ADMUX & 0xF0) | axis_value;
	ADCSRA |= (1 << ADSC);  // Inicia nueva conversión
}

int main(void){
	configurar_ADC();
	configurar_pwm_hardware();
	configurar_pwm_software();
	configurar_botones_y_leds();
	actualizar_LED_modo();
	UART_init(103);		// 9600 bauds para 16Mhz
	sei();// hABILITAR INTERRUPCIÓN
	
	actualizar_adc_por_modo(); // Inicializar ADC acorde al modo inicial

	while (1){
		if (modo_actual == MODO_MANUAL || modo_actual == MODO_EEPROM){
			mover_servos(); // Solo se mueven con joystick
		}
		
		if (modo_actual == MODO_UART) {
			char buffer[50];
			if (UART_read_line(buffer, sizeof(buffer))) {
				UART_ProcessCommand(buffer);
			}
		}


		// --- Lógica de botones ---
		if (!(PIND & (1 << BTN_MODO))){
			_delay_ms(300);
			modo_actual = (modo_actual + 1) % 3;
			actualizar_LED_modo();
			actualizar_adc_por_modo();
		}
		
		// Guardar posición actual en EEPROM
		if (!(PIND & (1 << BTN_GUARDAR)) && modo_actual == MODO_EEPROM){
			_delay_ms(300);
			if (total_guardadas >= 5){
				total_guardadas = 0;

				// Parpadeo blanco para indicar reinicio
				for (uint8_t i = 0; i < 3; i++) {
					set_led_rgb(1, 1, 1);
					_delay_ms(200);
					set_led_rgb(0, 0, 0);
					_delay_ms(200);
				}
				actualizar_LED_modo();
			}
			
			
			servo_positions[total_guardadas][0] = pos_s1;
			servo_positions[total_guardadas][1] = pos_s2;
			servo_positions[total_guardadas][2] = pos_s3;
			servo_positions[total_guardadas][3] = pos_s4;
			servo_positions[total_guardadas][4] = pos_s5;
			
			// Guarda EEPROM correctamente
			EEPROM_save_servo_positions((uint8_t*)servo_positions[total_guardadas], total_guardadas);
			
			// Parpadeo de LED para indicar exito
			indicar_guardado_exitoso(total_guardadas);  // Feedback visual
			
			// Incremento
			total_guardadas++;

		}
		

		// Ejecutar todas las posiciones desde EEPROM
		if (!(PIND & (1 << BTN_EJECUTAR)) && modo_actual == MODO_EEPROM){
			_delay_ms(300);
			for (uint8_t i = 0; i < total_guardadas; i++){
				EEPROM_load_servo_positions((uint8_t*)servo_positions[i], i);

				pos_s1 = servo_positions[i][0];
				pos_s2 = servo_positions[i][1];
				pos_s3 = servo_positions[i][2];
				pos_s4 = servo_positions[i][3];
				pos_s5 = servo_positions[i][4];
				mover_servos();
				_delay_ms(1000);
			}
		}

		_delay_ms(10);
	}
}
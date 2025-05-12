/*
 * Proyecto Gypsy Danger.c
 *
 * Created: 08/05/2025 12:25:21 a. m.
 * Author : David Carranza
 */ 

#define  F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "gypsi_danger.h"
#include "servo.h"
#include "joystick.h"
#include "eeprom_manager.h"
#include "uart_comm.h"
#include "led_rgb.h"
#include "pin_definitions.h"


typedef enum {
	MODE_MANUAL = 0,
	MODE_EEPROM,
	MODE_UART,
	NUM_MODES	// Este valos no es necesario manejarlo en el switch
} operation_mode_t;

static volatile operation_mode_t current_mode = MODE_MANUAL;
static volatile uint8_t eeprom_playback_pos = 0;

void gpio_init() {
	// Configurar botones como entrada con pull-up
	DDRD &= ~((1 << BTN_SAVE_PIN) | (1 << BTN_MODE_PIN));
	PORTD |= (1 << BTN_SAVE_PIN) | (1 << BTN_MODE_PIN);
}

void update_led() {
	switch(current_mode) {
		case MODE_MANUAL:
		led_rgb_set(LED_RED);  // Rojo para modo manual
		break;
		case MODE_EEPROM:
		led_rgb_set(LED_BLUE); // Azul para modo EEPROM
		break;
		case MODE_UART:
		led_rgb_set(LED_GREEN); // Verde para modo UART
		break;
	}
}

void check_buttons() {
	static uint8_t last_btn_mode_state = 1;
	static uint8_t last_btn_save_state = 1;
	
	uint8_t btn_mode_state = PIND & (1 << BTN_MODE_PIN);
	uint8_t btn_save_state = PIND & (1 << BTN_SAVE_PIN);
	
	// Detección de flanco descendente (botón presionado)
	if(last_btn_mode_state && !btn_mode_state) {
		current_mode = (current_mode + 1) % NUM_MODES;
		eeprom_playback_pos = 0;
		update_led(); // Actualizar color LED inmediatamente
		_delay_ms(50); // Debounce
	}
	
	if(last_btn_save_state && !btn_save_state && current_mode == MODE_MANUAL) {
		eeprom_save_position(eeprom_playback_pos);
		// Feedback visual - parpadeo rápido
		for(uint8_t i = 0; i < 3; i++) {
			led_rgb_set(LED_WHITE);
			_delay_ms(100);
			led_rgb_set(LED_RED);
			_delay_ms(100);
		}
		eeprom_playback_pos = (eeprom_playback_pos + 1) % NUM_POSITIONS;
		_delay_ms(50); // Debounce
	}
	
	last_btn_mode_state = btn_mode_state;
	last_btn_save_state = btn_save_state;
}

int main(void) {
	// Inicializaciones
	gpio_init();
	servo_init();
	joystick_init();
	eeprom_init();
	uart_init(9600);
	led_rgb_init();
	
	// Bucle principal
	while(1) {
		check_buttons();
		
		switch(current_mode) {
			case MODE_MANUAL:
				joystick_update_servos();
				break;
			
			case MODE_EEPROM:
				eeprom_load_position(eeprom_playback_pos);
				_delay_ms(1000); // Espera entre posiciones
				eeprom_playback_pos = (eeprom_playback_pos + 1) % NUM_POSITIONS;
				break;
			
			case MODE_UART:
				if(uart_available()) {
					char buffer[32];
					uart_read_line(buffer, sizeof(buffer));
					process_uart_command(buffer);
				}
				uart_send_servo_positions(); // Enviar estado actual
				_delay_ms(100);
				break;
		}
	}
	
	return 0;
}
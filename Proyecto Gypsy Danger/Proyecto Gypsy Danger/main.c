/*
 * Proyecto Gypsy Danger.c
 *
 * Created: 08/05/2025 12:25:21 a. m.
 * Author : David Carranza
 */ 

#include "gypsi_danger.h"
#include "lib/servo/servo.h"
#include "lib/joystick/joystick.h"
#include "lib/eeprom_manager/eeprom_manager.h"
#include "lib/uart_comm/uart_comm.h"
#include "lib/adafruit_io/adafruit_io.h"
#include "utils/pin_definitions.h"
#include <avr/io.h>
#include <util/delay.h>

typedef enum {
	MODE_MANUAL = 0,
	MODE_EEPROM,
	MODE_UART,
	NUM_MODES
} operation_mode_t;

static volatile operation_mode_t current_mode = MODE_MANUAL;
static volatile uint8_t eeprom_playback_pos = 0;

void gpio_init() {
	// Configurar botones como entrada con pull-up
	DDRD &= ~((1 << BTN_SAVE_PIN) | (1 << BTN_MODE_PIN));
	PORTD |= (1 << BTN_SAVE_PIN) | (1 << BTN_MODE_PIN);
	
	// Configurar LED como salida
	DDRD |= (1 << LED_MODE_PIN);
}

void update_led() {
	static uint16_t counter = 0;
	counter++;
	
	switch(current_mode) {
		case MODE_MANUAL:
		PORTD |= (1 << LED_MODE_PIN); // LED siempre encendido
		break;
		case MODE_EEPROM:
		if(counter % 1000 == 0) { // Parpadeo lento
			PORTD ^= (1 << LED_MODE_PIN);
		}
		break;
		case MODE_UART:
		if(counter % 200 == 0) { // Parpadeo rápido
			PORTD ^= (1 << LED_MODE_PIN);
		}
		break;
		default:
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
		_delay_ms(50); // Debounce
	}
	
	if(last_btn_save_state && !btn_save_state && current_mode == MODE_MANUAL) {
		eeprom_save_position(eeprom_playback_pos);
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
	adafruit_io_init();
	
	// Bucle principal
	while(1) {
		check_buttons();
		update_led();
		
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
			adafruit_io_process();
			adafruit_io_send_status();
			break;
		}
	}
	
	return 0;
}
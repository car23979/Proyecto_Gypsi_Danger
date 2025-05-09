/*
 * eeprom_manager.c
 *
 * Created: 08/05/2025 03:32:07 p. m.
 *  Author: Admin
 */ 

#include "eeprom_manager.h"
#include "servo.h"
#include <avr/eeprom.h>

static uint8_t eeprom_data[NUM_POSITIONS][NUM_SERVOS];

void eeprom_init() {
	// Leer todas las posiciones al iniciar
	for(uint8_t pos = 0; pos < NUM_POSITIONS; pos++){
		for(uint8_t servo = 0; servo < NUM_SERVOS; servo++){
			eeprom_data[pos][servo] = eeprom_read_byte((uint8_t*)(pos * NUM_SERVOS + servo));
		}
	}
}

void eeprom_save_position(uint8_t pos_num) {
	if(pos_num >= NUM_POSITIONS) return;
	for(uint8_t servo = 0, servo < NUM_SERVOS; servo++) {
		uint16_t pos = servo_get_position(servo);
		eeprom_data[pos_num][servo] = (uint8_t)(pos >> 2); // Almacenar como byte
		eeprom_write_byte((uint8_t*)(pos_num * NUM_SERVOS + servo), eeprom_data[pos_num][servo]);
	}
}
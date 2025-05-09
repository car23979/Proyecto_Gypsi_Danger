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
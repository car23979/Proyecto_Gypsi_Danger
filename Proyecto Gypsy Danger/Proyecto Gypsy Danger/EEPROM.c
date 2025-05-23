/*
 * EEPROM.c
 *
 * Created: 16/05/2025 05:44:57 p. m.
 *  Author: Admin
 */ 

#include "EEPROM.h"

void EEPROM_write(uint8_t dato, uint16_t direccion) {
	while (EECR & (1 << EEPE));  // Espera si EEPROM está ocupada
	EEAR = direccion;
	EEDR = dato;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}

uint8_t EEPROM_read(uint16_t direccion) {
	while (EECR & (1 << EEPE));
	EEAR = direccion;
	EECR |= (1 << EERE);
	return EEDR;
}

// Guarda una posición completa (5 servos) en la EEPROM en el índice especificado (0 a 4)
void EEPROM_save_servo_positions(uint8_t *servos, uint8_t index) {
	if (index >= 5) return;  // Índice fuera de rango
	uint16_t base_addr = index * 5;
	for (uint8_t i = 0; i < 5; i++) {
		EEPROM_write(servos[i], base_addr + i);
	}
}

// Carga una posición completa (5 servos) desde EEPROM al índice especificado (0 a 4)
void EEPROM_load_servo_positions(uint8_t *servos, uint8_t index) {
	if (index >= 5) return;
	uint16_t base_addr = index * 5;
	for (uint8_t i = 0; i < 5; i++) {
		servos[i] = EEPROM_read(base_addr + i);
	}
}

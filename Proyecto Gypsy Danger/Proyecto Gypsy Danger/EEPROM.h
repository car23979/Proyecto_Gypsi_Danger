/*
 * EEPROM.h
 *
 * Created: 16/05/2025 05:45:12 p. m.
 *  Author: Admin
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

void EEPROM_write(uint8_t dato, uint16_t direccion);
uint8_t EEPROM_read(uint16_t direccion);
// Guarda o carga una posición completa de 5 servos en una posición (0 a 4)
void EEPROM_save_servo_positions(uint8_t *servos, uint8_t index);
void EEPROM_load_servo_positions(uint8_t *servos, uint8_t index);


#endif /* EEPROM_H_ */
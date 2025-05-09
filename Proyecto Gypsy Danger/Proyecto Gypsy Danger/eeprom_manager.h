/*
 * eeprom_manager.h
 *
 * Created: 08/05/2025 03:31:50 p. m.
 *  Author: Admin
 */ 


#ifndef EEPROM_MANAGER_H_
#define EEPROM_MANAGER_H_

#include <stdint.h>
#define NUM_POSITIONS 4

void eeprom_init();
void eeprom_save_position(uint8_t pos_num);
void eeprom_load_position(uint8_t pos_num);
uint8_t eeprom_get_position_data(uint8_t pos_num, uint8_t servo_num);

#endif /* EEPROM_MANAGER_H_ */
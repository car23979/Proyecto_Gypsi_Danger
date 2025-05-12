/*
 * uart_comm.c
 *
 * Created: 08/05/2025 03:36:14 p. m.
 *  Author: Admin
 */ 

#include "uart_comm.h"
#include "servo.h"
#include "eeprom_manager.h"
#include <avr/io.h>
#include <string.h>
#include <stdlib.h> // Para atoi()
#include <stdio.h>	// Para snprintf()

// Variables externas compartidas con main.c
extern volatile uint8_t current_mode;
extern volatile uint8_t eeprom_playback_pos;
extern const uint8_t NUM_MODES;

// Implementación de uart_available
uint8_t uart_available(void) {
	return (UCSR0A & (1 << RXC0));
}

// Implementación de uart_read_line
uint8_t uart_read_line(char* buffer, uint8_t buffer_size) {
	static uint8_t idx = 0;
	char c;
	
	while(uart_available()) {
		c = UDR0;
		
		if(c == '\r' || c == '\n') {
			buffer[idx] = '\0';
			idx = 0;
			return 1;
		}
		
		if(idx < buffer_size - 1) {
			buffer[idx++] = c;
		}
	}
	return 0;
}

void process_uart_command(char* command) {
	if(strncmp(command, "MODE:", 5) == 0) {
		uint8_t mode = atoi(command + 5);
		if(mode < NUM_MODES) {
			current_mode = mode;
			eeprom_playback_pos = 0;
		}
	}
	else if(strncmp(command, "MOVE:", 5) == 0) {
		char* comma = strchr(command + 5, ',');
		if(comma) {
			*comma = '\0';
			uint8_t servo = atoi(command + 5);
			uint16_t pos = atoi(comma + 1);
			if(servo < NUM_SERVOS) {
				servo_set_position(servo, pos);
			}
		}
	}
	else if(strncmp(command, "SAVE:", 5) == 0) {
		uint8_t pos = atoi(command + 5);
		if(pos < NUM_POSITIONS) {
			eeprom_save_position(pos);
		}
	}
	else if(strncmp(command, "LOAD:", 5) == 0) {
		uint8_t pos = atoi(command + 5);
		if(pos < NUM_POSITIONS) {
			eeprom_load_position(pos);
		}
	}
	else if(strcmp(command, "STATUS") == 0) {
		uart_send_servo_positions();
	}
}
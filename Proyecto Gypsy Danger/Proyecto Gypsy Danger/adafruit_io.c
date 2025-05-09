/*
 * adafruit_io.c
 *
 * Created: 09/05/2025 04:55:31 p. m.
 *  Author: Admin
 */ 

#include "adafruit_io.h"
#include "servo.h"
#include "eeprom_manager.h"
#include "uart_comm.h"
#include <string.h>

void adafruit_io_init() {
	// Inicialización de módulo WiFi/Serial
	// (Dependerá del módulo usado, aquí un ejemplo genérico)
}

void adafruit_io_process() {
	char buffer[64];
	
	// Simulación de recepción de comandos desde Adafruit IO
	if(uart_receive_command(buffer, sizeof(buffer))) {
		if(strncmp(buffer, "MODE:", 5) == 0) {
			uint8_t mode = buffer[5] - '0';
			// Cambiar modo (implementar según necesidades)
		}
		else if(strncmp(buffer, "MOVE:", 5) == 0) {
			// Formato: MOVE:SERVO,POS
			uint8_t servo = buffer[5] - '0';
			uint16_t pos = atoi(buffer + 7);
			servo_set_position(servo, pos);
		}
		else if(strncmp(buffer, "SAVE:", 5) == 0) {
			uint8_t pos = buffer[5] - '0';
			eeprom_save_position(pos);
		}
		else if(strncmp(buffer, "LOAD:", 5) == 0) {
			uint8_t pos = buffer[5] - '0';
			eeprom_load_position(pos);
		}
	}
}

void adafruit_io_send_status() {
	char buffer[64];
	
	// Enviar posiciones actuales
	for(uint8_t i = 0; i < NUM_SERVOS; i++) {
		sprintf(buffer, "POS:%d,%d\n", i, servo_get_position(i));
		uart_send_string(buffer);
	}
	
	// Enviar modo actual
	// (Implementar según lógica de modos)
}
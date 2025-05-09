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
#include <util/delay.h>

void uart_init(uint32_t baud_rate) {
	uint16_t ubrr = F_CPU / 16 / baud_rate - 1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void uart_send_byte(uint8_t data) {
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
/*
 * uart.c
 *
 * Created: 22/05/2025 11:16:07 p. m.
 *  Author: Admin
 */ 

#include <avr/io.h>
#include <string.h>
#include "uart.h"

#define F_CPU 16000000UL
#define BAUD_PRESCALE(baud) (((F_CPU / (baud * 16UL))) - 1)

static char uart_buffer[64];
static uint8_t uart_index = 0;

void UART_init(uint16_t ubrr) {
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_send_char(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_send_string(const char *str) {
	while (*str) {
		UART_send_char(*str++);
	}
}

uint8_t UART_read_line(char *buffer, uint8_t max_len) {
	while (UCSR0A & (1 << RXC0)) {
		char c = UDR0;

		if (c == '\r' || c == '\n') {
			if (uart_index > 0) {
				uart_buffer[uart_index] = '\0';
				strncpy(buffer, uart_buffer, max_len);
				uart_index = 0;
				UART_send_string("ECO: ");
				UART_send_string(buffer);
				UART_send_string("\r\n");
				return 1;
			}
			} else {
			if (uart_index < sizeof(uart_buffer) - 1) {
				uart_buffer[uart_index++] = c;
			}
		}
	}
	return 0;
}

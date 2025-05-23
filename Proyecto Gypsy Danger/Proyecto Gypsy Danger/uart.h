/*
 * uart.h
 *
 * Created: 22/05/2025 11:16:19 p. m.
 *  Author: Admin
 */ 

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

void UART_init(uint16_t ubrr);
void UART_send_char(char data);
void UART_send_string(const char *str);
uint8_t UART_read_line(char *buffer, uint8_t max_len);

#endif /* UART_H_ */
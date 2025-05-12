/*
 * uart_comm.h
 *
 * Created: 08/05/2025 03:36:58 p. m.
 *  Author: Admin
 */ 


#ifndef UART_COMM_H_
#define UART_COMM_H_

#include <stdint.h>

// Declarar funciones públicas
void uart_init(uint32_t baud_rate);
void uart_send_string(const char* str);
void uart_send_servo_positions();
uint8_t uart_receive_command(char* buffer, uint8_t buffer_size);
void process_uart_command(char* command);

// Declarar funciones de disponibilidad de datos (deberían estar en uart_comm.c)
uint8_t uart_available(void);
uint8_t uart_read_line(char* buffer, uint8_t buffer_size);

#endif /* UART_COMM_H_ */
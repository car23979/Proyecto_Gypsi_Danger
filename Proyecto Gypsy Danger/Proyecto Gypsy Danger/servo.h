/*
 * servo.h
 *
 * Created: 08/05/2025 03:28:59 p. m.
 *  Author: Admin
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

// Inicialización de Timer1 y Timer2

void servo_init();
void servo_set_position(servo_channel_t channel, uint16_t position);
uint16_t servo_get_position(servo_channel_t channel);

// Declarar las funciones que se usan internamente
void servo_update(servo_channel_t channel);
void servo_update_all();

#endif /* SERVO_H_ */
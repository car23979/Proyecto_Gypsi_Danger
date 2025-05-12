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

// Mover un servo dado el pin físico (usa ADC para determinar posición)
void servo_set_position(uint8_t pin, uint16_t adc_value);

// Internamente usada por el software PWM para los servos de cabeza
void servo_software_pwm_update(void);

#endif /* SERVO_H_ */
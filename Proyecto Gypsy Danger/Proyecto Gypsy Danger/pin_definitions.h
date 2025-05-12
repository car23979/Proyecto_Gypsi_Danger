/*
 * pin_definitions.h
 *
 * Created: 08/05/2025 03:37:57 p. m.
 *  Author: Admin
 */ 


#ifndef PIN_DEFINITIONS_H_
#define PIN_DEFINITIONS_H_

// Pines de servomotores
#define SERVO_HEAD_H_PIN	PD5
#define SERVO_HEAD_V_PIN	PD6
#define SERVO_ARM_L_PIN		PB1
#define	SERVO_ARM_R_PIN		PB2

// Canales ADC para joysticks
#define JOY_HEAD_H_ADC		0
#define	JOY_HEAD_V_ADC		1
#define	JOY_ARM_L_ADC		2
#define	JOY_ARM_R_ADC		3

// Botones
#define BTN_SAVE_PIN		PD2
#define BTN_MODE_PIN		PD3

// LED
#define LED_R_PIN			PB3
#define LED_G_PIN			PC1
#define LED_B_PIN			PC0

// Estados del LED RGB
#define LED_OFF        0, 0, 0
#define LED_RED        255, 0, 0
#define LED_GREEN      0, 255, 0
#define LED_BLUE       0, 0, 255
#define LED_YELLOW     255, 255, 0
#define LED_MAGENTA    255, 0, 255
#define LED_CYAN       0, 255, 255
#define LED_WHITE      255, 255, 255


#endif /* PIN_DEFINITIONS_H_ */
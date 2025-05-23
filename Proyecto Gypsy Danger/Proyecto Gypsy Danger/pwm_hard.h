/*
 * pwm_hard.h
 *
 * Created: 16/05/2025 03:25:20 p. m.
 *  Author: Admin
 */ 


#ifndef PWM_HARD_H_
#define PWM_HARD_H_

void configurar_pwm_hardware(void);
void actualizar_PB1(uint16_t us);
void actualizar_PB2(uint16_t us);

#endif /* PWM_HARD_H_ */
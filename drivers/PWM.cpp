/*
 * PWM.cpp
 *
 *  Created on: 20 лип. 2016
 *      Author: alfons-14
 */

#include <avr/io.h>
#include "PWM.h"
#include "../libs/pin_macros.h"

#define PWM1p B, 1, H
#define PWM2p B, 2, H

void PWM_init(){
	//configure TIMER1 - fast pwm 8bit
	//OC1x - non inverting, pin mode out
	TCCR1A = (1 << COM1A1)| (0 << COM1A0)|(1 << COM1B1)| (0 << COM1B0)|(0 << WGM11)|(1 << WGM10);
	TCCR1B = (0 << WGM13)| (1 << WGM12)|(0 << CS12)| (1 << CS11)|(0 << CS10);
	DRIVER(PWM1p, OUT);
	DRIVER(PWM2p, OUT);
}

//void PWM1(uint8_t val){
//	OCR1A = val;
//}

//void PWM2(uint8_t val){
//	OCR1B = val;
//}

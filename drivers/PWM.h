/*
 * PWM.h
 *
 *  Created on: 20 лип. 2016
 *      Author: alfons-14
 */

#ifndef PWM_H_
#define PWM_H_

//uses timer1

//PWM1 port B pin 1
//PWM2 port B pin 2

void PWM_init();
#define PWM1 OCR1A
#define PWM2 OCR1B


#endif /* PWM_H_ */

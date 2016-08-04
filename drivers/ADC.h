/*
 * ADC.h
 *
 *  Created on: 4 лип. 2016
 *      Author: alfons-14
 */

#ifndef ADC_H_
#define ADC_H_

//ADC inputs
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

//ref
#define AREF	0b00
#define A2V56	0b11
#define AVCC	0b01

#define INPUT 0
#define REF 6

uint16_t ADC_get_result();
uint16_t ADC_get_result_from(uint8_t);
void ADC_set_input(uint8_t);
void ADC_set_ref(uint8_t);
void ADC_init();



#endif /* ADC_H_ */

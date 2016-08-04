/*
 * ADC.cpp
 *
 *  Created on: 4 лип. 2016
 *      Author: alfons-14
 */

#include "ADC.h"
#include <avr/io.h>

#define ADC_START_CONV 	ADCSRA|=(1<<ADSC)
#define ADC_ENABLE		ADCSRA|=(1<<ADEN)|(0b110<<ADPS0)
#define ADC_RUNNING		(!(ADCSRA&(1<<ADIF)))


void ADC_init(){
	//input 0
	//AREF
	ADMUX = (AREF << REF) | (0 << INPUT);
	ADC_ENABLE;
}

uint16_t ADC_get_result(){
	ADC_START_CONV;
	while(ADC_RUNNING);
	return ADCW;
}
uint16_t ADC_get_result_from(uint8_t input){
	ADC_set_input(input);
	return ADC_get_result();
}

void ADC_set_input(uint8_t input){
	ADMUX&=0b11110000; //clear input bits
	ADMUX|=(input<<INPUT);
}

void ADC_set_ref(uint8_t ref){
	ADMUX&=0b00111111; //clear ref bits
	ADMUX|=(ref<<REF);

}

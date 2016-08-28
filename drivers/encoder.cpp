/*
 * encoder.cpp
 *
 *  Created on: 16 лип. 2016
 *      Author: alfons-14
 */

#include "encoder.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "../libs/pin_macros.h"


uint8_t states[]={3,2,0,1,3,2,0,1};
uint8_t pos=4;

volatile int8_t state=0;

void encoder_init(){
	DRIVER(P1, IN);
	DRIVER(P1, PULLUP);
	DRIVER(P2, IN);
	DRIVER(P2, PULLUP);
	//port d
	PCMSK2 = (1 << PCINT19) | (1 << PCINT18);
	PCICR = (1 << PCIE2);
}

int8_t encoder_get_state(){
	return state;
}

void encoder_reset(){
	state=0;
}

ISR(PCINT2_vect){
	PCICR &= ~(1 << PCIE2);
	uint8_t a=((bool)ACTIVE(P2)<<1)|((bool)ACTIVE(P1));
	if(states[((pos-1)%4)]==a) {pos--;}
	if(states[((pos+1)%4)]==a) {pos++;}
	switch (pos){
	case 0: state--; pos=4; break;
	case 8: state++; pos=4;
	}
	PCICR |= (1 << PCIE2);
}

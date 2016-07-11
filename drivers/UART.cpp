/*
 * UART.cpp
 *
 *  Created on: 18 трав. 2016
 *      Author: alfons-14
 */
#include <avr/io.h>
#include "../drivers/UART.h"

void UART_init (){
	UBRRH = HI(bauddivider(baudrate));
	UBRRL = LO(bauddivider(baudrate));
	UCSRC = ( 1 << URSEL ) | ( 1 << UCSZ1 ) | ( 1 << UCSZ0 );
	UCSRB = ( 1 << TXEN ) | ( 1 << RXEN );
}

void UART_send(char a){ // single symvol
	while( ( UCSRA & ( 1 << UDRE ) ) == 0 );
	UDR = a;
}

void UART_send(char *s){ //string
    unsigned char c;
    while( ( c = *s++ ) != 0 ) {
        UART_send( c );
    }
}




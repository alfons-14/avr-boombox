/*
 * UART.cpp
 *
 *  Created on: 18 трав. 2016
 *      Author: alfons-14
 */
#include <avr/io.h>
#include "../drivers/UART.h"

void UART_init (){
	UBRR0H = HI(bauddivider(baudrate));
	UBRR0L = LO(bauddivider(baudrate));
	UCSR0C = ( 1 << UCSZ01 ) | ( 1 << UCSZ00 );
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 );
}

void UART_send(char a){ // single char
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = a;
}

void UART_send(char *s){ //string
    unsigned char c;
    while( ( c = *s++ ) != 0 ) {
        UART_send( c );
    }
}




/*
 * UART.h
 *
 *  Created on: 18 трав. 2016
 *      Author: alfons-14
 */

#ifndef UART_H_
#define UART_H_

#define HI(x) ((x)>>8)
#define LO(x) ((x)&0xFF)

//volatile char RX[20], TX[20], TX_pos, RX_pos;

#define baudrate 9600L
#define bauddivider(x) (F_CPU/(16*x))-1

void UART_init();
void UART_send(char);
void UART_send(char*);


#endif /* UART_H_ */

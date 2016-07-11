/*
 * TWI.cpp
 *
 *  Created on: 16 трав. 2016
 *      Author: alfons-14
 */
#include "TWI.h"
#include <avr/interrupt.h>

#if TWI_UART_DEBUG==1
#include "UART.h"
#include <stdio.h>
#endif

volatile uint8_t TWI_ADRESS;
volatile union TWI_state TWI = {0};
volatile uint8_t *TWI_DATA;
volatile uint8_t TWI_BYTE;
volatile uint8_t TWI_DATA_COUNT;
volatile uint8_t TWI_DATA_POS;

#if TWI_UART_DEBUG==1
char buf[20];
#endif

void TWI_init(){
	TWI.busy=0;
	TWBR = ((F_CPU / TWI_FREQ) - 16) / (2 * 1);
	TWI_ENABLE;
}

void TWI_send(uint8_t adress, uint8_t *data, uint8_t data_count){
	while(TWI.busy);
	uint8_t sreg;
	sreg = SREG;
	TWI_ADRESS = (adress << 1) | TWI_WRITE;
	TWI_DATA = data;
	TWI_DATA_COUNT = data_count;
	TWI.busy = 1;
	TWI_START;
	SREG = sreg;
}

void TWI_send_byte(uint8_t adress, uint8_t byte){
	while(TWI.busy);
	uint8_t sreg;
	sreg = SREG;
	cli();
	TWI_ADRESS = (adress << 1) | TWI_WRITE;
	TWI_BYTE = byte;
	TWI_DATA_COUNT = 1;
	TWI.busy = 1;
	TWI_START;
	SREG = sreg;
}

void TWI_receive(uint8_t adress, uint8_t *data, uint8_t data_count){
	while(TWI.busy);
	uint8_t sreg;
	sreg = SREG;
	cli();
	TWI_ADRESS = (adress << 1) | TWI_READ;
	TWI_DATA = data;
	TWI_DATA_COUNT = data_count;
	TWI.busy = 1;
	TWI_START;
	SREG = sreg;
}

void TWI_repeat(){
	while(TWI.busy);
	uint8_t sreg;
	sreg = SREG;
	cli();
	TWI.busy = 1;
	TWI_START;
	SREG = sreg;

}

ISR(TWI_vect){
    if(TWI_ADRESS & TWI_READ){
    	//read
    	switch(TWSR){

    	case TWI_START_:
    	case TWI_RESTART_:
#if TWI_UART_DEBUG==1
    		sprintf(buf, "Recieve from 0x%X ", TWI_ADRESS>>1);
    		UART_send(buf);
#endif
    		TWI_DATA_POS=0;
    		TWDR = TWI_ADRESS;
    		TWI_SEND;
    		break;

    	case TWI_SLA_R_ACK_:
    		if (TWI_DATA_COUNT == 1){
    			TWI_RECIEVE_LAST;
    		}
    		else{
    			TWI_RECIEVE;
    		}
    		break;

    	case TWI_SLA_R_NACK_:
#if TWI_UART_DEBUG==1
    		sprintf(buf, "failed: SLA+R+NACK\r\n");
    		UART_send(buf);
#endif
    		TWI.errorCode = TWSR>>2;
    		TWI.lastOpOK = 0;
    		TWI.busy = 0;
    		TWI_STOP;
    		break;

    	case TWI_RECIEVE_BYTE_:
    		TWI_DATA[TWI_DATA_POS] = TWDR;
#if TWI_UART_DEBUG==1
    		sprintf(buf, "%d ", TWDR);
    		UART_send(buf);
#endif
    		if (++TWI_DATA_POS+1 < TWI_DATA_COUNT){
    			TWI_RECIEVE;
    		}
    		else{
    			TWI_RECIEVE_LAST;
    		}
    		break;

    	case TWI_RECIEVE_BYTE_NACK_:
#if TWI_UART_DEBUG==1
    		sprintf(buf, "%d stopped", TWDR);
    		UART_send(buf);
#endif
    		TWI_DATA[TWI_DATA_POS] = TWDR;
    		TWI_STOP;
    		TWI.lastOpOK = 1;
    		TWI.busy = 0;
    		break;
    	}
    }
    else{
    	//write
    	switch(TWSR){

    	case TWI_START_:
    	case TWI_RESTART_:
#if TWI_UART_DEBUG==1
    		sprintf(buf, "Send to 0x%X ", TWI_ADRESS>>1);
    		UART_send(buf);
#endif
    		TWI_DATA_POS=0;
    		TWDR = TWI_ADRESS;
    		TWI_SEND;
    		break;

    	case TWI_SLA_W_ACK_:
    		if (TWI_DATA_COUNT==1) TWDR = TWI_BYTE; else
    		TWDR = TWI_DATA[0];
#if TWI_UART_DEBUG==1
    		sprintf(buf, "%d ", TWDR);
    		UART_send(buf);
#endif
    		TWI_SEND;
    		break;

    	case TWI_SLA_W_NACK_:
#if TWI_UART_DEBUG==1
    		sprintf(buf, "failed: SLA+W+NACK\r\n");
    		UART_send(buf);
#endif
    		TWI.errorCode = TWSR>>2;
    		TWI.lastOpOK = 0;
    		TWI.busy = 0;
    		TWI_STOP;
    		break;

    	case TWI_BYTE_ACK_:
    		if	(++TWI_DATA_POS < TWI_DATA_COUNT){
    			TWDR = TWI_DATA[TWI_DATA_POS];
#if TWI_UART_DEBUG==1
    		sprintf(buf, "%d ", TWDR);
    		UART_send(buf);
#endif
    			TWI_SEND;
    			break;
    		}
#if TWI_UART_DEBUG==1
    		sprintf(buf, "stoped\r\n");
    		UART_send(buf);
#endif
    		TWI_STOP;
    		TWI.lastOpOK = 1;
    		TWI.busy = 0;
    		break;

    	case TWI_BYTE_NACK_:
#if TWI_UART_DEBUG==1
    		sprintf(buf, "failed: BYTE+NACK\r\n");
    		UART_send(buf);
#endif
    		TWI.errorCode = TWSR>>2;
    		TWI.lastOpOK = 0;
    		TWI.busy = 0;
    		TWI_STOP;
    		break;
    	}
    }

}



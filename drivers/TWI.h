/*
 * TWI.h
 *
 *  Created on: 16 трав. 2016
 *      Author: alfons-14
 */

#ifndef TWI_H_
#define TWI_H_

#define TWI_FREQ 100000UL
#define TWI_UART_DEBUG	0 //uses sprintf and very slow
typedef unsigned char uint8_t;

//actions
#define TWI_ENABLE 				TWCR = (0<<TWSTA)|(0<<TWSTO)|(0<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE)
#define TWI_START 				TWCR = (1<<TWSTA)|(0<<TWSTO)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE)
#define TWI_STOP 				TWCR = (0<<TWSTA)|(1<<TWSTO)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE)
#define TWI_SEND 				TWCR = (0<<TWSTA)|(0<<TWSTO)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE)
#define TWI_RECIEVE 			TWCR = (0<<TWSTA)|(0<<TWSTO)|(1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE)
#define TWI_RECIEVE_LAST		TWCR = (0<<TWSTA)|(0<<TWSTO)|(1<<TWINT)|(0<<TWEA)|(1<<TWEN)|(1<<TWIE)

//states for master
#define TWI_BUS_FAIL_			0x00
#define TWI_START_				0x08
#define TWI_RESTART_			0x10

//write
#define TWI_SLA_W_ACK_			0x18
#define TWI_SLA_W_NACK_			0x20
#define TWI_BYTE_ACK_			0x28
#define TWI_BYTE_NACK_			0x30

#define TWI_COLLISION_			0x38

//read
#define TWI_SLA_R_ACK_			0x40
#define TWI_SLA_R_NACK_			0x48
#define TWI_RECIEVE_BYTE_		0x50
#define TWI_RECIEVE_BYTE_NACK_	0x58

#define TWI_WRITE				0
#define TWI_READ				1

union TWI_state
{
	uint8_t all;
    struct
    {
    	uint8_t busy:1;
        uint8_t lastOpOK:1;
        uint8_t errorCode:6;

    };
};

extern volatile union TWI_state TWI;

void TWI_init();
void TWI_send(uint8_t, uint8_t*, uint8_t);
void TWI_send_byte(uint8_t, uint8_t);
void TWI_receive(uint8_t, uint8_t*, uint8_t);
void TWI_repeat();
bool TWI_check_for(uint8_t);

#endif /* TWI_H_ */

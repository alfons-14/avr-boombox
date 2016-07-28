/*
 * RTC.cpp
 *
 *  Created on: 7 лип. 2016
 *      Author: alfons-14
 */

#include "RTC.h"
#include "../drivers/TWI.h"

void RTC::setHour(uint8_t _hour) {
	hour.h = _hour / 10;
	hour.l = _hour % 10;
}

void RTC::setMin(uint8_t _min) {
	min.h = _min / 10;
	min.l = _min % 10;
}

void RTC::setSec(uint8_t _sec) {
	sec.h = _sec / 10;
	sec.l = _sec % 10;
}

RTC::RTC(uint8_t _addr) {
	addr = _addr;
}

void RTC::setTime() {
	uint8_t tx[] = { 0, sec.byte, min.byte, hour.byte };
	TWI_send(addr, tx, 4);
}

void RTC::update() {
	TWI_send_byte(addr, 0);
	TWI_receive(addr, rx, 7);
	while (TWI.busy);
	sec.byte = rx[0];
	min.byte = rx[1];
	hour.byte = rx[2];
	day = rx[3];
	date.byte = rx[4];
	century = rx[5]>>8;
	mounth.byte = rx[5]&0b01111111;
	year.byte = rx[6];
}


/*
 * TDA7439.cpp
 *
 *  Created on: 16 трав. 2016
 *      Author: alfons-14
 */
#include "TDA7439.h"
#include "../drivers/TWI.h"

uint8_t t[] = { 0, 1, 2, 3, 4, 5, 6, 7, 14, 13, 12, 11, 10, 9, 8 };

#define VOL(x) -x
#define TONE(x) t[x+7]

TDA7439::TDA7439() {
	gain = 0;
	input = IN2;
	tone.bass = 0;
	tone.middle = 0;
	tone.treble = 0;
	attL = 0;
	attR = 0;
	volume = 0;
	upload();
}

void TDA7439::upload() {
	tx[0] = INC_ADR;
	tx[GAIN + 1] = gain / 2;
	tx[IN + 1] = (~input) & 0b11;
	tx[BASS + 1] = TONE(tone.bass);
	tx[MIDDLE + 1] = TONE(tone.middle);
	tx[TREBLE + 1] = TONE(tone.treble);
	tx[VOLUME + 1] = VOL(volume);
	tx[ATTL + 1] = VOL(attL);
	tx[ATTR + 1] = VOL(attR);
	TWI_send(TDA7439ADDR, tx, 9);
}

void TDA7439::setInput(uint8_t _input) {
	input = _input;
}


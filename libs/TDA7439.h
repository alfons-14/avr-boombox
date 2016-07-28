/*
 * TDA7439.h
 *
 *  Created on: 16 трав. 2016
 *      Author: alfons-14
 */

#ifndef TDA7439_H_
#define TDA7439_H_

#include <inttypes.h>

#define TDA7439ADDR		0b01000100

#define VOLUME			0b00000010
#define IN				0b00000000
#define GAIN			0b00000001
#define BASS			0b00000011
#define TREBLE			0b00000101
#define MIDDLE			0b00000100
#define ATTL			0b00000111
#define ATTR			0b00000110
#define MUTE
#define IN1				0
#define IN2				1
#define IN3				2
#define IN4				3

#define INC_ADR			0b00010000

union tone_t {
	uint16_t all;
	struct {
		int8_t bass :4;
		int8_t treble :4;
		int8_t middle :4;
	};
};

class TDA7439 {
public:
	int8_t volume;
	int8_t attR;
	int8_t attL;
	uint8_t gain;
	uint8_t input;
	union tone_t tone;
	void upload();
	void setInput(uint8_t);
	void mute();
	void unmute();
	void att(uint8_t, uint8_t);
	TDA7439();
private:
	uint8_t tx[9];
};

#endif /* TDA7439_H_ */

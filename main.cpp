/*
 * main.cpp
 *
 *  Created on: 16 трав. 2016
 *      Author: alfons-14
 */
#include <avr/io.h>
#include "scmRTOS/core/scmRTOS.h"
#include "libs/pin_macros.h"
#include <util/delay.h>
#include "drivers/UART.h"
#include "drivers/TWI.h"
#include "libs/TDA7439.h"
#include "libs/LCD.h"
#include "drivers/encoder.h"
#include <stdlib.h>
#include "libs/TEA5767N.h"
#include "drivers/PWM.h"
#include "libs/RTC.h"

typedef OS::process<OS::pr0, 50> p1;
typedef OS::process<OS::pr1, 100> p2;
typedef OS::process<OS::pr2, 100> p3;

p2 hard;
p1 adc;
p3 LCD;

TDA7439 audio = TDA7439();
TEA5767N radio = TEA5767N();
RTC rtc = RTC(0x68); //ds3231

//TDA7439 inputs
//3.5mm
#define IN3_5 IN2
//6.3mm
#define IN6_3 IN3
//fm radio
#define INFM IN1
//bluetooth
#define INBL IN4

volatile bool tuning = false;
volatile bool tuning_input = false;
volatile uint8_t setting = 0; //volume, bass, etc...
volatile float freq = 107.0;
volatile uint8_t sig;
volatile uint16_t button;
volatile bool pressed = false;
volatile bool longpress = false;

uint8_t getADC() {
	ADCSRA |= 1 << ADSC;
	while (!(ADCSRA & (1 << ADIF)));
	if (ADC < 200)
		return 0;
	if (ADC < 400)
		return 1;
	if (ADC < 500)
		return 2;
	if (ADC < 550)
		return 3;
	if (ADC < 620)
		return 4;
	if (ADC < 700)
		return 5;
	if (ADC < 760)
		return 6;
	if (ADC < 830)
		return 7;
	if (ADC < 900)
		return 8;
	if (ADC < 980)
		return 9;
	return 10;
}

char chars[][8] { { 0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001,
		0b10001, 0b11111 }, { 0b01110, 0b11111, 0b10001, 0b10001, 0b10001,
		0b10001, 0b11111, 0b11111 }, { 0b01110, 0b11111, 0b10001, 0b10001,
		0b10001, 0b11111, 0b11111, 0b11111 }, { 0b01110, 0b11111, 0b10001,
		0b10001, 0b11111, 0b11111, 0b11111, 0b11111 }, { 0b01110, 0b11111,
		0b10001, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 }, { 0b01110,
		0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 },
		{ 0b00011, 0b00110, 0b01100, 0b11111, 0b11111, 0b00110, 0b01100, 0b11000 },
		{ 0b10101, 0b10101, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 } };

int main() {
	UART_init();
	TWI_init();
	ADMUX = 0b00000000; //ADC_init
	ADCSRA = 0b10000101;
	PWM_init();
	PWM1=50;
	encoder_init();
	enable_interrupts();
	LCD_init();
	char i;
	for (i = 1; i < 9; i++)
		LCD_create_char(i, &chars[i - 1][0]);
	LCD_go_to_xy(15, 0);
	LCD_print(5);
	LCD_backlight(1);
	radio.setHighCutControlOn();
	radio.setStereoNoiseCancellingOn();
	radio.selectFrequency(freq);
	radio.setSearchLowStopLevel();
	TIMER0_CS_REG = (1 << CS01) | (1 << CS00); // clk/64
	TIMER0_IE_REG |= (1 << TOIE0);
	OS::run();
	//LCD_go_to_xy(14,0);
	//LCD_print(7);
	//LCD_print(i+1);
	//i=(i+1)%6;
	//sleep(100);
}

namespace OS {
template<> OS_PROCESS void p1::exec() {
	for (;;) {
		rtc.update();
		sleep(1000);
	}
}
}
namespace OS { //read buttons and update hardware: TDA7439, TEA5767
template<> OS_PROCESS void p2::exec() {
	uint_fast32_t timeout = 0;
	uint8_t adc;
	for (;;) {
		adc = getADC();
		if (adc == 10)
			pressed = false;
		else {
			button++;
			pressed = true;
		}
		switch (adc) {
		case 0:
		case 1:
		case 2:
		case 3:
			setting = adc;
			timeout = get_tick_count() + 1000;
			tuning = true;
			tuning_input = false;
			break;
		case 4:
			timeout = get_tick_count() + 1000;
			tuning_input = true;
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
//			if(!pressed && button!=0){
//			if(button<10){
//				freq-=.1;
//				radio.selectFrequency(freq);
//			}
//			else{
			radio.setSearchDown();
			radio.searchNextMuting();
			freq = radio.readFrequencyInMHz();
//			}
//			}
			break;
		case 9:
//			if(!pressed && button!=0){
//			if(button<10){
//				freq+=.1;
//				radio.selectFrequency(freq);
//			}
//			else{
			radio.setSearchUp();
			radio.searchNextMuting();
			freq = radio.readFrequencyInMHz();
//			}
//			}
			break;
		}
		if (encoder_get_state() != 0) {
			timeout = get_tick_count() + 1000;
			if (tuning && !tuning_input) {
				switch (setting) {
				case 0:
					audio.volume += encoder_get_state();
					if (audio.volume > 0)
						audio.volume = 0;
					if (audio.volume < -47)
						audio.volume = -47;
					break;
				case 1:
					if (audio.tone.bass + encoder_get_state() < -7)
						audio.tone.bass = -7;
					else if (audio.tone.bass + encoder_get_state() > 7)
						audio.tone.bass = 7;
					else
						audio.tone.bass += encoder_get_state();
					break;
				case 2:
					if (audio.tone.middle + encoder_get_state() < -7)
						audio.tone.middle = -7;
					else if (audio.tone.middle + encoder_get_state() > 7)
						audio.tone.middle = 7;
					else
						audio.tone.middle += encoder_get_state();
					break;
				case 3:
					if (audio.tone.treble + encoder_get_state() < -7)
						audio.tone.treble = -7;
					else if (audio.tone.treble + encoder_get_state() > 7)
						audio.tone.treble = 7;
					else
						audio.tone.treble += encoder_get_state();
					break;
				}
			} else if (tuning_input)
				audio.input = (audio.input + encoder_get_state()) & 0b11;
			encoder_reset();
			tuning = true;
			audio.upload();
		}
		if (timeout == get_tick_count()) {
			setting = 0;
			tuning = false;
			tuning_input = false;
		}
		if (button > 10)
			longpress = true;
		else
			longpress = false;
		if (!pressed)
			button = 0;
		sleep(5);
	}
}
}

namespace OS { //LCD update
template<> OS_PROCESS void p3::exec() {
	char buf[16];
	uint8_t i;
	for (;;) {
		LOCK_SYSTEM_TIMER();
		LCD_go_to_xy(0,0);
//		if (!pressed){
//		if(longpress) LCD_print("LP");
//		else LCD_print("SP");}
//		else LCD_print("p");
//		LCD_print(utoa(pressed, buf, 10));
//		LCD_print(" ");
//		LCD_print(utoa(button, buf, 10));
//		LCD_print("      ");
		LCD_print(rtc.hour.h+'0');
		LCD_print(rtc.hour.l+'0');
		LCD_print(':');
		LCD_print(rtc.min.h+'0');
		LCD_print(rtc.min.l+'0');
		UNLOCK_SYSTEM_TIMER();
		LOCK_SYSTEM_TIMER();
		LCD_go_to_xy(0, 1);
		if (tuning && !tuning_input) {
			switch (setting) {
			case 0:
				LCD_print("Volume: ");
				LCD_print(itoa(audio.volume, buf, 10));
				LCD_print(" db    ");
				break;
			case 1:
				LCD_print("Bass: ");
				if (audio.tone.bass > 0)
					LCD_print('+');
				LCD_print(itoa(2 * audio.tone.bass, buf, 10));
				LCD_print(" db      ");
				break;
			case 2:
				LCD_print("Middle: ");
				if (audio.tone.middle > 0)
					LCD_print('+');
				LCD_print(itoa(2 * audio.tone.middle, buf, 10));
				LCD_print(" db     ");
				break;
			case 3:
				LCD_print("Treble: ");
				if (audio.tone.treble > 0)
					LCD_print('+');
				LCD_print(itoa(2 * audio.tone.treble, buf, 10));
				LCD_print(" db     ");
				break;
			}
		} else if (tuning_input) {
			switch (audio.input) {
			case IN3_5:
				LCD_print("1: 3.5mm        ");
				break;
			case IN6_3:
				LCD_print("2: 6.3mm        ");
				break;
			case INBL:
				LCD_print("3: Bluetooth    ");
				break;
			case INFM:
				LCD_print("4: FM radio     ");
				break;
			}
		} else {
			switch (audio.input) {
			case IN3_5:
				LCD_print("3.5mm           ");
				break;
			case IN6_3:
				LCD_print("6.3mm           ");
				break;
			case INBL:
				LCD_print("Bluetooth       ");
				break;
			case INFM:
				if (!((i++) % 7))
					sig = radio.getSignalLevel(); //check signal 1 time per 7 updates of LCD
				LCD_print(utoa((uint8_t) freq, buf, 10));
				LCD_print('.');
				LCD_print(utoa((uint8_t) (freq * 100 + 5) / 10 % 10, buf, 10));
				LCD_print(" MHz    ");
				LCD_go_to_xy(13, 1);
				LCD_print(8);
				if (sig < 10)
					LCD_print('0');
				LCD_print(utoa(sig, buf, 10));
				break;
			}
		}
		UNLOCK_SYSTEM_TIMER();
		sleep(10);
	}
}
}

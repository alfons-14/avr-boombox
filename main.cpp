/*
 * main.cpp
 *
 *  Created on: 8 серп. 2016 р.
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
#include "drivers/ADC.h"
#include "libs/jobs.h"
#include <avr/pgmspace.h>
#include "bombox.h"

typedef OS::process<OS::pr0, 100> Tp0;
typedef OS::process<OS::pr1, 100> Tp1;
typedef OS::process<OS::pr2, 100> Tp2;
typedef OS::process<OS::pr3, 100> Tp3;
typedef OS::process<OS::pr4, 500> Tp4;

OS::channel<TJob*, 4> JobHighPr;
OS::channel<TJob*, 4> JobLowPr;

Tp0 p0;
Tp1 p1;
Tp2 p2;
Tp3 p3;
Tp4 p4;


TDA7439 audio = TDA7439();
TEA5767N radio = TEA5767N();
RTC rtc = RTC(0x68); //ds3231


TLCDTimeUpdate LCDTimeUpdate(&rtc);
TLCDBattUpdate LCDBattUpdate(true);
TLCDSecLineUpdate LCDSecLineUpdate(&radio, &audio);
TLCDFMSigUpdate LCDFMSigUpdate(&radio);

float freq;

#if PROGMEM_CHARS == 1

const char char1[8] PROGMEM =	{ 0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111 };
const char char2[8] PROGMEM =	{ 0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111, 0b11111 };
const char char3[8] PROGMEM =	{ 0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111 };
const char char4[8] PROGMEM =	{ 0b01110, 0b11111, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111, 0b11111 };
const char char5[8] PROGMEM =	{ 0b01110, 0b11111, 0b10001, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };
const char char6[8] PROGMEM =	{ 0b01110, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };
const char char7[8] PROGMEM =	{ 0b00011, 0b00110, 0b01100, 0b11111, 0b11111, 0b00110, 0b01100, 0b11000 };
const char char8[8] PROGMEM =	{ 0b10101, 0b10101, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 };

const char* const chars[] PROGMEM ={
    char1,
    char2,
    char3,
    char4,
    char5,
    char6,
    char7,
    char8
};

char buf[8];

#else

char chars[][8] = {
	{0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111},
	{0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111, 0b11111},
	{0b01110, 0b11111, 0b10001, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111},
	{0b01110, 0b11111, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111, 0b11111},
	{0b01110, 0b11111, 0b10001, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111},
	{0b01110, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111},
	{0b00011, 0b00110, 0b01100, 0b11111, 0b11111, 0b00110, 0b01100, 0b11000},
	{0b10101, 0b10101, 0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100}
};

#endif

uint8_t getButton() {
	uint16_t res=ADC_get_result_from(2);
	if (res < BUT0)
		return 10;
	if (res < BUT1)
		return 9;
	if (res < BUT2)
		return 8;
	if (res < BUT3)
		return 7;
	if (res < BUT4)
		return 6;
	if (res < BUT5)
		return 5;
	if (res < BUT6)
		return 4;
	if (res < BUT7)
		return 3;
	if (res < BUT8)
		return 2;
	if (res < BUT9)
		return 1;
	return 0;
}

int main(){
  TWI_init();
  enable_interrupts();
  UART_init();
  ADC_init();
  ADC_set_ref(ANALOG_REF);
  LCD_init();
  encoder_init();
  radio.setHighCutControlOff();
  radio.selectFrequency(101.5);
  radio.setStereoNoiseCancellingOn();
  radio.setMonoReception();
  char i;
  for (i = 1; i < 9; i++){
#if PROGMEM_CHARS == 1
      strcpy_P(buf, (char*)pgm_read_word(&(chars[i-1])));
      LCD_create_char(i, buf);
#else
      LCD_create_char(i, &chars[i - 1][0]);
#endif
  }
#if BUTCAL==true
 while(1){
      char buf[10];
      LCD_go_to_xy(0, 0);
      utoa(ADC_get_result_from(2), buf, 10);
      LCD_print(buf);
      LCD_print("    ");
      LCD_go_to_xy(0, 1);
      LCD_print(utoa(getButton(), buf, 10));
      LCD_print("   ");
  }
#endif
  TIMER0_CS_REG = (1 << CS01) | (1 << CS00); // clk/64
  TIMER0_IE_REG |= (1 << TOIE0);
  OS::run();
}


namespace OS {
template<> OS_PROCESS void Tp0::exec() {
	for (;;) {
	    rtc.update();
	    JobLowPr.push(&LCDBattUpdate);
	    JobLowPr.push(&LCDTimeUpdate);
	    if(audio.input==INFM &&LCDSecLineUpdate.mode==LCD_INPUT) JobLowPr.push(&LCDFMSigUpdate);
	    sleep(300);
	}
}
}

namespace OS {
template<> OS_PROCESS void Tp1::exec() {
	for (;;) {
	    TJob *Job;
	    JobHighPr.pop(Job);
	    Job->execute();
	}
}
}

namespace OS {
template<> OS_PROCESS void Tp2::exec() {
	for (;;) {
	    if(encoder_get_state()){
		switch(LCDSecLineUpdate.mode){
		  case LCD_BASS:
		    if(audio.tone.bass+encoder_get_state()<-7) audio.tone.bass=-7;
		    else
		    if(audio.tone.bass+encoder_get_state()>7) audio.tone.bass=7;
		    else audio.tone.bass+=encoder_get_state();
		    break;
		  case LCD_TREBLE:
		    if(audio.tone.treble+encoder_get_state()<-7) audio.tone.treble=-7;
		    else
		    if(audio.tone.treble+encoder_get_state()>7) audio.tone.treble=7;
		    else audio.tone.treble+=encoder_get_state();
		    break;
		  case LCD_MIDDLE:
		    if(audio.tone.middle+encoder_get_state()<-7) audio.tone.middle=-7;
		    else
		    if(audio.tone.middle+encoder_get_state()>7) audio.tone.middle=7;
		    else audio.tone.middle+=encoder_get_state();
		    break;
		  case LCD_INPUT_SELECT:
		    audio.input=(audio.input+encoder_get_state())&0b11;
		    break;
		  default:
		    LCDSecLineUpdate.mode=LCD_VOLUME;
		    audio.volume+=encoder_get_state();
		    if(audio.volume>0) audio.volume=0;
		    if(audio.volume<-47) audio.volume=-47;
		}
		LCDSecLineUpdate.timeout = get_tick_count()+1000;
		JobLowPr.push(&LCDSecLineUpdate);
		encoder_reset();
		audio.upload();
	    }
	    sleep(20);
	}
}
}

namespace OS {
template<> OS_PROCESS void Tp3::exec() {
  uint8_t butt;
  uint_fast32_t time;
  bool pressed=false;
	for (;;) {
	    butt = getButton();
	    if(!pressed){
		switch(butt){
		  case VOL:
		    LCDSecLineUpdate.mode = LCD_VOLUME;
		    break;
		  case BAS:
		    LCDSecLineUpdate.mode = LCD_BASS;
		    break;
		  case MID:
		    LCDSecLineUpdate.mode = LCD_MIDDLE;
		    break;
		  case TRE:
		    LCDSecLineUpdate.mode = LCD_TREBLE;
		    break;
		  case SETB:
		    LCDBattUpdate.charging=!LCDBattUpdate.charging;
		    break;
		  case INP:
		    LCDSecLineUpdate.mode = LCD_INPUT_SELECT;
		    break;
		  case SDW:
		    radio.setSearchDown();
		    radio.searchNextMuting();
		    freq=radio.readFrequencyInMHz();
		    LCDSecLineUpdate.freq = (uint16_t)(freq*100);
		    break;
		  case SUP:
		    radio.setSearchUp();
		    radio.searchNextMuting();
		    freq=radio.readFrequencyInMHz();
		    LCDSecLineUpdate.freq = (uint16_t)(freq*100);
		    break;
		  case NOPRESS: pressed=false;
		    break;
		}
		if(butt){
		    time = get_tick_count();
		    JobLowPr.push(&LCDSecLineUpdate);
		    LCDSecLineUpdate.timeout = get_tick_count()+1000;
		}
		audio.upload();
	    }
	    if (LCDSecLineUpdate.timeout <= get_tick_count()){
		LCDSecLineUpdate.mode=LCD_INPUT;
		JobLowPr.push(&LCDSecLineUpdate);
	    }
	    pressed = bool(butt);
	    sleep(20);
	}
}
}

namespace OS {
template<> OS_PROCESS void Tp4::exec() {
	for (;;) {
	    TJob *Job;
	    JobLowPr.pop(Job);
	    Job->execute();
	}
}
}


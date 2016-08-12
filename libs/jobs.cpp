/*
 * jobs.cpp
 *
 *  Created on: 6 серп. 2016 р.
 *      Author: alfons-14
 */
#include "jobs.h"
#include "LCD.h"
#include <stdlib.h>

TLCDTimeUpdate::TLCDTimeUpdate(RTC* _rtc){
  rtc = _rtc;
}

void TLCDTimeUpdate::execute(){
  LCD_go_to_xy(0,0);
  LCD_print(rtc->hour.h+'0');
  LCD_print(rtc->hour.l+'0');
  LCD_print(':');
  LCD_print(rtc->min.h+'0');
  LCD_print(rtc->min.l+'0');
}

TLCDBattUpdate::TLCDBattUpdate(bool _charging){
  charging = _charging;
}

void TLCDBattUpdate::execute(){
  LCD_go_to_xy(14,0);
  if(charging){
      LCD_print(7);
      LCD_print(level+1);
      level=(level+1)%6;
  }
  else{
  LCD_print(' ');
  LCD_print(level+1);
  }
}

TLCDSecLineUpdate::TLCDSecLineUpdate(TEA5767N* _radio, TDA7439* _audio){
  radio = _radio;
  audio = _audio;
  mode = LCD_INPUT;
  freq=0;
  timeout=0;
}

void TLCDSecLineUpdate::execute(){
  LCD_go_to_xy(0,1);
  switch(mode){
    case LCD_INPUT:
      switch(audio->input){
	case IN35: LCD_print("3.5mm           "); break;
	case IN63: LCD_print("6.3mm           "); break;
	case INBL: LCD_print("Bluetooth       "); break;
	case INFM:
//	  freq = (uint16_t)radio->readFrequencyInMHz()*100;
	  LCD_print(utoa((freq/100), buf, 10));
	  LCD_print('.');
	  LCD_print(utoa((freq+5)/10%10, buf, 10));
	  LCD_print(" MHz    ");
	  break;
      }
      break;
      case LCD_VOLUME:
	LCD_print("Volume: ");
	LCD_print(itoa(audio->volume, buf, 10));
	LCD_print(" db     ");
	break;
    case LCD_BASS:
      LCD_print("Bass: ");
      if(audio->tone.bass>0) LCD_print('+');
      LCD_print(itoa(audio->tone.bass*2, buf, 10));
      LCD_print(" db      ");
      break;
    case LCD_MIDDLE:
      LCD_print("Middle: ");
      if(audio->tone.middle>0) LCD_print('+');
      LCD_print(itoa(audio->tone.middle*2, buf, 10));
      LCD_print(" db   ");
      break;
    case LCD_TREBLE:
      LCD_print("Treble: ");
      if(audio->tone.treble>0) LCD_print('+');
      LCD_print(itoa(audio->tone.treble*2, buf, 10));
      LCD_print(" db      ");
      break;
    case LCD_INPUT_SELECT:
      LCD_print("IN: ");
      switch(audio->input){
	case IN35:
	  LCD_print("3.5mm       ");
	  break;
	case IN63:
	  LCD_print("6.3mm       ");
	  break;
	case INBL:
	  LCD_print("Bluetooth   ");
	  break;
	case INFM:
	  LCD_print("FM radio    ");
	  break;
      }
      break;
  }
}

TLCDFMSigUpdate::TLCDFMSigUpdate(TEA5767N* _radio){
  radio = _radio;
  level = 0;
}

void TLCDFMSigUpdate::execute(){
  LCD_go_to_xy(13,1);
  LCD_print(8);
  level=radio->getSignalLevel();
  LCD_print(level/10+'0');
  LCD_print(level%10+'0');
}

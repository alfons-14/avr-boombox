/*
 * jobs.cpp
 *
 *  Created on: 6 серп. 2016 р.
 *      Author: alfons-14
 */
#include "jobs.h"
#include "LCD.h"

TLCDTimeUpdate::TLCDTimeUpdate(RTC* _rtc){
  rtc = _rtc;
}

void TLCDTimeUpdate::execute(){
  LCD_go_to_xy(0,0);
  LCD_print(rtc->hour.h);
  LCD_print(rtc->hour.l);
  LCD_print(':');
  LCD_print(rtc->min.h);
  LCD_print(rtc->min.l);
}

TLCDBattUpdate::TLCDBattUpdate(bool _charging){
  charging = _charging;
}

void TLCDBattUpdate::execute(){
  LCD_go_to_xy(14,0);
  if(charging){
      LCD_print(8);
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
}

void TLCDSecLineUpdate::execute(){
  LCD_go_to_xy(0,1);
  switch(mode){
    case LCD_INPUT:
    case LCD_BASS:
    case LCD_MIDDLE:
    case LCD_TREBLE:
    case LCD_INPUT_SELECT:
  }

}

/*
 * jobs.h
 *
 *  Created on: 6 серп. 2016 р.
 *      Author: alfons-14
 */

#ifndef LIBS_JOBS_H_
#define LIBS_JOBS_H_

#include "../scmRTOS/ext/jobs/job.h"
#include "RTC.h"
#include "TDA7439.h"
#include "TEA5767N.h"

class TLCDTimeUpdate : TJob{
protected:
  RTC* rtc;
public:
  TLCDTimeUpdate(RTC*);
  virtual void execute();
};

class TLCDBattUpdate : TJob{
protected:
public:
  bool charging = false;
  uint8_t level=0;
  TLCDBattUpdate(bool);
  virtual void execute();
};

//modes
#define LCD_INPUT_SELECT 0
#define LCD_BASS 1
#define LCD_MIDDLE 2
#define LCD_TREBLE 3
#define LCD_INPUT 4

class TLCDSecLineUpdate : TJob{
protected:
  char buf[17];
  TEA5767N* radio;
  TDA7439* audio;
  uint8_t mode;
public:
  TLCDSecLineUpdate(TEA5767N*, TDA7439*);
  virtual void execute();
};


#endif /* LIBS_JOBS_H_ */

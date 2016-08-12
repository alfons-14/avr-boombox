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

class TLCDTimeUpdate : public TJob{
protected:
  RTC* rtc;
public:
  TLCDTimeUpdate(RTC*);
  virtual void execute();
};

class TLCDBattUpdate : public TJob{
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
#define LCD_VOLUME 5

//inputs
#define INFM IN1
#define IN35 IN2
#define IN63 IN3
#define INBL IN4

class TLCDSecLineUpdate : public TJob{
protected:
  char buf[17];
  TEA5767N* radio;
  TDA7439* audio;
public:
  uint8_t mode;
  uint16_t freq;
  uint_fast32_t timeout;
  TLCDSecLineUpdate(TEA5767N*, TDA7439*);
  virtual void execute();
};

class TLCDFMSigUpdate : public TJob{
protected:
  TEA5767N* radio;
  uint8_t level;
public:
  TLCDFMSigUpdate(TEA5767N*);
  virtual void execute();
};


#endif /* LIBS_JOBS_H_ */

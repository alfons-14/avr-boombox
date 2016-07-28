/*
 * RTC.h
 *
 *  Created on: 6 лип. 2016
 *      Author: alfons-14
 */

#ifndef RTC_H_
#define RTC_H_

#include <inttypes.h>

union time_t {
	uint8_t byte;
	struct {
		uint8_t l :4;
		uint8_t h :4;
	};
};

class RTC {
private:
	uint8_t rx[7];
public:
	uint8_t addr;
	time_t min;
	time_t sec;
	time_t hour;
	uint8_t day;
	time_t date;
	time_t mounth;
	uint8_t century;
	time_t year;
	void setTime();
	void setMin(uint8_t);
	void setSec(uint8_t);
	void setHour(uint8_t);
	void setDay(uint8_t);
	void setYear(uint8_t);
	void update();
	void upload();
	RTC(uint8_t);

};

#endif /* RTC_H_ */

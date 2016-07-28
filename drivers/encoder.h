/*
 * encoder.h
 *
 *  Created on: 16 лип. 2016
 *      Author: alfons-14
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <inttypes.h>

//encoder pins

#define P1 B, 0, H
#define P2 B, 7, H

void encoder_init();
int8_t encoder_get_state();
void encoder_reset();
#endif /* ENCODER_H_ */

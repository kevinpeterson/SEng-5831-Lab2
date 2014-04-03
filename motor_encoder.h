/*
 * motor_encoder.h
 *
 *  Created on: Mar 30, 2014
 *      Author: m005256
 */

#ifndef MOTOR_ENCODER_H_
#define MOTOR_ENCODER_H_

#include <inttypes.h>

void initialize_motor_encoder();

int32_t get_encoder_count();

#endif /* MOTOR_ENCODER_H_ */

/*
 * An interface into the motor encoder.
 */

#ifndef MOTOR_ENCODER_H_
#define MOTOR_ENCODER_H_

#include <inttypes.h>

/**
 * Initialization
 */
void initialize_motor_encoder();

/**
 * Get the current number of counts that the motor has traversed.
 */
int32_t get_encoder_count();

#endif /* MOTOR_ENCODER_H_ */

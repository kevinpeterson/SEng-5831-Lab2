/*
 * Motor control functionality
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include <inttypes.h>

/**
 * Initialization
 */
void initialize_motor();

/**
 * Set the motor speed (in counts/second)
 */
void set_motor_speed(int16_t speed);

#endif /* MOTOR_H_ */

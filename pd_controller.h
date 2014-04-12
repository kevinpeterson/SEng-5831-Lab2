/*
 * A Proportional/Derivative controller.
 */

#ifndef PD_CONTROLLER_H_
#define PD_CONTROLLER_H_

/**
 * Initialize the controller.
 */
void initialize_pd_controller();

/**
 * Set the desired motor speed (in counts/second).
 */
void set_speed(int16_t desired_speed);

/**
 * Set a position for the motor to rotate to (and stay there).
 * This is 'count' position -- there are 128 counts per rotation.
 */
void set_position(uint8_t desired_position);

#endif /* PD_CONTROLLER_H_ */

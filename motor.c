#include <pololu/orangutan.h>
#include <avr/io.h>
#include <inttypes.h>
#include "command_parse.h"
#include "motor.h"

void _set_up_pwm() {

	// Set PWM output ports
    DDRC |= _BV(PC6);
    DDRD |= _BV(PD6);

    // Non-Inverting
    TCCR2A = _BV(COM2B1);

    // Fast PWM, TOP == 0xFF,
    TCCR2A |= _BV(WGM21) | _BV(WGM20);

    // CPU/64 prescalar
    TCCR2B = _BV(CS22);

    // Start at 0 speed
    OCR2B = 0;
}

void set_motor_speed(uint8_t speed) {
	OCR2B = speed;
}

void initialize_motor() {
	_set_up_pwm();
}

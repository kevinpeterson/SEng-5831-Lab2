#include <pololu/orangutan.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
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
    OCR2A = 0;
    OCR2B = 0;
}

void set_motor_speed(int16_t speed) {
	if(speed >= 0) {
		PORTC &= ~_BV(6);
	} else {
		PORTC |= _BV(6);
	}
	OCR2B = abs(speed);
}

void initialize_motor() {
	_set_up_pwm();
}

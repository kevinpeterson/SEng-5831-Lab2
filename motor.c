#include <pololu/orangutan.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "command_parse.h"
#include "motor.h"
#include "logger.h"


#define MAX_SPEED_CHANGE 200

volatile char transition_motor_wait = 0;

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

    // Brownout prevention timer
    TIMSK1 = _BV(OCIE1A);
	OCR1A = 31250;
}

void _start_timer() {
	clear();
	transition_motor_wait = 1;
	TCCR1B = _BV(CS11) |  _BV(CS10) | _BV(WGM12);
}

void _stop_timer() {
	clear();
	TCCR1B = 0;
	TCNT1H = 0;
	TCNT1L = 0;
	transition_motor_wait = 0;
}

int16_t _calculate_signed_speed_value() {
	if(PORTC & _BV(6)) {
		return -OCR2B;
	} else {
		return OCR2B;
	}
}

uint16_t _calculate_requested_speed_change(int16_t requested_speed) {
	return abs(requested_speed - _calculate_signed_speed_value());
}

void set_motor_speed(int16_t speed) {
	if(transition_motor_wait) {
		return;
	}

	uint16_t speed_change = _calculate_requested_speed_change(speed);

	int16_t signed_speed_value = _calculate_signed_speed_value();

	if(speed_change > MAX_SPEED_CHANGE) {
		_start_timer();
		if(signed_speed_value - speed < 0) {
			speed = signed_speed_value + MAX_SPEED_CHANGE;
		} else {
			speed = signed_speed_value - MAX_SPEED_CHANGE;
		}
	}

	if(speed >= 0) {
		PORTC &= ~_BV(6);
	} else {
		PORTC |= _BV(6);
	}

#if 0
	char c[24];
	sprintf(c, "T Delta: %d", speed_change);
	log_msg(c, DEBUG);
#endif

	OCR2B = abs(speed);
}

void initialize_motor() {
	_set_up_pwm();
}

ISR(TIMER1_COMPA_vect ) {
	_stop_timer();
}


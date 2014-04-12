#include <pololu/orangutan.h>

#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "motor_encoder.h"
#include "serial.h"
#include "logger.h"

volatile int32_t wheel_counter = 0;

uint8_t pcint_mask = _BV(0) | _BV(1);

volatile uint8_t last_pina;

volatile uint8_t last_changed;

volatile char reversed = 0;

unsigned long prev_ticks = 0;

volatile double velocity;

void initialize_motor_encoder() {
	//make port A as input
	DDRA &= ~pcint_mask;

	//enable pull-ups
	PORTA |= pcint_mask;

	PCICR = _BV(PCIE0);
	PCIFR = _BV(PCIF0);
	PCMSK0 = pcint_mask;
	last_pina = (PINA & pcint_mask);
}

ISR(PCINT0_vect) {

#if 0
	char c[20];
	sprintf(c, "Wheel Cnt: %d", (int)wheel_counter);
	log_msg(c, DEBUG);
#endif

	uint8_t changed = (last_pina ^ (PINA & pcint_mask));

	if(changed == last_changed) {
		reversed = !reversed;
	}

	if(reversed) {
		wheel_counter--;
	} else {
		wheel_counter++;
	}
/*
 	 This was an attempt to measure velocity in terms of system clock time.
 	 It didn't work very well... see the writeup for more information.

	if(! reversed) {
		unsigned long ticks = get_ticks();
		unsigned long elapsed_time_us = ticks_to_microseconds(ticks - prev_ticks);
		prev_ticks = ticks;

		velocity = ((long) 1 * 1000 * 1000) / (elapsed_time_us);
	} else {
		prev_ticks = ticks_to_microseconds( get_ticks() );
		velocity = 0;
	}

#if 1
	char c[20];
	sprintf(c, "W V: %8.6f", velocity);
	log_msg(c, DEBUG);
#endif
*/

	last_changed = changed;
	last_pina = (PINA & pcint_mask);
}

int32_t get_encoder_count() {
	return wheel_counter;
}

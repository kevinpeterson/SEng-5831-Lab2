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
	sprintf(c, "Wheel: %d", (int)wheel_counter);
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

	last_changed = changed;
	last_pina = (PINA & pcint_mask);
}

int32_t get_encoder_count() {
	return wheel_counter;
}

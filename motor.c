#include <avr/io.h>

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
    OCR2B = 50;
}

void initialize_motor() {
	_set_up_pwm();
}

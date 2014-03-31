#include <avr/io.h>
#include "command_parse.h"

ParseResult _set_speed_function(char* params, void (*output_line)(char*));

Command speed_command = {.command = 'S', .alias = 's',
		.command_function = &_set_speed_function, .help = "Set the reference speed (counts/sec)" };

ParseResult _set_speed_function(char* params, void (*output_line)(char*)) {
	return COMMAND_PARSE_OK;
}

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
	add_command(&speed_command);
}

/*
    T = Output motor signal (torque)
    Pr = Desired motor position or speed
    Pm = Current motor position or speed
    Vm = Current motor velocity (computed based on finite differences)
    Kp = Proportional gain
    Kd = Derivative gain

    (positional)
    T = Kp(Pr - Pm) - Kd * Vm

    (speed)
    T = Kp(Vr - Vm) + Kd( d (Vr-Vm) / dt )
 */
#include <pololu/orangutan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "motor_encoder.h"
#include "motor.h"
#include "scheduler.h"
#include "inttypes.h"
#include "command_parse.h"
#include "logger.h"
#include "serial.h"

typedef enum {
  SPEED_MODE = 1,
  POSITION_MODE = 2
} ControllerMode;

volatile ControllerMode controller_mode;

void _pd_controller_cycle();
void _calculate_avg_motor_speed();
void _sample_motor_speed();
void _log_status();

ParseResult _set_speed_function(char* params, void (*output_line)(char*));
ParseResult _set_position_function(char* params, void (*output_line)(char*));
ParseResult _toggle_logging_function(char* params, void (*output_line)(char*));
ParseResult _set_kp_function(char* params, void (*output_line)(char*));
ParseResult _set_kd_function(char* params, void (*output_line)(char*));
ParseResult _view_current_values_function(char* params, void (*output_line)(char*));


Command speed_command = {.command = 'S', .alias = 's',
		.command_function = &_set_speed_function, .help = "Set the reference speed (counts/sec)" };

Command position_command = {.command = 'R', .alias = 'r',
		.command_function = &_set_position_function, .help = "Set the reference position" };

Command toggle_logging_command = {.command = 'L', .alias = 'l',
		.command_function = &_toggle_logging_function, .help = "Toggle logging of Pr, Pm, and T" };

Command set_kp_command = {.command = 'P', .alias = 'p',
		.command_function = &_set_kp_function, .help = "Set Kp" };

Command set_kd_command = {.command = 'D', .alias = 'd',
		.command_function = &_set_kd_function, .help = "Set Kd" };

Command view_current_values_command = {.command = 'V', .alias = 'v',
		.command_function = &_view_current_values_function, .help = "View the current values Kd, Kp, Vm, Pr, Pm, and T" };

volatile Task pd_controller_task = { .period = 20, .interrupt_function =
		&_pd_controller_cycle, .released = 0, .name = "Cycle the PD Controller Task" };

volatile Task sample_motor_position_task = { .period = 10, .interrupt_function =
		&_sample_motor_speed, .released = 0, .name = "Sample Motor Position" };

volatile Task log_status = { .period = 5, .interrupt_function =
		&_log_status, .released = 0, .name = "Log the status of Pr, Pm, and T" };

volatile uint8_t proportional_gain = 1;
volatile uint8_t derivative_gain = 1;

volatile int16_t desired_speed = 0;
volatile uint8_t desired_position = 0;

volatile int16_t previous_velocity;
volatile int16_t current_velocity;

volatile int32_t motor_position_samples[20] = {0};

volatile char is_logging = 0;

volatile int16_t torque;

void initialize_pd_controller() {
	register_task(&pd_controller_task);
	register_task(&sample_motor_position_task);
	register_task(&log_status);

	add_command(&speed_command);
	add_command(&position_command);
	add_command(&toggle_logging_command);
	add_command(&set_kp_command);
	add_command(&set_kd_command);
	add_command(&view_current_values_command);
}

ParseResult _toggle_logging_function(char* params, void (*output_line)(char*)) {
	is_logging = ~is_logging;
	return COMMAND_PARSE_OK;
}

void set_speed(int16_t speed) {
	desired_speed = speed;
}

uint8_t _normalize_position(uint8_t position) {
	if(position > 127) {
		position = 127;
	}

	return position;
}

void set_position(uint8_t position) {
	desired_position = _normalize_position(position);
}

void set_proportional_gain(uint8_t gain) {
	proportional_gain = gain;
}

void set_derivative_gain(uint8_t gain) {
	derivative_gain = gain;
}

void set_pd_controller_hz(uint16_t hz) {
	if(hz != pd_controller_task.period) {
		pd_controller_task.period = hz;
		recalculate_hyperperiod();
	}
}

int16_t _normalize_torque(int16_t _torque) {
	if(_torque > 255) {
		_torque = 255;
	}
	if(_torque < -255) {
		_torque = -255;
	}
	return _torque;
}

int32_t _sample_motor_position() {
	return get_encoder_count() % 128;
}

void _sample_motor_speed() {
	int32_t current_motor_position = get_encoder_count();

	int32_t tmp_samples[10] = {0};

	int i;
	for(i = 1;i < 10;i++) {
		tmp_samples[i] = motor_position_samples[i - 1];
	}
	tmp_samples[0] = current_motor_position;

	int32_t diff = (tmp_samples[0] - tmp_samples[9]);

#if 0
		char c[24];
		sprintf(c, "V: %d", (int)diff * 10);
		log_msg(c, DEBUG);
#endif

	for(i = 0;i < 10;i++) {
		motor_position_samples[i] = tmp_samples[i];
	}

	cli();
	previous_velocity = current_velocity;
	current_velocity = diff;
	sei();
}

/**
 * T = Kp(Pr - Pm) - Kd*Vm where
 */
void _calculate_position_torque() {
	int16_t _torque = (proportional_gain * (desired_position - _sample_motor_position())) - (derivative_gain * current_velocity);

	_torque = _normalize_torque(_torque);

#if 1
	char c[24];
	sprintf(c, "Pos T: %d", _torque);
	log_msg(c, DEBUG);
#endif

	torque = _torque;
}

/**
 * Kp(Vr - Vm) + Kd( d (Vr-Vm) / dt )
 */
void _calculate_speed_torque() {
	int16_t _torque = (proportional_gain * (desired_speed - current_velocity)) + (derivative_gain * ( (previous_velocity - current_velocity) ));

	_torque = _normalize_torque(_torque);

#if 0
	char c[24];
	sprintf(c, "Sp T: %d", _torque);
	log_msg(c, DEBUG);
#endif

	torque = _torque;
}


void _pd_controller_cycle() {
	if(! controller_mode) {
		return;
	}

	switch (controller_mode) {
		case SPEED_MODE:
			_calculate_speed_torque();
			break;
		case POSITION_MODE:
			_calculate_position_torque();
			break;
		default:
			log_msg("Error executing PD Controller", ERROR);
	}

	set_motor_speed(torque);
}

ParseResult _set_speed_function(char* params, void (*output_line)(char*)) {
	controller_mode = SPEED_MODE;
	int i;
	sscanf(params, "%d", &i);
	set_speed(i);
	return COMMAND_PARSE_OK;
}

ParseResult _set_position_function(char* params, void (*output_line)(char*)) {
	controller_mode = POSITION_MODE;
	int i;
	sscanf(params, "%d", &i);
	set_position(i);
	return COMMAND_PARSE_OK;
}

/**
 * Toggle logging of Pr, Pm, and T
 */
void _log_status() {
	if(is_logging) {
		char output_line[50];
		int len = sprintf(output_line, "Pr: %d, Pm`: %d, Pm: %d, T: %d\r\n", desired_speed, previous_velocity, current_velocity, torque);
		serial_to_send(output_line, len);
	}
}

ParseResult _set_kp_function(char* params, void (*output_line)(char*)) {
	int i;
	sscanf(params, "%d", &i);
	proportional_gain = i;
	return COMMAND_PARSE_OK;
}

ParseResult _set_kd_function(char* params, void (*output_line)(char*)) {
	int i;
	sscanf(params, "%d", &i);
	derivative_gain = i;
	return COMMAND_PARSE_OK;
}

/**
 * View the current values Kd, Kp, Vm, Pr, Pm, and T
 */
ParseResult _view_current_values_function(char* params, void (*output_line)(char*)) {
	char output_buffer[100];
	sprintf(output_buffer, "Kd: %d, Kp: %d, Vr: %d, Vm: %d, Pr: %d, Pm: %d, T: %d\r\n",
			derivative_gain,
			proportional_gain,
			desired_speed,
			current_velocity,
			desired_position,
			(int)_sample_motor_position(),
			torque);
	output_line(output_buffer);
	return COMMAND_PARSE_OK;
}

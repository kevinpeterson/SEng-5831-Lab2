/*
 * command_line.c
 *
 *  Created on: Mar 29, 2014
 *      Author: m005256
 */
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "command_parse.h"
#include "serial.h"

unsigned char command_buffer_position;
char command_buffer[32];

void _print_command_prompt() {
	serial_to_send("\r\n>: ", 5);
}

void process_command(char* buffer) {
	serial_to_send("got it", 6);
	_print_command_prompt();
}

void _build_command_buffer(char c) {
	command_buffer[command_buffer_position] = c;
	command_buffer_position++;
	if(strstr(command_buffer, "\r") != NULL){
		process_command(command_buffer);
		memset(command_buffer, 0, strlen(command_buffer));
		command_buffer_position = 0;
	}
}

void initialize_command_line() {
	register_incoming_callback(&_build_command_buffer);
	_print_command_prompt();
}

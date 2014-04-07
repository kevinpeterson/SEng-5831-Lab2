/*
 * command_line.c
 *
 *  Created on: Mar 29, 2014
 *      Author: m005256
 */
#include <pololu/orangutan.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "command_parse.h"
#include "serial.h"

ParseResult help_function(char* params, void (*output_line)(char*));

Command help_command = {.command = 'h',
		.command_function = &help_function, .help = "Display the help menu." };

unsigned char command_buffer_position;
char command_buffer[32];

ParseResult help_function(char* params, void (*output_line)(char*)) {
	CommandNode* n = get_all_commands();
	while(n != NULL) {
		char cmd = n->command->command;
		char alias = n->command->alias;
		char* msg = n->command->help;

		char buffer[100];
		if(! alias) {
			sprintf(buffer, "%c - %s", cmd, msg);
		} else {
			sprintf(buffer, "%c/%c - %s", cmd, alias, msg);
		}
		output_line(buffer);
		n = n->next;
	}
	return COMMAND_PARSE_OK;
}

void _print_command_prompt() {
	serial_to_send("\n>: ", 5);
}

void process_command(char* buffer) {
	parse_command(buffer);
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
	add_command(&help_command);
}

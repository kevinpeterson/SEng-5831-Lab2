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
ParseResult _toggle_echo_back_function();

Command help_command = {.command = 'h',
		.command_function = &help_function, .help = "Display the help menu." };

Command echo_back_command = {.command = 'e',
		.command_function = &_toggle_echo_back_function, .help = "Toggle serial echo back." };

static volatile char echo_back = 1;

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

void print_command_prompt() {
	if(echo_back) {
		serial_to_send("\n>: ", 5);
	}
}

void process_command(char* buffer) {
	parse_command(buffer);
	print_command_prompt();
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
	print_command_prompt();
	add_command(&help_command);
	add_command(&echo_back_command);
}


ParseResult _toggle_echo_back_function(char* params, void (*output_line)(char*)) {
	char c;
	sscanf(params, "%c", &c);
	if(c == '+') {
		echo_back = 1;
	} else if(c == '-') {
		echo_back = 0;
	} else {
		echo_back = !echo_back;
	}
    set_echo_back(echo_back);
    return COMMAND_PARSE_OK;
}

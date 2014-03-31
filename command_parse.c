/**
 * A linked-list of callback functions.
 */
#include <pololu/orangutan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_parse.h"
#include "serial.h"

CommandNode* command_list = NULL;

void add_command(Command* command) {
	if(command_list == NULL) {
		CommandNode* n = (CommandNode*) malloc(sizeof(CommandNode));
		n->command = command;
		n->next = NULL;
		command_list = n;
	} else {
		CommandNode* n = command_list;
		while(n->next != NULL) {
			n = n->next;
		}
		CommandNode* new_node = (CommandNode*) malloc(sizeof(CommandNode));
		new_node->command = command;
		new_node->next = NULL;
		n->next = new_node;
	}

}

CommandNode* get_all_commands() {
	return command_list;
}

void _send_line_to_serial(char* line) {
	serial_to_send(line, strlen(line));
	serial_to_send("\r\n", 2);
}

void parse_command(char* command) {
	CommandNode* n = command_list;
	char c;
	char args[25];
	sscanf(command, "%c %s", &c, args);
	while(n != NULL) {
		char cmd = n->command->command;
		char alias = n->command->alias;
		if(cmd == c || alias == c) {
			n->command->command_function(args, &_send_line_to_serial);
			serial_to_send("\r\n", 2);
			break;
		} else {
			n = n->next;
		}
	}
}

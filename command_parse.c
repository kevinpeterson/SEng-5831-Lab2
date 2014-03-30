/**
 * A linked-list of callback functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include "command_parse.h"

typedef struct CommandNode {
	Command* command;
	struct CommandNode* next;
} CommandNode;

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

/**
 * A command line parsing tool inspired by the Python argparse library:
 * http://docs.python.org/3.4/library/argparse.html
 */
typedef enum {
  COMMAND_PARSE_OK = 0,
  COMMAND_PARSE_INVALID = 1
} ParseResult;

typedef struct command_t {
	ParseResult (*command_function)(char* params, void (*output_line)(char*));
	char command;
	char alias;
	char* help;
} Command;

typedef struct CommandNode {
	Command* command;
	struct CommandNode* next;
} CommandNode;

void add_command(Command* command);

CommandNode* get_all_commands();

void parse_command(char* command);


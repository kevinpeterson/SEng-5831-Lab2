/**
 * A command line parsing tool inspired by the Python argparse library:
 * http://docs.python.org/3.4/library/argparse.html
 */
typedef enum {
  OK = 0,
  INVALID = 1
} ParseResult;

typedef struct command_t {
	uint16_t period;
	ParseResult (*command_function)(char* params);
	char* command;
	char* help;
} Command;


void add_command(Command* command);

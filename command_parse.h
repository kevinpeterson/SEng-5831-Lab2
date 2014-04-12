/**
 * A command line parsing tool inspired by the Python argparse library:
 * http://docs.python.org/3.4/library/argparse.html
 *
 * Any component of the system can register commands here, and add them to
 * the list of allowed command line commands.
 */

/**
 * An indicator as to wheter the command was parsed successfully or not.
 */
typedef enum {
  COMMAND_PARSE_OK = 0,
  COMMAND_PARSE_INVALID = 1
} ParseResult;


/**
 * A representation of a command line command. Each command has a:
 *  - 'command_function' -- a funtion to invoke upon activation of the command
 * 	- 'command' -- a single letter command representation
 * 	- 'alias' (optional) -- another way of invoking the command
 * 	- 'help' -- a help message to display
 */
typedef struct command_t {
	ParseResult (*command_function)(char* params, void (*output_line)(char*));
	char command;
	char alias;
	char* help;
} Command;


/**
 * A linked-list to store the commands.
 */
typedef struct CommandNode {
	Command* command;
	struct CommandNode* next;
} CommandNode;


/**
 * Register a new command line command with the system.
 */
void add_command(Command* command);


/**
 * Get all the currently registered commands.
 */
CommandNode* get_all_commands();


/**
 * Execute a requested single char command.
 */
void parse_command(char* command);


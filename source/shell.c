/**
 * shell.c: a simple shell for development and testing
 *
 * Currently this only exists for Serial, because a shell needs input and HDMI
 * can't provide input without USB (ugh).
 */
#include "pi.h"
#include "serial.h"

static int echo(int argc, char *argv[])
{
	int i;
	for (i = 0; i < argc; i++)
		printf("arg %u: \"%s\"\n", i, argv[i]);

	return 0;
}

static int help(int argc, char *argv[]);

struct shell_command {
	char *name;
	int (*call)(int argc, char *argv[]);
	char *help;
};

struct shell_command cmds[] = {
	{"sysinfo", sysinfo, "display contents of CPU registers"},
	{"echo", echo, "echo each argument on its own line"},
	{"help", help, "display this message"},
};


static int help(int argc, char *argv[])
{
	uint32_t i;
	(void)argc;
	(void)argv;

	puts("Stevix shell: a basic bare-metal shell for testing kernel "
	     "components\n\n");

	for (i = 0; i < nelem(cmds); i++) {
		printf("%s - %s\n", cmds[i].name, cmds[i].help);
	}

	return 0;
}

char input[1024];
char *args[32];

static inline int in(char c, const char *delim)
{
	for (; *delim; delim++) {
		if (*delim == c)
			return 1;
	}
	return 0;
}

/**
 * This is not actually the correct signature for strtok. But it's a simpler
 * abstraction to implement. It returns the next token from the string "input",
 * using "delim" as token delimiters. It stores the pointer where you should
 * start tokenizing again in *next.
 */
char *strtok_(char *input, const char *delim, char **next)
{
	/* advance past delimiters, if they are there */
	while (*input && in(*input, delim))
		input++;

	if (!*input)
		return NULL;

	*next = input;

	/* advance until the next delimiter or end of string */
	while (**next && !in(**next, delim))
		(*next)++;

	if (**next) {
		/* we are not at the end of string. nul-terminate and set next
		 * to the character past this one */
		**next = '\0';
		(*next)++;
	}
	/* if we were already at end of string, *next points at NUL so the next
	 * time we are called, we will receive an empty string and return NULL.
	 */
	return input;
}

/**
 * This is strcmp. Yup.
 */
int strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

static uint8_t tokenize(char *input)
{
	uint8_t position = 0;
	char *token, *next;
	token = next = input;

	token = strtok_(next, "\r\n\t ", &next);
	while (token != NULL) {
		args[position] = token;
		position++;

		if (position >= nelem(args)) {
			break;
		}

		token = strtok_(next, "\r\n\t ", &next);
	}
	return position;
}

static struct shell_command *find_command(char *cmd)
{
	uint8_t i;
	for (i = 0; i < nelem(cmds); i++)
		if (strcmp(cmds[i].name, cmd) == 0)
			return &cmds[i];
	return NULL;
}

void shell(void)
{
	uint8_t argc;
	struct shell_command *cmd;
	puts("\nStevix shell. Configure your terminal program:\n"
	     "1. Expect CRLF line endings on output.\n"
	     "2. Send CR when Enter key pressed.\n"
	     "3. Enable local echo.\n\n");

	for (;;) {
		puts("> ");
		serial_recv_line(input, sizeof(input));
		argc = tokenize(input);
		cmd = find_command(args[0]);
		if (cmd) {
			cmd->call(argc, args);
		} else {
			printf("command \"%s\" not found\n", args[0]);
		}
	}
}

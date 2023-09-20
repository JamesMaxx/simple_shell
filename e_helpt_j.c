#include "main.h"

/**
 * help_env - Display help information for the 'env' command.
 */
void help_env(void)
{
	char *msg = "env: env\n\tPrint current environment.\n";

	write(STDOUT_FILENO, msg, _strlen(msg));
}

/**
 * help_setenv - Display help information for the 'setenv' command.
 */
void help_setenv(void)
{
	char *msg = "setenv: setenv [VARIABLE] [VALUE]\n\tInitializes new variable";

	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "environment variable, modifies existing variable.\n\n";
	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "\tWhen fails, prints message to stderr.\n";
	write(STDOUT_FILENO, msg, _strlen(msg));
}

/**
 * help_unsetenv - Display help information for the 'unsetenv' command.
 */
void help_unsetenv(void)
{
	char *msg = "unsetenv: unsetenv [VARIABLE]\n\tFor removing an ";

	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "environmental variable.\n\n\tWhen fail, prints a ";
	write(STDOUT_FILENO, msg, _strlen(msg));
	msg = "message to stderr.\n";
	write(STDOUT_FILENO, msg, _strlen(msg));
}

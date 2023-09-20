#include "main.h"
alias_t *aliases = NULL;
/**
 * sig_handler - Handles the SIGINT signal (Ctrl+C) for the shell.
 * @sig: The signal number.
 *
 * Description: This function is used as a signal handler for SIGINT
 * signals. It writes a new prompt to the terminal after the signal
 * is received to maintain the shell's functionality.
 */
void sig_handler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * execute - Executes a command in the shell.
 * @args: An array of strings containing the command and its arguments.
 * @front: A pointer to the front of the arguments array.
 *
 * Return: The exit status of the executed command.
 *
 * Description: This function is responsible for executing commands in the shell.
 * It checks if the command is an absolute path or in the current directory.
 * If not, it searches for the command in the directories listed in the PATH
 * environment variable. If the command is found, it forks a child process to
 * execute the command using the execve system call. The exit status of the
 * command is returned.
 */
int execute(char **args, char **front)
{
	pid_t child_pid;
	int status, flag = 0, ret = 0;
	char *command = args[0];

	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		command = get_location(command);
	}

	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (create_error(args, 126));
		else
			ret = (create_error(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (flag)
				free(command);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(command, args, environ);
			if (errno == EACCES)
				ret = (create_error(args, 126));
			free_env();
			free_args(args, front);
			free_alias_list(aliases);
			_exit(ret);
		}
		else
		{
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}
	if (flag)
		free(command);
	return (ret);
}

/**
 * main - The entry point of the shell.
 * @argc: The number of arguments passed to the shell.
 * @argv: An array of strings containing the arguments passed to the shell.
 *
 * Return: The exit status of the shell.
 *
 * Description: This function is the entry point of the shell. It sets up the
 * shell environment, including signal handlers and initializations, and
 * processes commands based on whether the shell is run interactively or
 * from a file.
 */
int main(int argc, char *argv[])
{
	int ret = 0, retn;
	int *exe_ret = &retn;
	char *prompt = "$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, sig_handler);

	*exe_ret = 0;
	environ = _copyenv();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		ret = proc_file_commands(argv[1], exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}
	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handle_args(exe_ret);
		free_env();
		free_alias_list(aliases);
		return (*exe_ret);
	}
	while (1)
	{
		write(STDOUT_FILENO, prompt, 2);
		ret = handle_args(exe_ret);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_env();
			free_alias_list(aliases);
			exit(*exe_ret);
		}
	}
	free_env();
	free_alias_list(aliases);
	return (*exe_ret);
}

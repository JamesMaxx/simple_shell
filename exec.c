#include "main.h"

/**
 * get_args - Read and process input from the user.
 * @line: Pointer to a buffer to store the input.
 * @exe_ret: Pointer to the exit status variable.
 *
 * Return: Pointer to the input line.
 */
char *get_args(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}

/**
 * call_args - Call appropriate functions based on logical operators.
 * @args: The arguments provided by the user.
 * @front: The original array of arguments.
 * @exe_ret: Pointer to the exit status variable.
 *
 * Return: The exit status of the executed command.
 */
int call_args(char **args, char **front, int *exe_ret)
{
	int ret, m_m;

	if (!args[0])
		return (*exe_ret);
	for (m_m = 0; args[m_m]; m_m++)
	{
		if (_strncmp(args[m_m], "||", 2) == 0)
		{
			/* Handling logical OR operator */
			free(args[m_m]);
			args[m_m] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++m_m];
				m_m = 0;
			}
			else
			{
				for (m_m++; args[m_m]; m_m++)
					free(args[m_m]);
				return (ret);
			}
		}
		else if (_strncmp(args[m_m], "&&", 2) == 0)
		{
			/* Handling logical AND operator */
			free(args[m_m]);
			args[m_m] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++m_m];
				m_m = 0;
			}
			else
			{
				for (m_m++; args[m_m]; m_m++)
					free(args[m_m]);
				return (ret);
			}
		}
	}
	args = replace_aliases(args);
	ret = run_args(args, front, exe_ret);
	return (ret);
}

/**
 * run_args - Execute built-in commands or external programs.
 * @args: The arguments provided by the user.
 * @front: The original array of arguments.
 * @exe_ret: Pointer to the exit status variable.
 *
 * Return: The exit status of the executed command.
 */
int run_args(char **args, char **front, int *exe_ret)
{
	int ret, m_m;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*exe_ret = ret;
	}
	else
	{
		*exe_ret = execute(args, front);
		ret = *exe_ret;
	}

	hist++;

	for (m_m = 0; args[m_m]; m_m++)
		free(args[m_m]);

	return (ret);
}

/**
 * handle_args - Process and execute the input from the user.
 * @exe_ret: Pointer to the exit status variable.
 *
 * Return: The exit status of the executed command.
 */
int handle_args(int *exe_ret)
{
	int ret = 0, m_m;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (m_m = 0; args[m_m]; m_m++)
	{
		if (_strncmp(args[m_m], ";", 1) == 0)
		{
			/* Handling multiple commands separated by ';' */
			free(args[m_m]);
			args[m_m] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[++m_m];
			m_m = 0;
		}
	}
	if (args)
		ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}

/**
 * check_args - Check for syntax errors in the provided arguments.
 * @args: Represents user's
 *
 * Return: 0 if no syntax errors, otherwise -1.
 */
int check_args(char **args)
{
	size_t m_m;
	char *cur, *nex;

	for (m_m = 0; args[m_m]; m_m++)
	{
		cur = args[m_m];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (m_m == 0 || cur[1] == ';')
				return (create_error(&args[m_m], 2));
			nex = args[m_m + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&args[m_m + 1], 2));
		}
	}
	return (0);
}

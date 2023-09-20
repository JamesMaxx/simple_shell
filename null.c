#include "main.h"


/**
 * cant_open - Prints an error message when a file cannot be opened.
 *
 * @file_path: The path of the file that failed to open.
 *
 * Return: 127 to indicate the error.
 */
int cant_open(char *file_path)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": Can't open ");
	_strcat(error, file_path);
	_strcat(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, len);
	free(error);
	return (127);
}

/**
 * proc_file_commands - Processes commands from a file.
 *
 * @file_path: The path of the file containing the commands.
 * @exe_ret: Pointer to store the exit status of the command execution.
 *
 * Return: The exit status of the command execution.
 */
int proc_file_commands(char *file_path, int *exe_ret)
{
	ssize_t file, b_read, m_m;
	unsigned int line_size = 0;
	unsigned int old_size = 120;
	char *line, **args, **front;
	char buffer[120];
	int ret;

	hist = 0;
	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		*exe_ret = cant_open(file_path);
		return (*exe_ret);
	}
	line = malloc(sizeof(char) * old_size);
	if (!line)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && line_size == 0)
			return (*exe_ret);
		buffer[b_read] = '\0';
		line_size += b_read;
		line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read);
	for (m_m = 0; line[m_m] == '\n'; m_m++)
		line[m_m] = ' ';
	for (; m_m < line_size; m_m++)
	{
		if (line[m_m] == '\n')
		{
			line[m_m] = ';';
			for (m_m += 1; m_m < line_size && line[m_m] == '\n'; m_m++)
				line[m_m] = ' ';
		}
	}
	variable_replacement(&line, exe_ret);
	handle_line(&line, line_size);
	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (0);
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
			free(args[m_m]);
			args[m_m] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[++m_m];
			m_m = 0;
		}
	}

	ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}

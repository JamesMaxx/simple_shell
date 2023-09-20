#include "main.h"

/**
 * free_args - Frees memory allocated for an array of strings and the array itself.
 *
 * @args: The array of strings to be freed.
 * @front: The array itself that needs to be freed as well.
 */
void free_args(char **args, char **front)
{
	size_t z;

	for (z = 0; args[z] || args[z + 1]; z++)
		free(args[z]);

	free(front);
}

/**
 * get_pid - Retrieves the process ID (PID) of the current process.
 *
 * Return: A string containing the PID, or NULL on failure.
 */
char *get_pid(void)
{
	size_t z = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[z] != ' ')
		z++;
	buffer[z] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Retrieves the value of an environment variable.
 *
 * @beginning: A pointer to the beginning of the environment variable name.
 * @len: The length of the environment variable name.
 *
 * Return: The value of the environment variable, or NULL if not found.
 */
char *get_env_value(char *beginning, int len)
{
	char **var_addr;
	char *replacement = NULL, *temp, *var;

	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, beginning, len);

	var_addr = _getenv(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (replacement)
			_strcpy(replacement, temp);
	}

	return (replacement);
}

/**
 * variable_replacement - Replaces environment variable references in a string.
 *
 * @line: A pointer to a pointer to the original string.
 * @exe_ret: Pointer to store the exit status of the command execution.
 */
void variable_replacement(char **line, int *exe_ret)
{
	int z, k = 0, len;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (z = 0; old_line[z]; z++)
	{
		if (old_line[z] == '$' && old_line[z + 1] &&
				old_line[z + 1] != ' ')
		{
			if (old_line[z + 1] == '$')
			{
				replacement = get_pid();
				k = z + 2;
			}
			else if (old_line[z + 1] == '?')
			{
				replacement = _itoa(*exe_ret);
				k = z + 2;
			}
			else if (old_line[z + 1])
			{
				for (k = z + 1; old_line[k] &&
						old_line[k] != '$' &&
						old_line[k] != ' '; k++)
					;
				len = k - (z + 1);
				replacement = get_env_value(&old_line[z + 1], len);
			}
			new_line = malloc(z + _strlen(replacement) +
					_strlen(&old_line[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, z);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[k]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			z = -1;
		}
	}
}

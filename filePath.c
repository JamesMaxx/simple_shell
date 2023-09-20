#include "main.h"

/**
 * get_location - Get the location of a command in the PATH directories.
 * @command: The command to find in the PATH directories.
 *
 * Return: Pointer to full path of command if found, otherwise NULL.
 */
char *get_location(char *command)
{
	char **path, *temp;
	list_t *dirs, *head;
	struct stat st;

	path = _getenv("PATH");
	if (!path || !(*path))
		return (NULL);

	dirs = get_path_dir(*path + 5);
	head = dirs;

	while (dirs)
	{
		temp = malloc(_strlen(dirs->dir) + _strlen(command) + 2);
		if (!temp)
			return (NULL);

		_strcpy(temp, dirs->dir);
		_strcat(temp, "/");
		_strcat(temp, command);

		if (stat(temp, &st) == 0)
		{
			free_list(head);
			return (temp);
		}

		dirs = dirs->next;
		free(temp);
	}

	free_list(head);

	return (NULL);
}

/**
 * fill_path_dir - Fill missing paths in the PATH variable with the current directory.
 * @path: The PATH variable.
 *
 * Return: A new string with the missing paths filled with the current directory.
 */
char *fill_path_dir(char *path)
{
	int m_m, length = 0;
	char *path_copy, *pwd;

	pwd = *(_getenv("PWD")) + 4;
	for (m_m = 0; path[m_m]; m_m++)
	{
		if (path[m_m] == ':')
		{
			if (path[m_m + 1] == ':' || m_m == 0 || path[m_m + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	path_copy = malloc(sizeof(char) * (length + 1));
	if (!path_copy)
		return (NULL);
	path_copy[0] = '\0';
	for (m_m = 0; path[m_m]; m_m++)
	{
		if (path[m_m] == ':')
		{
			if (m_m == 0)
			{
				_strcat(path_copy, pwd);
				_strcat(path_copy, ":");
			}
			else if (path[m_m + 1] == ':' || path[m_m + 1] == '\0')
			{
				_strcat(path_copy, ":");
				_strcat(path_copy, pwd);
			}
			else
				_strcat(path_copy, ":");
		}
		else
		{
			_strncat(path_copy, &path[m_m], 1);
		}
	}
	return (path_copy);
}

/**
 * get_path_dir - Get a list of directories from the PATH variable.
 * @path: The PATH variable.
 *
 * Return: A linked list of directories from the PATH variable.
 */
list_t *get_path_dir(char *path)
{
	int m_m;
	char **dirs, *path_copy;
	list_t *head = NULL;

	path_copy = fill_path_dir(path);
	if (!path_copy)
		return (NULL);
	dirs = _strtok(path_copy, ":");
	free(path_copy);
	if (!dirs)
		return (NULL);

	for (m_m = 0; dirs[m_m]; m_m++)
	{
		if (add_node_end(&head, dirs[m_m]) == NULL)
		{
			free_list(head);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (head);
}

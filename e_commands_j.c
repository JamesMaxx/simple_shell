#include "main.h"

/**
 * get_builtin - Get the corresponding built-in function for a command.
 * @command: The command to find the built-in function for.
 *
 * Return: Pointer to the corresponding built-in function.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", e_j_exit },
		{ "env", e_j_env },
		{ "setenv", e_j_setenv },
		{ "unsetenv", e_j_unsetenv },
		{ "cd", e_j_cd },
		{ "alias", e_j_alias },
		{ "help", e_j_help },
		{ NULL, NULL }
	};
	int m_m;

	for (m_m = 0; funcs[m_m].name; m_m++)
	{
		if (_strcmp(funcs[m_m].name, command) == 0)
			break;
	}
	return (funcs[m_m].f);
}

/**
 * e_j_exit - Exit the shell with a given status code.
 * @args: The arguments provided by the user (containing the status code).
 * @front: A double pointer to the beginning of args.
 *
 * Return: Nothing.
 */
int e_j_exit(char **args, char **front)
{
	int m_m, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			m_m = 1;
			len_of_int++;
		}
		for (; args[0][m_m]; m_m++)
		{
			if (m_m <= len_of_int && args[0][m_m] >= '0' && args[0][m_m] <= '9')
				num = (num * 10) + (args[0][m_m] - '0');
			else
				return (create_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (create_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(num);
}

/**
 * e_j_cd - Change the current working directory.
 * @args: The arguments provided by the user (containing the new directory).
 * @front: A double pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

/**
 * e_j_cd - Change the current working directory.
 * @args: The arguments provided by the user (containing the new directory).
 * @front: A double pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on error.
 */
int e_j_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info;
	char *oldpwd = NULL;
	char *pwd = NULL;
	int ret = 0;

	oldpwd = getcwd(NULL, 0);
	if (oldpwd == NULL)
	{
		perror("getcwd");
		return -1;
	}

	if (args[0] != NULL)
	{
		if (strcmp(args[0], "-") == 0)
		{
			char *prev_dir = getenv("OLDPWD");
			if (prev_dir != NULL)
			{
				ret = chdir(prev_dir);
				if (ret != 0)
					perror("chdir");
			}
			else
			{
				fprintf(stderr, "OLDPWD not set\n");
				ret = -1;
			}
		}
		else if (strcmp(args[0], "--") == 0)
		{
			char *home_dir = getenv("HOME");
			if (home_dir != NULL)
			{
				ret = chdir(home_dir);
				if (ret != 0)
					perror("chdir");
			}
			else
			{
				fprintf(stderr, "HOME not set\n");
				ret = -1;
			}
		}
		else
		{
			ret = chdir(args[0]);
			if (ret != 0)
				perror("chdir");
		}
	}
	else
	{
		char *home_dir = getenv("HOME");
		if (home_dir != NULL)
		{
			ret = chdir(home_dir);
			if (ret != 0)
				perror("chdir");
		}
		else
		{
			fprintf(stderr, "HOME not set\n");
			ret = -1;
		}
	}

	if (ret == 0)
	{
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
		{
			perror("getcwd");
			ret = -1;
		}

		dir_info = malloc(sizeof(char *) * 2);
		if (dir_info == NULL)
		{
			perror("malloc");
			ret = -1;
		}
		else
		{
			dir_info[0] = "OLDPWD";
			dir_info[1] = oldpwd;
			if (e_j_setenv(dir_info, front) == -1)
				ret = -1;

			dir_info[0] = "PWD";
			dir_info[1] = pwd;
			if (e_j_setenv(dir_info, front) == -1)
				ret = -1;

			free(dir_info);
		}
	}

	free(oldpwd);
	free(pwd);

	return ret;
}

/**
 * help_all - Display help information for all shell built-in commands
 */
void help_all(void)
{
    printf("Help for all built-in commands\n");
}

/**
 * help_alias - Display help information for the 'alias' built-in command
 */
void help_alias(void)
{
    printf("Help for 'alias' command\n");
}

/**
 * help_cd - Display help information for the 'cd' built-in command
 */
void help_cd(void)
{
    printf("Help for 'cd' command\n");
}

/**
 * help_exit - Display help information for the 'exit' built-in command
 */
void help_exit(void)
{
    printf("Help for 'exit' command\n");
}

/**
 * help_help - Display help information for the 'help' built-in command
 */
void help_help(void)
{
    printf("Help for 'help' command\n");
}


/**
 * e_j_help - Display help information about shell built-in commands
 * @args: Represents user's arguments for command to help
 * @front: Represents double pointer to @args beginning
 *
 * Return: Always 0
 */
int e_j_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		help_alias();
	else if (_strcmp(args[0], "cd") == 0)
		help_cd();
	else if (_strcmp(args[0], "exit") == 0)
		help_exit();
	else if (_strcmp(args[0], "env") == 0)
		help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}
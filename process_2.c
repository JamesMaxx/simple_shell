#include "main.h"
/**
 * handle_line - Modifies the command line to handle logical operators
 *
 * @line: Represents pointer to a pointer to the original command line string
 * @read: The number of characters read from the input
 */
void handle_line(char **line, ssize_t read)
{
	char *old_line, *new_line;
	char previous, current, next;
	size_t m_m, z;
	ssize_t new_len;

	new_len = get_new_len(*line);
	if (new_len == read - 1)
		return;
	new_line = malloc(new_len + 1);
	if (!new_line)
		return;
	z = 0;
	old_line = *line;
	for (m_m = 0; old_line[m_m]; m_m++)
	{
		current = old_line[m_m];
		next = old_line[m_m + 1];
		if (m_m != 0)
		{
			previous = old_line[m_m - 1];
			if (current == ';')
			{
				if (next == ';' && previous != ' ' && previous != ';')
				{
					new_line[z++] = ' ';
					new_line[z++] = ';';
					continue;
				}
				else if (previous == ';' && next != ' ')
				{
					new_line[z++] = ';';
					new_line[z++] = ' ';
					continue;
				}
				if (previous != ' ')
					new_line[z++] = ' ';
				new_line[z++] = ';';
				if (next != ' ')
					new_line[z++] = ' ';
				continue;
			}
			else if (current == '&')
			{
				if (next == '&' && previous != ' ')
					new_line[z++] = ' ';
				else if (previous == '&' && next != ' ')
				{
					new_line[z++] = '&';
					new_line[z++] = ' ';
					continue;
				}
			}
			else if (current == '|')
			{
				if (next == '|' && previous != ' ')
					new_line[z++] = ' ';
				else if (previous == '|' && next != ' ')
				{
					new_line[z++] = '|';
					new_line[z++] = ' ';
					continue;
				}
			}
		}
		else if (current == ';')
		{
			if (m_m != 0 && old_line[m_m - 1] != ' ')
				new_line[z++] = ' ';
			new_line[z++] = ';';
			if (next != ' ' && next != ';')
				new_line[z++] = ' ';
			continue;
		}
		new_line[z++] = old_line[m_m];
	}
	new_line[z] = '\0';

	free(*line);
	*line = new_line;
}
/**
 * get_new_len - Calculates new command-line lenght post  handling logical operators
 *
 * @line: The command line string.
 *
 * Return: The new length of the command line.
 */
ssize_t get_new_len(char *line)
{
	size_t m_m;
	ssize_t new_len = 0;
	char current, next;

	for (m_m = 0; line[m_m]; m_m++)
	{
		current = line[m_m];
		next = line[m_m + 1];
		if (current == '#')
		{
			if (m_m == 0 || line[m_m - 1] == ' ')
			{
				line[m_m] = '\0';
				break;
			}
		}
		else if (m_m != 0)
		{
			if (current == ';')
			{
				if (next == ';' && line[m_m - 1] != ' ' && line[m_m - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (line[m_m - 1] == ';' && next != ' ')
				{
					new_len += 2;
					continue;
				}
				if (line[m_m - 1] != ' ')
					new_len++;
				if (next != ' ')
					new_len++;
			}
			else
				logical_ops(&line[m_m], &new_len);
		}
		else if (current == ';')
		{
			if (m_m != 0 && line[m_m - 1] != ' ')
				new_len++;
			if (next != ' ' && next != ';')
				new_len++;
		}
		new_len++;
	}
	return (new_len);
}
/**
 * logical_ops - Handles logical operators and adjusts the new length accordingly.
 *
 * @line: Represents pointer to character in the command line string being analyzed.
 * @new_len: Represents pointer to new commandlinelength update
 */
void logical_ops(char *line, ssize_t *new_len)
{
	char previous, current, next;

	previous = *(line - 1);
	current = *line;
	next = *(line + 1);

	if (current == '&')
	{
		if (next == '&' && previous != ' ')
			(*new_len)++;
		else if (previous == '&' && next != ' ')
			(*new_len)++;
	}
	else if (current == '|')
	{
		if (next == '|' && previous != ' ')
			(*new_len)++;
		else if (previous == '|' && next != ' ')
			(*new_len)++;
	}
}

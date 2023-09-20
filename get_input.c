#include "main.h"

/**
 * _realloc - Reallocate memory for a block of memory.
 * @ptr: Pointer to the original memory block.
 * @old_size: Old size of the memory block.
 * @new_size: New size to reallocate.
 *
 * Return: Pointer to the reallocated memory block on success,
 *         NULL on failure or if @new_size is 0 (with ptr freed).
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mem = malloc(sizeof(*ptr_copy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * assign_lineptr - Assign lineptr and size to buffer or free buffer if needed.
 * @lineptr: Pointer to the line buffer.
 * @u: Pointer to the size of the buffer.
 * @buffer: Pointer to the buffer containing the line.
 * @p: The number of characters read.
 */
void assign_lineptr(char **lineptr, size_t *u, char *buffer, size_t p)
{
	if (*lineptr == NULL)
	{
		if (p > 120)
			*u = p;
		else
			*u = 120;
		*lineptr = buffer;
	}
	else if (*u < p)
	{
		if (p > 120)
			*u = p;
		else
			*u = 120;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * _getline - Get a line from a stream.
 * @lineptr: Pointer to the buffer storing the line.
 * @u: Pointer to the size of the buffer.
 * @stream: File stream to read from.
 *
 * Return: The number of characters read (including '\0'), -1 on failure.
 */
ssize_t _getline(char **lineptr, size_t *u, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char p = 'x', *buffer;
	int t;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (p != '\n')
	{
		t = read(STDIN_FILENO, &p, 1);
		if (t == -1 || (t == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (t == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = _realloc(buffer, input, input + 1);

		buffer[input] = p;
		input++;
	}
	buffer[input] = '\0';

	assign_lineptr(lineptr, u, buffer, input);

	ret = input;
	if (t != 0)
		input = 0;
	return (ret);
}

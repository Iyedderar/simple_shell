
#include "shell.h"

void *_realloc(void *ptr, unsigned int old, unsigned int new);
void ptr_assign(char **lineptr, size_t *n, char *buff, size_t f);
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);

/**
 * _realloc - Reallocates a memory block using malloc and free.
 * @ptr: A pointer to the memory to be reallocated.
 * @old: The size in bytes of the allocated space for ptr.
 * @new: The size in bytes for the new memory block.
 *
 * Return: If new == old - ptr.
 *         If new == 0 and ptr is not NULL - NULL.
 *         Otherwise - a pointer to the reallocated memory block.
 */
void *_realloc(void *ptr, unsigned int old, unsigned int new)
{
	void *memory;
	char *copy, *fill;
	unsigned int i;

	if (new == old)
		return (ptr);

	if (ptr == NULL)
	{
		memory = malloc(new);
		if (memory == NULL)
			return (NULL);

		return (memory);
	}

	if (new == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	copy = ptr;
	memory = malloc(sizeof(*copy) * new);
	if (memory == NULL)
	{
		free(ptr);
		return (NULL);
	}

	fill = memory;

	for (i = 0; i < old && i < new; i++)
		fill[i] = *copy++;

	free(ptr);
	return (memory);
}

/**
 * ptr_assign - Reassigns the lineptr variable for _getline.
 * @lineptr: A buff to store an input string.
 * @n: The size of lineptr.
 * @buff: The string to assign to lineptr.
 * @f: The size of buff.
 */
void ptr_assign(char **lineptr, size_t *n, char *buff, size_t f)
{
	if (*lineptr == NULL)
	{
		if (f > 120)
			*n = f;
		else
			*n = 120;
		*lineptr = buff;
	}
	else if (*n < f)
	{
		if (f > 120)
			*n = f;
		else
			*n = 120;
		*lineptr = buff;
	}
	else
	{
		_strcpy(*lineptr, buff);
		free(buff);
	}
}

/**
 * _getline - Reads input from a stream.
 * @lineptr: A buff to store the input.
 * @n: The size of lineptr.
 * @stream: The stream to read from.
 *
 * Return: The number of bytes read.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buff;
	int r;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buff = malloc(sizeof(char) * 120);
	if (!buff)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buff);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buff = _realloc(buff, input, input + 1);

		buff[input] = c;
		input++;
	}
	buff[input] = '\0';

	ptr_assign(lineptr, n, buff, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}

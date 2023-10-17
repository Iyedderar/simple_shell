/*
 * File: split.c
 */

#include "shell.h"

int token_lenth(char *str, char *dlm);
int count_touken(char *str, char *dlm);
char **_strtok(char *line, char *dlm);

/**
 * token_lenth - Locates the delimiter index marking the end
 *             of the first token contained within a string.
 * @str: A string for searching.
 * @dlm: A delimiter character.
 *
 * Return: The index of the delimiter marks the end of
 *         the intitial token pointed to be str.
 */
int token_lenth(char *str, char *dlm)
{
	int index = 0, len = 0;

	while (*(str + index) && *(str + index) != *dlm)
	{
		len++;
		index++;
	}

	return (len);
}

/**
 * count_touken - Counts  delimited words number
 *                contained within a string.
 * @str: A string for searching.
 * @dlm: A delimiter character.
 *
 * Return: Number of the words contained within str.
 */
int count_touken(char *str, char *dlm)
{
	int index, tokens = 0, len = 0;

	for (index = 0; *(str + index); index++)
		len++;

	for (index = 0; index < len; index++)
	{
		if (*(str + index) != *dlm)
		{
			tokens++;
			index += token_lenth(str + index, dlm);
		}
	}

	return (tokens);
}

/**
 * _strtok - Tokenizes a string.
 * @line: The string.
 * @dlm: The delimiter character to tokenize the string by.
 *
 * Return: A pointer for the  array containing the tokenized words.
 */
char **_strtok(char *line, char *dlm)
{
	char **ptr;
	int index = 0, tokens, t, letters, l;

	tokens = count_touken(line, dlm);
	if (tokens == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (tokens + 2));
	if (!ptr)
		return (NULL);

	for (t = 0; t < tokens; t++)
	{
		while (line[index] == *dlm)
			index++;

		letters = token_lenth(line + index, dlm);

		ptr[t] = malloc(sizeof(char) * (letters + 1));
		if (!ptr[t])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			return (NULL);
		}

		for (l = 0; l < letters; l++)
		{
			ptr[t][l] = line[index];
			index++;
		}

		ptr[t][l] = '\0';
	}
	ptr[t] = NULL;
	ptr[t + 1] = NULL;

	return (ptr);
}


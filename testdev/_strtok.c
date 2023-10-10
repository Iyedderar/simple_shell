#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

char *_strtok(char *str, const char *delim)
{
	static char *input;
	char *result;
	int i, j;

	
	if (str != NULL)
		input = str;


	if (input == NULL)
		return NULL;

	// Allocate memory for the result string
	result = (char *)malloc(sizeof(char) * (strlen(input) + 1));
	if (result == NULL)
		return NULL;

	for (i = 0; input[i]; i++)
	{
		int is_delim = 0; // Flag to check if the character is a delimiter

		// Check if the current character is a delimiter
		for (j = 0; delim[j]; j++)
		{
			if (input[i] == delim[j])
			{
				is_delim = 1;
				break;
			}
		}

		if (!is_delim)
		{
			// Copy non-delimiter character to the result
			result[i] = input[i];
		}
		else
		{
			// Null-terminate the result and update input pointer
			result[i] = '\0';
			input = input + i + 1;
			return result;
		}
	}

	// Null-terminate the result when the input string ends
	result[i] = '\0';
	input = NULL; // Reset input pointer when no more tokens are found
	return result;
}
int main(void)
{
	char *token = _strtok("flipped - mofo - dicki", "-");
	while (token)
	{
		printf("%s\n", token);
		token = _strtok(NULL, "-");
	}

	return 0;
}
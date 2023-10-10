#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
char **splitter(char *command)
{
	int i = 0;
	char *token, **tokens = NULL;
	token = strtok(command, " \n\t");
	while (token != NULL)
	{
		tokens = realloc(tokens, (i++) * sizeof(char *));
		if (tokens == NULL)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		tokens[i - 1] = token;
		token = strtok(NULL, " \n\t");
	}
	for (i = 0; tokens[i]; i++)
		printf("%s\n", tokens[i]);
	return(tokens);
}
int main(int arc, char **arv)
{
	(void)arc;
	char *buffer = NULL;
	char **tokens = NULL;
	size_t size = 0;
	int n_chars = 0;
	write(1, "$ ", 2);
	n_chars = getline(&buffer, &size, stdin);
	if (n_chars == EOF)
	{
		perror("getline");
	}
	buffer[n_chars - 1] = '\0';
	tokens = splitter(buffer);
}
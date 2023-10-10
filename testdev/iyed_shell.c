#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>



int main (void)
{
    int i = 0,status;
	char *buffer = NULL, *token, **tokens;
	size_t size = 0,n_chars;
	pid_t pid;


	while(1)
	{
		write(1,"$ ", 2);
		n_chars = getline(&buffer,&size, stdin);
		tokens = malloc(1024 * sizeof(char *));
		token= strtok(buffer, " \t\n");
		while (token)
		{
			tokens[i] = token;
			token = strtok(NULL, " \t\n");
			i++;
		}
		tokens[i] = NULL;
		pid = fork();
		if (pid == 0)
		{
			if (execve(tokens[0],tokens,NULL) == -1);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
		}
		i = 0;
		free(tokens);
	}
}
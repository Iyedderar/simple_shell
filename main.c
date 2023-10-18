/*
 * File: main.c
 */

#include "shell.h"

void sig_handler(int sg);
int execute(char **rgs, char **fnt);

/**
 * sig_handler - A new prmpt will be printed upon a signal.
 * @sg: The signal.
 */
void sig_handler(int sg)
{
	char *nw_prmpt = "\n$ ";

	(void)sg;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, nw_prmpt, 3);
}

/**
 * execute - Execute the cmnd in a child process.
 * @rgs: An array of arguments.
 * @fnt: A double pointer to the starting of rgs.
 *
 * Return: If thier is an error - the error code.
 *         Else - The value of last executed cmnd.
 */
int execute(char **rgs, char **fnt)
{
	pid_t chld_pid;
	int stts, flg = 0, ret = 0;
	char *cmnd = rgs[0];

	if (cmnd[0] != '/' && cmnd[0] != '.')
	{
		flg = 1;
		cmnd = get_loct(cmnd);
	}

	if (!cmnd || (access(cmnd, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (create_error(rgs, 126));
		else
			ret = (create_error(rgs, 127));
	}
	else
	{
		chld_pid = fork();
		if (chld_pid == -1)
		{
			if (flg)
				free(cmnd);
			perror("Error child:");
			return (1);
		}
		if (chld_pid == 0)
		{
			execve(cmnd, rgs, environ);
			if (errno == EACCES)
				ret = (create_error(rgs, 126));
			free_env();
			free_rgs(rgs, fnt);
			freealias_list(aliases);
			_exit(ret);
		}
		else
		{
			wait(&stts);
			ret = WEXITSTATUS(stts);
		}
	}
	if (flg)
		free(cmnd);
	return (ret);
}

/**
 * main - Run simple UNIX cmnd interpreter.
 * @argc: A number of arguments given to the program.
 * @argv: An array of the pointers for the arguments.
 *
 * Return: The last executed cmnd value.
 */
int main(int argc, char *argv[])
{
	int ret = 0, retn;
	int *exe_ret = &retn;
	char *prmpt = "$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, sig_handler);

	*exe_ret = 0;
	environ = _copyenv();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		ret = proc_flie_cmnd(argv[1], exe_ret);
		free_env();
		freealias_list(aliases);
		return (*exe_ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handle_rgs(exe_ret);
		free_env();
		freealias_list(aliases);
		return (*exe_ret);
	}

	while (1)
	{
		write(STDOUT_FILENO, prmpt, 2);
		ret = handle_rgs(exe_ret);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_env();
			freealias_list(aliases);
			exit(*exe_ret);
		}
	}

	free_env();
	freealias_list(aliases);
	return (*exe_ret);
}


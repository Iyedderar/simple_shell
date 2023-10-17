/*
 * File: input_helpers.c
 */

#include "shell.h"

char *get_rgs(char *lns, int *exe_ret);
int call_rgs(char **rgs, char **fnt, int *exe_ret);
int run_rgs(char **rgs, char **fnt, int *exe_ret);
int handle_rgs(int *exe_ret);
int check_rgs(char **rgs);

/**
 * get_rgs - Get the command from a standard input.
 * @lns: A buffer for store the command.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: If thier is an error - NULL.
 *         Else - a pointer for the stored command.
 */
char *get_rgs(char *lns, int *exe_ret)
{
	size_t n = 0;
	ssize_t rd;
	char *prompt = "$ ";

	if (lns)
		free(lns);

	rd = _getline(&lns, &n, STDIN_FILENO);
	if (rd == -1)
		return (NULL);
	if (rd == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_rgs(lns, exe_ret));
	}

	lns[rd - 1] = '\0';
	variable_rp(&lns, exe_ret);
	handle_ln(&lns, rd);

	return (lns);
}

/**
 * call_rgs - Calls operators and partitions them from commands.
 * @rgs: An array of arguments.
 * @fnt: A double pointer for beginning of rgs.
 * @exe_ret: The return value of the parent process last executed command.
 *
 * Return: The return value for the last executed command.
 */
int call_rgs(char **rgs, char **fnt, int *exe_ret)
{
	int ret, index;

	if (!rgs[0])
		return (*exe_ret);
	for (index = 0; rgs[index]; index++)
	{
		if (_strncmp(rgs[index], "||", 2) == 0)
		{
			free(rgs[index]);
			rgs[index] = NULL;
			rgs = replace_aliases(rgs);
			ret = run_rgs(rgs, fnt, exe_ret);
			if (*exe_ret != 0)
			{
				rgs = &rgs[++index];
				index = 0;
			}
			else
			{
				for (index++; rgs[index]; index++)
					free(rgs[index]);
				return (ret);
			}
		}
		else if (_strncmp(rgs[index], "&&", 2) == 0)
		{
			free(rgs[index]);
			rgs[index] = NULL;
			rgs = replace_aliases(rgs);
			ret = run_rgs(rgs, fnt, exe_ret);
			if (*exe_ret == 0)
			{
				rgs = &rgs[++index];
				index = 0;
			}
			else
			{
				for (index++; rgs[index]; index++)
					free(rgs[index]);
				return (ret);
			}
		}
	}
	rgs = replace_aliases(rgs);
	ret = run_rgs(rgs, fnt, exe_ret);
	return (ret);
}

/**
 * run_rgs - Calls the execution of a command.
 * @rgs: An array of arguments.
 * @fnt: A double pointer for beginning of rgs.
 * @exe_ret: The return value of the parent process last executed command.
 *
 * Return: The return value of the last executed command.
 */
int run_rgs(char **rgs, char **fnt, int *exe_ret)
{
	int ret, ik;
	int (*builtin)(char **rgs, char **fnt);

	builtin = get_builtin(rgs[0]);

	if (builtin)
	{
		ret = builtin(rgs + 1, fnt);
		if (ret != EXIT)
			*exe_ret = ret;
	}
	else
	{
		*exe_ret = execute(rgs, fnt);
		ret = *exe_ret;
	}

	hist++;

	for (ik = 0; rgs[ik]; ik++)
		free(rgs[ik]);

	return (ret);
}

/**
 * handle_rgs - Get and  call, and run the execution of the command.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is rd - END_OF_FILE (-2).
 *         If an input that cannot be tokenized - -1.
 *         Else - an exit value for the last executed command.
 */
int handle_rgs(int *exe_ret)
{
	int ret = 0, index;
	char **rgs, *lns = NULL, **fnt;

	lns = get_rgs(lns, exe_ret);
	if (!lns)
		return (END_OF_FILE);

	rgs = _strtok(lns, " ");
	free(lns);
	if (!rgs)
		return (ret);
	if (check_rgs(rgs) != 0)
	{
		*exe_ret = 2;
		free_args(rgs, rgs);
		return (*exe_ret);
	}
	fnt = rgs;

	for (index = 0; rgs[index]; index++)
	{
		if (_strncmp(rgs[index], ";", 1) == 0)
		{
			free(rgs[index]);
			rgs[index] = NULL;
			ret = call_rgs(rgs, fnt, exe_ret);
			rgs = &rgs[++index];
			index = 0;
		}
	}
	if (rgs)
		ret = call_rgs(rgs, fnt, exe_ret);

	free(fnt);
	return (ret);
}

/**
 * check_rgs - Check for any leading ';', ';;', '&&', or '||'.
 * @rgs: 2D pointer for tokenized command and argument.
 *
 * Return: If one of ';', '&&', or '||' is placed  in an invalid position - 2.
 *	   Else - 0.
 */
int check_rgs(char **rgs)
{
	size_t ik;
	char *cur, *nex;

	for (ik = 0; rgs[ik]; ik++)
	{
		cur = rgs[ik];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (ik == 0 || cur[1] == ';')
				return (create_error(&rgs[ik], 2));
			nex = rgs[ik + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&rgs[ik + 1], 2));
		}
	}
	return (0);
}


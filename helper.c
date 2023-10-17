/*
 * File: helper.c
 */

#include "shell.h"

void free_rgs(char **rgs, char **fnt);
char *getpid(void);
char *getent_val(char *start, int lnth);
void variable_rp(char **rgs, int *exe_rp);

/**
 * free_rgs - Free the memory by rgs.
 * @rgs: double pointer that contain commands/arguments.
 * @fnt: A double pointer for the start of rgs.
 */
void free_rgs(char **rgs, char **fnt)
{
	size_t ik;

	for (ik = 0; rgs[ik] || rgs[ik + 1]; ik++)
		free(rgs[ik]);

	free(fnt);
}

/**
 * getpid - Get the process ID.
 * Description: Open a stat file, a space-delimited file that contain
 *              the information of the process. The PID is the
 *              first word in the file. The function read the PID
 *              and it replace the space in the end with a \0 byte.
 *
 * Return: The process ID or NULL.
 */
char *getpid(void)
{
	size_t ik = 0;
	char *bffr;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Can not read file");
		return (NULL);
	}
	bffr = malloc(120);
	if (!bffr)
	{
		close(file);
		return (NULL);
	}
	read(file, bffr, 120);
	while (bffr[ik] != ' ')
		ik++;
	bffr[ik] = '\0';

	close(file);
	return (bffr);
}

/**
 * getent_val - Get the value that corresponding to an environmental variable.
 * @start: The environmental variable to search for.
 * @lnth: The length of the environmental variable.
 *
 * Return: If the variable isnt found - an empty string.
 *         Else - the value of the environmental variable.
 *
 * Description: Variables are stored in the format VARIABLE=VALUE.
 */
char *getent_val(char *start, int lnth)
{
	char **var_addr;
	char *replacement = NULL, *tmp, *var;

	var = malloc(lnth + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, start, lnth);

	var_addr = _getenv(var);
	free(var);
	if (var_addr)
	{
		tmp = *var_addr;
		while (*tmp != '=')
			tmp++;
		tmp++;
		replacement = malloc(_strlen(tmp) + 1);
		if (replacement)
			_strcpy(replacement, tmp);
	}

	return (replacement);
}

/**
 * variable_rp - Handles variable replacement.
 * @line: A double pointer that contain the command and the arguments.
 * @exe_rp: A pointer to the return value of the last executed command.
 *
 * Description: Replaces the signs $$ with the PID, the return value
 *              of the last executed program with $?,and envrionmental
 *              variables preceded with $ with their value.
 */
void variable_rp(char **line, int *exe_rp)
{
	int jk, kk = 0, lnth;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (jk = 0; old_line[jk]; jk++)
	{
		if (old_line[jk] == '$' && old_line[jk + 1] &&
				old_line[jk + 1] != ' ')
		{
			if (old_line[jk + 1] == '$')
			{
				replacement = getpid();
				kk = jk + 2;
			}
			else if (old_line[jk + 1] == '?')
			{
				replacement = _itoa(*exe_rp);
				kk = jk + 2;
			}
			else if (old_line[jk + 1])
			{
				/* extract the name of the variable to search for */
				for (kk = jk + 1; old_line[kk] &&
						old_line[kk] != '$' &&
						old_line[kk] != ' '; kk++)
					;
				lnth = kk - (jk + 1);
				replacement = getent_val(&old_line[jk + 1], lnth);
			}
			new_line = malloc(jk + _strlen(replacement)
					  + _strlen(&old_line[kk]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, jk);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[kk]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			jk = -1;
		}
	}
}


#include "shell.h"

int cannot_open(char *flie_pth);
int proc_flie_cmnd(char *flie_pth, int *exe_ret);

/**
 * cannot_open - If the file does not exist, print a cant open err.
 * @flie_pth: Path to the supposed file.
 *
 * Return: 127.
 */

int cannot_open(char *flie_pth)
{
	char *err, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	len = _strlen(name) + _strlen(hist_str) + _strlen(flie_pth) + 16;
	err = malloc(sizeof(char) * (len + 1));
	if (!err)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(err, name);
	_strcat(err, ": ");
	_strcat(err, hist_str);
	_strcat(err, ": Can not open ");
	_strcat(err, flie_pth);
	_strcat(err, "\n");

	free(hist_str);
	write(STDERR_FILENO, err, len);
	free(err);
	return (127);
}

/**
 * proc_flie_cmnd - Take the file and attempt for runnig the command stored.
 * @flie_pth: Path to the file.
 * @exe_ret: Return the last executed command value.
 *
 * Return: If the file could not opened - 127.
 *	   If malloc fail - -1.
 *	   Else the return value of the last command ran.
 */
int proc_flie_cmnd(char *flie_pth, int *exe_ret)
{
	ssize_t file, b_read, i;
	unsigned int lns_size = 0;
	unsigned int old_size = 120;
	char *lns, **rgs, **fnt;
	char buffer[120];
	int ret;

	hist = 0;
	file = open(flie_pth, O_RDONLY);
	if (file == -1)
	{
		*exe_ret = cannot_open(flie_pth);
		return (*exe_ret);
	}
	lns = malloc(sizeof(char) * old_size);
	if (!lns)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && lns_size == 0)
			return (*exe_ret);
		buffer[b_read] = '\0';
		lns_size += b_read;
		lns = _realloc(lns, old_size, lns_size);
		_strcat(lns, buffer);
		old_size = lns_size;
	} while (b_read);
	for (i = 0; lns[i] == '\n'; i++)
		lns[i] = ' ';
	for (; i < lns_size; i++)
	{
		if (lns[i] == '\n')
		{
			lns[i] = ';';
			for (i += 1; i < lns_size && lns[i] == '\n'; i++)
				lns[i] = ' ';
		}
	}
	variable_rp(&lns, exe_ret);
	handle_ln(&lns, lns_size);
	rgs = _strtok(lns, " ");
	free(lns);
	if (!rgs)
		return (0);
	if (check_rgs(rgs) != 0)
	{
		*exe_ret = 2;
		free_rgs(rgs, rgs);
		return (*exe_ret);
	}
	fnt = rgs;

	for (i = 0; rgs[i]; i++)
	{
		if (_strncmp(rgs[i], ";", 1) == 0)
		{
			free(rgs[i]);
			rgs[i] = NULL;
			ret = call_rgs(rgs, fnt, exe_ret);
			rgs = &rgs[++i];
			i = 0;
		}
	}

	ret = call_rgs(rgs, fnt, exe_ret);

	free(fnt);
	return (ret);
}


/*
 * File: locate.c
 */

#include "shell.h"

char *fillpath_dir(char *pth);
list_t *getpath_dir(char *pth);

/**
 * get_loct - Locates the cmnd in a path.
 * @cmnd: The cmnd to locate.
 *
 * Return: If thir is an error - NULL.
 *         Else - the pathname of the cmnd.
 */
char *get_loct(char *cmnd)
{
	char **pth, *temp;
	list_t *dirs, *hd;
	struct stat st;

	pth = _getenv("PATH");
	if (!pth || !(*pth))
		return (NULL);

	dirs = getpath_dir(*pth + 5);
	hd = dirs;

	while (dirs)
	{
		temp = malloc(_strlen(dirs->dir) + _strlen(cmnd) + 2);
		if (!temp)
			return (NULL);

		_strcpy(temp, dirs->dir);
		_strcat(temp, "/");
		_strcat(temp, cmnd);

		if (stat(temp, &st) == 0)
		{
			free_list(hd);
			return (temp);
		}

		dirs = dirs->next;
		free(temp);
	}

	free_list(hd);

	return (NULL);
}

/**
 * fillpath_dir - Copie the pth(path)but replace leading/sandwiched/trailing
 *		   colons (:) with the working directory.
 * @pth: The colon-separated list of directories.
 *
 * Return: A copy of the pth(path) with any leading/sandwiched/trailing
 *	  colons replaced with the working directory.
 */
char *fillpath_dir(char *pth)
{
	int i, length = 0;
	char *pth_cpy, *pwd;

	pwd = *(_getenv("PWD")) + 4;
	for (i = 0; pth[i]; i++)
	{
		if (pth[i] == ':')
		{
			if (pth[i + 1] == ':' || i == 0 || pth[i + 1] == '\0')
				length += _strlen(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	pth_cpy = malloc(sizeof(char) * (length + 1));
	if (!pth_cpy)
		return (NULL);
	pth_cpy[0] = '\0';
	for (i = 0; pth[i]; i++)
	{
		if (pth[i] == ':')
		{
			if (i == 0)
			{
				_strcat(pth_cpy, pwd);
				_strcat(pth_cpy, ":");
			}
			else if (pth[i + 1] == ':' || pth[i + 1] == '\0')
			{
				_strcat(pth_cpy, ":");
				_strcat(pth_cpy, pwd);
			}
			else
				_strcat(pth_cpy, ":");
		}
		else
		{
			_strncat(pth_cpy, &pth[i], 1);
		}
	}
	return (pth_cpy);
}

/**
 * getpath_dir - Tokenizes a colon-separated list of
 *                directories into the list_s linked.
 * @pth: The colon-separated list of directories.
 *
 * Return: A pointer to the initialized linked list.
 */
list_t *getpath_dir(char *pth)
{
	int index;
	char **dirs, *pth_cpy;
	list_t *hd = NULL;

	pth_cpy = fillpath_dir(pth);
	if (!pth_cpy)
		return (NULL);
	dirs = _strtok(pth_cpy, ":");
	free(pth_cpy);
	if (!dirs)
		return (NULL);

	for (index = 0; dirs[index]; index++)
	{
		if (add_node_end(&hd, dirs[index]) == NULL)
		{
			free_list(hd);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (hd);
}


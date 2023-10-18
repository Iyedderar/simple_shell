/*
 * File: helpers_2.c
 */

#include "shell.h"

void hndle_ln(char **lns, ssize_t rd);
ssize_t getnewlen(char *lns);
void logical_ops(char *lns, ssize_t *new_len);

/**
 * hndle_ln - Assigns a lns rd from standard input when needed.
 * @lns: A pointer to a lns rd from standard input.
 * @rd: The length of lns(lines).
 *
 * Description: The spaces are used to separate ";", "||", and "&&".
 *              Replace "#" with '\0'.
 */
void hndle_ln(char **lns, ssize_t rd)
{
	char *oldline, *newline;
	char prvs, crrnt, nxt;
	size_t ik, jk;
	ssize_t new_len;

	new_len = getnewlen(*lns);
	if (new_len == rd - 1)
		return;
	newline = malloc(new_len + 1);
	if (!newline)
		return;
	jk = 0;
	oldline = *lns;
	for (ik = 0; oldline[ik]; ik++)
	{
		crrnt = oldline[ik];
		nxt = oldline[ik + 1];
		if (ik != 0)
		{
			prvs = oldline[ik - 1];
			if (crrnt == ';')
			{
				if (nxt == ';' && prvs != ' ' && prvs != ';')
				{
					newline[jk++] = ' ';
					newline[jk++] = ';';
					continue;
				}
				else if (prvs == ';' && nxt != ' ')
				{
					newline[jk++] = ';';
					newline[jk++] = ' ';
					continue;
				}
				if (prvs != ' ')
					newline[jk++] = ' ';
				newline[jk++] = ';';
				if (nxt != ' ')
					newline[jk++] = ' ';
				continue;
			}
			else if (crrnt == '&')
			{
				if (nxt == '&' && prvs != ' ')
					newline[jk++] = ' ';
				else if (prvs == '&' && nxt != ' ')
				{
					newline[jk++] = '&';
					newline[jk++] = ' ';
					continue;
				}
			}
			else if (crrnt == '|')
			{
				if (nxt == '|' && prvs != ' ')
					newline[jk++]  = ' ';
				else if (prvs == '|' && nxt != ' ')
				{
					newline[jk++] = '|';
					newline[jk++] = ' ';
					continue;
				}
			}
		}
		else if (crrnt == ';')
		{
			if (ik != 0 && oldline[ik - 1] != ' ')
				newline[jk++] = ' ';
			newline[jk++] = ';';
			if (nxt != ' ' && nxt != ';')
				newline[jk++] = ' ';
			continue;
		}
		newline[jk++] = oldline[ik];
	}
	newline[jk] = '\0';

	free(*lns);
	*lns = newline;
}

/**
 * getnewlen - Get new length of the lns divided
 *               by ";", "||", "&&&", or "#".
 * @lns: The lns for checking.
 *
 * Return: The new length of the lns.
 *
 * Description: Shortens lines containing '#' comments with '\0'.
 */

ssize_t getnewlen(char *lns)
{
	size_t ik;
	ssize_t new_len = 0;
	char crrnt, nxt;

	for (ik = 0; lns[ik]; ik++)
	{
		crrnt = lns[ik];
		nxt = lns[ik + 1];
		if (crrnt == '#')
		{
			if (ik == 0 || lns[ik - 1] == ' ')
			{
				lns[ik] = '\0';
				break;
			}
		}
		else if (ik != 0)
		{
			if (crrnt == ';')
			{
				if (nxt == ';' && lns[ik - 1] != ' ' && lns[ik - 1] != ';')
				{
					new_len += 2;
					continue;
				}
				else if (lns[ik - 1] == ';' && nxt != ' ')
				{
					new_len += 2;
					continue;
				}
				if (lns[ik - 1] != ' ')
					new_len++;
				if (nxt != ' ')
					new_len++;
			}
			else
				logical_ops(&lns[ik], &new_len);
		}
		else if (crrnt == ';')
		{
			if (ik != 0 && lns[ik - 1] != ' ')
				new_len++;
			if (nxt != ' ' && nxt != ';')
				new_len++;
		}
		new_len++;
	}
	return (new_len);
}
/**
 * logical_ops - Check the lns for the logical operators "||" or "&&".
 * @lns: A pointer for the character for checking in the lns.
 * @new_len: Pointer to new_len in the getnewlen function.
 */
void logical_ops(char *lns, ssize_t *new_len)
{
	char prvs, crrnt, nxt;

	prvs = *(lns - 1);
	crrnt = *lns;
	nxt = *(lns + 1);

	if (crrnt == '&')
	{
		if (nxt == '&' && prvs != ' ')
			(*new_len)++;
		else if (prvs == '&' && nxt != ' ')
			(*new_len)++;
	}
	else if (crrnt == '|')
	{
		if (nxt == '|' && prvs != ' ')
			(*new_len)++;
		else if (prvs == '|' && nxt != ' ')
			(*new_len)++;
	}
}


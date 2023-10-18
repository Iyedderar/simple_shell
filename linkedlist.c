/*
 * File: linkedlist.c
 */

#include "shell.h"

alias_t *addalias_end(alias_t **hd, char *name, char *val);
void freealias_list(alias_t *hd);
list_t *add_nd_end(list_t **hd, char *dir);
void free_list(list_t *hd);

/**
 * addalias_end - Add a node in the end of the alias_t linked list.
 * @hd: A pointer for the hd of a list_t list.
 * @name: A name for the new alias to add it.
 * @val: A val for the new alias to add it.
 *
 * Return: If thier is an error - NULL.
 *         Else - a pointer for the new node.
 */
alias_t *addalias_end(alias_t **hd, char *name, char *val)
{
	alias_t *nw_nd = malloc(sizeof(alias_t));
	alias_t *last;

	if (!nw_nd)
		return (NULL);

	nw_nd->next = NULL;
	nw_nd->name = malloc(sizeof(char) * (_strlen(name) + 1));
	if (!nw_nd->name)
	{
		free(nw_nd);
		return (NULL);
	}
	nw_nd->val = val;
	_strcpy(nw_nd->name, name);

	if (*hd)
	{
		last = *hd;
		while (last->next != NULL)
			last = last->next;
		last->next = nw_nd;
	}
	else
		*hd = nw_nd;

	return (nw_nd);
}

/**
 * add_nd_end - Add a node in the end of the list_t linked list.
 * @hd: A pointer to the hd of the list_t list.
 * @dir: The directory path for the new node to contain.
 *
 * Return: If thier is an error - NULL.
 *         Else - a pointer for the new node.
 */
list_t *add_nd_end(list_t **hd, char *dir)
{
	list_t *nw_nd = malloc(sizeof(list_t));
	list_t *last;

	if (!nw_nd)
		return (NULL);

	nw_nd->dir = dir;
	nw_nd->next = NULL;

	if (*hd)
	{
		last = *hd;
		while (last->next != NULL)
			last = last->next;
		last->next = nw_nd;
	}
	else
		*hd = nw_nd;

	return (nw_nd);
}

/**
 * freealias_list - Frees a alias_t linked list.
 * @hd: THe hd of the alias_t list.
 */
void freealias_list(alias_t *hd)
{
	alias_t *next;

	while (hd)
	{
		next = hd->next;
		free(hd->name);
		free(hd->val);
		free(hd);
		hd = next;
	}
}

/**
 * free_list - Free the list_t linked list.
 * @hd: The hd of the list_t list.
 */
void free_list(list_t *hd)
{
	list_t *next;

	while (hd)
	{
		next = hd->next;
		free(hd->dir);
		free(hd);
		hd = next;
	}
}


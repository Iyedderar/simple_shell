/*
 * File: err_msgs1.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

char *error_env(char **args);
char *error_1(char **args);
char *error_2_exit(char **args);
char *error_2_syntax(char **args);
/**
 * error_env - Creates an error message for shellby_env errors.
 * @args: arguments.
 *
 * Return: The error string.
 */
char *error_env(char **args)
{
	char *error, *histr;
	int len;

	histr = _itoa(hist);
	if (!histr)
		return (NULL);

	args--;
	len = _strlen(name) + _strlen(histr) + _strlen(args[0]) + 45;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(histr);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, histr);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": Unable to add/remove from environment\n");

	free(histr);
	return (error);
}

/**
 * error_1 - Creates an error message for shellby_alias errors.
 * @args: arguments
 *
 * Return: The error string.
 */
char *error_1(char **args)
{
	char *error;
	int len;

	len = _strlen(name) + _strlen(args[0]) + 13;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		return (NULL);

	_strcpy(error, "alias: ");
	_strcat(error, args[0]);
	_strcat(error, " not found\n");

	return (error);
}

/**
 * error_2_exit - Creates an error message for shellby_exit errors.
 * @args: arguments
 *
 * Return: The error string.
 */
char *error_2_exit(char **args)
{
	char *error, *histr;
	int len;

	histr = _itoa(hist);
	if (!histr)
		return (NULL);

	len = _strlen(name) + _strlen(histr) + _strlen(args[0]) + 27;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(histr);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, histr);
	_strcat(error, ": exit: Illegal number: ");
	_strcat(error, args[0]);
	_strcat(error, "\n");

	free(histr);
	return (error);
}

/**
 * error_2_cd - Creates an error message for shellby_cd errors.
 * @args: arguments
 *
 * Return: The error string.
 */
char *error_2_cd(char **args)
{
	char *error, *histr;
	int len;

	histr = _itoa(hist);
	if (!histr)
		return (NULL);

	if (args[0][0] == '-')
		args[0][2] = '\0';
	len = _strlen(name) + _strlen(histr) + _strlen(args[0]) + 24;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(histr);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, histr);
	if (args[0][0] == '-')
		_strcat(error, ": cd: Illegal option ");
	else
		_strcat(error, ": cd: can't cd to ");
	_strcat(error, args[0]);
	_strcat(error, "\n");

	free(histr);
	return (error);
}

/**
 * error_2_syntax - Creates an error message for syntax errors.
 * @args: arguments
 *
 * Return: The error string.
 */
char *error_2_syntax(char **args)
{
	char *error, *histr;
	int len;

	histr = _itoa(hist);
	if (!histr)
		return (NULL);

	len = _strlen(name) + _strlen(histr) + _strlen(args[0]) + 33;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(histr);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, histr);
	_strcat(error, ": Syntax error: \"");
	_strcat(error, args[0]);
	_strcat(error, "\" unexpected\n");

	free(histr);
	return (error);
}

this is a repository created for ALX simple shell project.
first task : Write a beautiful code that passes the Betty checks
second task : Write a UNIX command line interpreter.
third task: Simple shell
4 task: Simple shell 0.2 +
5 task : Simple shell 0.3 +
6 task : Simple shell 0.4 + 
7 task : Write your own getline function
Use a buffer to read many chars at once and call the least possible the read system call
You will need to use static variables
You are not allowed to use getline

8 task : Simple shell 0.2 +
9 task : handle arguments for the built-in exit
Usage: exit status, where status is an integer used to exit the shell

10 task : mplement the setenv and unsetenv builtin commands

setenv
Initialize a new environment variable, or modify an existing one
Command syntax: setenv VARIABLE VALUE
Should print something on stderr on failure
unsetenv
Remove an environment variable
Command syntax: unsetenv VARIABLE
Should print something on stderr on failure

11 task : Implement the builtin command cd:

Changes the current directory of the process.
Command syntax: cd [DIRECTORY]
If no argument is given to cd the command must be interpreted like cd $HOME
You have to handle the command cd -
You have to update the environment variable PWD when you change directory

12 task : Handle the commands separator ;

13 task : Handle the && and || shell logical operators

14 task : Implement the alias builtin command
Usage: alias [name[='value'] ...]
alias: Prints a list of all aliases, one per line, in the form name='value'
alias name [name2 ...]: Prints the aliases name, name2, etc 1 per line, in the form name='value'
alias name='value' [...]: Defines an alias for each name whose value is given. If name is already an alias, replaces its value with value


15 task : Handle variables replacement
Handle the $? variable
Handle the $$ variable

16 task : Handle comments (#)

17 task : Usage: simple_shell [filename]
Your shell can take a file as a command line argument
The file contains all the commands that your shell should run before exiting
The file should contain one command per line
In this mode, the shell should not print a prompt and should not read from stdin

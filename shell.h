#ifndef SHELL_H
#define SHELL_H

#include <stdio.h> /* for printf*/
#include <unistd.h> /* for fork, execve*/
#include <stdlib.h>
#include <string.h> /* for strtok*/
#include <stddef.h>
#include <errno.h> /* for errno and perror */
#include <sys/types.h> /* for type pid */
#include <sys/wait.h> /* for wait */
#include <sys/stat.h> /* for use of stat function */
#include <signal.h> /* for signal management */
#include <fcntl.h> /* for open files*/
#include "macro.h" /* for msg help and prompt */


/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

/**
 * struct info - struct for the info
 * @program_name: the associated function to be called for program name
 *@input_line: the input line
 *@command_name: command name
 *@exec_counter: counting execution
 *@file_descriptor: file des
 *@tokens: tokes
 *@env: envirnoment
 *@alias_list: aliasl list
 */

typedef struct info
{
	char *program_name;
	char *input_line;
	char *command_name;
	int exec_counter;
	int file_descriptor;
	char **tokens;
	char **env;
	char **alias_list;
} data_of_program;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct builtins - struct for the builtins
 * @builtins: the name of the builtin
 * @function: the associated function to be called for each builtin
 */
typedef struct builtins
{
	char *builtins;
	int (*function)(data_of_program *data);
} builtins;


/* Inicialize the struct with the info of the program */
void inicialize_data(data_of_program *data, int arc, char *argv[], char **env);

/* Makes the infinite loop that shows the prompt*/
void sisifo(char *prompt, data_of_program *data);

/* Print the prompt in a new line */
void handle_ctrl_c(int opr UNUSED);

int main(int argc, char *argv[], char *env[]);

void execmd(char **argv);

int execute(data_of_program *data);

int _myexit(info_t *info);

int _mycd(info_t *info);

int _myhelp(info_t *info);

int _myhistory(info_t *info);

int unset_alias(info_t *info, char *str);

int set_alias(info_t *info, char *str);

int print_alias(list_t *node);

int _myalias(info_t *info);

void bring_line(char **lineptr, size_t *n, char *buffer, size_t j);

ssize_t get_line(char **lineptr, size_t *n, FILE *stream);

char **get_environ(info_t *);

int _unsetenv(info_t *, char *);

int _setenv(info_t *, char *, char *);

/* history file -- history.c */

char *get_history_file(info_t *info);

int write_history(info_t *info);

int read_history(info_t *info);

int build_history_list(info_t *info, char *buf, int linecount);

int renumber_history(info_t *info);

void clear_info(info_t *);

void set_info(info_t, char *);

void free_info(info_t *, int);

void _eputs(char *);

int _eputchar(char);

int _putfd(char c, int fd);

int _putsfd(char *str, int fd);

int _erratoi(char *);

void print_error(info_t *, char *);

int print_d(int, int);

char *convert_number(long int, int, int);

void remove_comments(char *);

char *_strncpy(char *, char *, int);

char *_strncat(char *, char *, int);

char *_strchr(char *, char);

int bfree(void **);

char *_memset(char *, char, unsigned int);

void ffree(char **);

void *_realloc(void *, unsigned int, unsigned int);

int is_cmd(info_t *, char *);

char *dup_chars(char *, int, int);

char *find_path(info_t *, char *, char *);

char **strtow(char *, char *);

char **strtow2(char *, char);

int is_chain(info_t *, char *, size_t *);

void check_chain(info_t *, char *, size_t *, size_t, size_t);

int replace_alias(info_t *);

int replace_vars(info_t *);

int replace_string(char **, char *);

#endif

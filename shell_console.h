#ifndef _SHELL_CONSOLE_H_

#include "shell_uart.h"

#define buffer_length (256)
#define arg_length (100)

typedef struct
{
	char buffer[buffer_length];
	unsigned int cnt;
	char argv[][arg_length];
}shell_console_s;

typedef struct
{
	char *str; /* cmd */
	unsigned char arg_num; /* value num include  */
	void (*cmd_function)(char argc, char argv[][arg_length]);
	char *help_str; /* cmd help notice */
}cmd_list;

void shell_init(void);
void shell_put(unsigned char ch);
void shell_puts(char *str);
void shell_gets(void);
void shell_debug(char argc, char argv[][arg_length]);
void shell_main(void);

#define for_each_cmd(i, max_cmd) \
	for(i=0; i<max_cmd; i++)

#define get_cmd_num() (sizeof(SHELL_CMD)/sizeof(cmd_list))

#define shell_printf printf

#endif


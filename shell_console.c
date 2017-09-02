#include "shell_console.h"
#include <string.h>
#include <stdlib.h>

shell_console_s shell_console;

#define console_title "\r\nshell_v1.0# "
#define shell_put_title() shell_puts(console_title)

static int shell_strcmp(char *str1, cmd_list *shell_cmd, char argv[][arg_length], unsigned char length);

void test_debug(char argc, char argv[][arg_length])
{
	shell_printf("this is test , num is %d argv is %s\r\n", argc, argv[1]);
}

void check_memory(char argc, char argv[][arg_length])
{
	unsigned long int addr;
	unsigned char length;

	addr = strtoul(argv[1], NULL, 16);
	length = strtoul(argv[2], NULL, 10);
	shell_printf("this is %s , addr[%x] length[%d]\r\n", argv[0], addr, length);
}

/* regist your cmd here */
cmd_list SHELL_CMD[] = 
{
	{
		"shell_info",
		1,
		shell_debug,
		"look up for the shell information"
	},

	{
		"shell",
		1,
		shell_debug,
		"shell"
	},
	
	{
		"test",
		2,
		test_debug,
		"test debug"
	},

	{
		"md",
		3,
		check_memory,
		"check memory"
	}
};

void shell_init(void)
{
	console->init();
	memset(&shell_console, 0, sizeof(shell_console_s));
	shell_puts("\r\n=========welcome to console v1.0========");
	shell_put_title();
}

void shell_put(unsigned char ch)
{
	console->putchar(ch);
}

void shell_puts(char *str)
{
	console->putstring(str);
}

void shell_gets(void)
{
	unsigned char ch;
	ch = console->getchar();
	shell_console.buffer[shell_console.cnt++] = ch;
	//printf("strong one receive:%d \r\n", ch);
}

static void shell_check(void)
{
	static unsigned int shell_check_cnt = 0;
	char ch;
	unsigned char cmd_num, cmd_sum;
	int cmp_cnt;
	unsigned char mismatch_cnt = 0;

	if(shell_check_cnt != shell_console.cnt)
	{
		ch = shell_console.buffer[shell_check_cnt++];
		/* shell display the number and alphabet */
		if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') 
			|| (ch >= '0' && ch<='9') || (ch == 95) || (ch == 32))
		{
			shell_put(ch);
		}
		/* detect the enter button */
		else if(ch == 13)
		{
			//shell_puts("\r\nshell_v1.0#");
			if(shell_console.cnt == 1)
			{
				shell_put_title();
			}
			else
			{
				cmd_sum = get_cmd_num();
				for_each_cmd(cmd_num, cmd_sum) {
					cmp_cnt = shell_strcmp(shell_console.buffer, &SHELL_CMD[cmd_num], shell_console.argv, shell_console.cnt-1);
					if(strlen(SHELL_CMD[cmd_num].str) == cmp_cnt || cmp_cnt == 0)
					{
						/* arguments matched */
						shell_puts("\r\n");
						SHELL_CMD[cmd_num].cmd_function(SHELL_CMD[cmd_num].arg_num, shell_console.argv);
					}
					else
					{
						/* mismatch */
						mismatch_cnt++;
					}
				}
				
				if(mismatch_cnt == cmd_sum)
				{
					shell_puts("\r\ncan not find command");
					shell_put_title();
					mismatch_cnt = 0;
				}
			}
			
			memset(&shell_console, 0, sizeof(shell_console_s));
			shell_check_cnt = 0;
		}
		/* tab */
		else if(ch == 9)
		{
			cmd_sum = get_cmd_num();
			for_each_cmd(cmd_num, cmd_sum) {
				cmp_cnt = shell_strcmp(shell_console.buffer, &SHELL_CMD[cmd_num], shell_console.argv, shell_console.cnt-1);
				if(strlen(SHELL_CMD[cmd_num].str) == cmp_cnt)
				{
					/* match */
					memset(&shell_console, 0, sizeof(shell_console_s));
					shell_check_cnt = cmp_cnt;
					shell_console.cnt = strlen(SHELL_CMD[cmd_num].str);
					strcpy(shell_console.buffer, SHELL_CMD[cmd_num].str); //back to where it should be
					
				}
				else
				{
					/* mismatch */
					if(cmp_cnt > 0)
					{
						//matched sort of words from the head of the cmd
						memset(&shell_console, 0, sizeof(shell_console_s));
						shell_check_cnt = cmp_cnt;
						shell_console.cnt = strlen(SHELL_CMD[cmd_num].str);
						strcpy(shell_console.buffer, SHELL_CMD[cmd_num].str);
					}
				}
			}
		}
	}
}

void shell_main(void)
{
	shell_check();
}

void shell_debug(char argc, char argv[][arg_length])
{
	shell_puts("this is shell debug version v1.0, made by westzhao\r\n");
}

static int shell_strcmp(char *str1, cmd_list *shell_cmd, char argv[][arg_length], unsigned char length)
{
	unsigned char cmp_cnt = 0, cmd_arg, arg_cnt = 1;
	char *cmd_str;
	char pCnt;
	
	cmd_arg = shell_cmd->arg_num;
	cmd_str = shell_cmd->str;
	while(length)
	{
		if((*str1-*cmd_str++)!=0 || !*str1++)
		{
			goto mismatch;
		}
		length--;
		cmp_cnt++;
	}
	
	return cmp_cnt; /* one argument, if matched it returns length of the arg */
	
mismatch:
	if((cmp_cnt == strlen(shell_cmd->str)) && (cmd_arg != 1) && (*(str1) == ' '))
	{
		strcpy(argv[0], shell_cmd->str);
		str1++;
		while(arg_cnt < cmd_arg)
		{
			pCnt = 0;
			while((*str1 != ' ') && (*str1 != 0) && (*str1 != 13))
			{
				argv[arg_cnt][pCnt++] = *str1++;
			}
		    arg_cnt++;
			if(*str1 == ' ')
				str1++;
		}
		/* few arguments, if check done it returns 0, and arguments have put into the point argv*/
		return 0; 
	}
	return -1; /* mismatch any command */
}

/**
 * @brief  UART0 中断服务函数
 * we overwrite the uart interrupt function(strong one)
 * @param  None
 * @retval None
 */
void UART0_IRQHandler(void)
{
	shell_gets();
}


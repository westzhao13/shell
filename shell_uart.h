#ifndef _SHELL_UART_H_

/* bsp interface: nxp bsp for kl02's uart modle
   if you want to transplant to another platform
   change the bsp interface file here
*/
#include "uart.h"

typedef struct
{
	void (*init)(void);
	void (*putchar)(char ch);
	unsigned char (*getchar)(void);
	void (*putstring)(char *str);
}console_function_s;

extern console_function_s *console;




#endif

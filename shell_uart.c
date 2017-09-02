#include "shell_uart.h"


/* KL02 bsp interface */
static void uart_console_init(void)
{
	/* UART initialize */
    UART_QuickInit(UART0_RX_B2_TX_B1, 9600);
    UART_ITConfig(HW_UART0, kUART_IT_Rx, true);
}

static void uart_console_putchar(char ch)
{
	UART_WriteByte(HW_UART0, ch);
}

/* usually we get one byte in the uart interrupt */
static unsigned char uart_console_getchar(void)
{
	unsigned char ch;
	UART_ReadByte(HW_UART0, &ch);
	return ch;
}

static void uart_console_putstring(char *str)
{
	while(*str != '\0')
	{
    	UART_WriteByte(HW_UART0, *str++);
	}
}

console_function_s uart_console = 
{
	.init = uart_console_init,
	.putchar = uart_console_putchar,
	.getchar = uart_console_getchar,
	.putstring = uart_console_putstring
};
console_function_s *console = &uart_console;





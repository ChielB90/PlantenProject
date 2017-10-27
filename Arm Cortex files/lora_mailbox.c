//////////////////////////////////////////////////////////////////////////////
// prog: loraread_mailbox.c
//       shows mailbox handling and along the way handles LORA_read
// note: for OSMboxCreate, see main.c!
// auth: CB
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

void LoraRead_PostTask(void *pdata)
{
	char buffer[LORA_SIZE];
	INT8U error;

	while(TRUE)
	{
		OSTimeDly(LOOP_DELAY);
		LORA_gets(buffer);
		UART_puts("\r\npbuffer: "); UART_puts(buffer); UART_puts(MSGEND);
		OSMboxPost(LORA_ReadHandle, buffer);

		OSFlagPend(FlagReadHandle, READY, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, WAIT_FOREVER, &error);
	}
}

void LoraRead_PendTask(void *pdata)
{
	INT8U error;
	char *command;

	while(TRUE)
	{
		command = OSMboxPend(LORA_ReadHandle, WAIT_FOREVER, &error);
		UART_puts(MSGEND); UART_puts("uC/OS-II Task recieved: "); UART_puts(command); UART_puts(MSGEND);

		OSFlagPost(FlagWriteHandle, READY, OS_FLAG_SET, &error);
	}
}

//////////////////////////////////////////////////////////////////////////////
// prog: lorawrite_mailbox.c
//       shows mailbox handling and along the way handles LORA_write
// note: for OSMboxCreate, see main.c!
// auth: CB
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

void LoraWrite_PostTask(void *pdata) {
	INT8U error;
	char msg[LORA_SIZE];
	char buffer[LORA_SIZE];

	while (TRUE) {
		OSTimeDly(LOOP_DELAY);
		UART_gets(buffer, FALSE);
		LORA_combine(LORA_TX, buffer, msg);
		UART_puts("\r\nprint LORA_combine: ");
		UART_puts(msg);
		OSMboxPost(LORA_WriteHandle, msg);

		OSFlagPend(FlagWriteHandle, READY,
				OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, WAIT_FOREVER, &error);
	}
}

void LoraWrite_PendTask(void *pdata) {
	INT8U error;
	char *command;
	char response[LORA_SIZE];

	while (TRUE) {
		command = OSMboxPend(LORA_WriteHandle, WAIT_FOREVER, &error);
		UART_puts(MSGEND);
		UART_puts("uC/OS-II Write to Lora: ");
		UART_puts(command);
		UART_puts(MSGEND);
		LORA_puts(command);
		LORA_gets(response);
		if (strstr(response, LOK) != NULL)
		{
			UART_puts(MSGEND);
			UART_puts("LORA responce: ");
			UART_puts(response);

			OSFlagPost(FlagReadHandle, READY, OS_FLAG_SET, &error);
		}
	}
}

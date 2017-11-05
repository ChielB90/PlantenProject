//////////////////////////////////////////////////////////////////////////////
// prog: loraread_mailbox.c
//       shows mailbox handling and along the way handles LORA_read
// note: for OSMboxCreate, see main.c!
//		 see for defines lora.h in ports/header/
// auth: Chileam Bohnen
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
		//Receive data from LoRA-module
		LORA_gets(buffer);
		UART_puts("\r\nbuffer: "); UART_puts(buffer); UART_puts(MSGEND);

		//MailBox post on LORA_ReadHandle
		OSMboxPost(LORA_ReadHandle, buffer);
		//Wait for FlagReadHandle is READY by LoraWrite_PendTask
		OSFlagPend(FlagReadHandle, READY,
				OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, WAIT_FOREVER, &error);
	}
}

void LoraRead_PendTask(void *pdata)
{
	INT8U error;
	char *command;

	while(TRUE)
	{
		//Receive MailBox from LoraRead_PostTask
		command = OSMboxPend(LORA_ReadHandle,
				WAIT_FOREVER, &error);
		UART_puts(MSGEND); UART_puts("uC/OS-II Task received: ");
		UART_puts(command); UART_puts(MSGEND);
		//Set flag FlagWriteHandle READY for LoraWrite_PostTask
		OSFlagPost(FlagWriteHandle, READY, OS_FLAG_SET, &error);
	}
}

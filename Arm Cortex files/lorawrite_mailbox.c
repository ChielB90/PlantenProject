//////////////////////////////////////////////////////////////////////////////
// prog: lorawrite_mailbox.c
//       shows mailbox handling and along the way handles LORA_write
// note: for OSMboxCreate, see main.c!
// auth: CB
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

#define LORA_DATASIZE 50
#define DATASIZE 10

void LoraWrite_PostTask(void *pdata) {
	INT8U error;
	char msg[LORA_DATASIZE];
	char buffer[LORA_DATASIZE];
	char msglen = 0;
	OS_Q_DATA qd;

	while (TRUE) {
		OSTimeDly(LOOP_DELAY);
		OSQQuery(LoraQHandle,&qd);
		if(qd.OSNMsgs > 0)
		{
			while(msglen <= (LORA_DATASIZE - DATASIZE) && msglen < (DATASIZE*qd.OSNMsgs))
			{
				strcpy((buffer+msglen),(char*)OSQPend(LoraQHandle, WAIT_FOREVER,&error));
				msglen += DATASIZE;
			}
			msglen = 0;

		LORA_combine(LORA_TX, buffer, msg);
		UART_puts("\r\nprint LORA_combine: ");
		UART_puts(msg);
		OSMboxPost(LORA_WriteHandle, msg);

		OSFlagPend(FlagWriteHandle, READY,
				OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, WAIT_FOREVER, &error);
		}
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
			UART_puts("LORA response: ");
			UART_puts(response);

			OSFlagPost(FlagReadHandle, READY, OS_FLAG_SET, &error);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// prog: lorawrite_mailbox.c
//       shows mailbox handling and along the way handles LORA_write
// note: for OSMboxCreate, see main.c!
//		 see for defines lora.h in ports/header/
// auth: Chileam Bohnen
//////////////////////////////////////////////////////////////////////////////

#include <includes.h>
#include "main.h"

#define LORA_DATASIZE 50

void LoraWrite_PostTask(void *pdata) {
	INT8U error;
	char msg[LORA_DATASIZE];
	char buffer[LORA_DATASIZE];
	char msglen = 0;
	OS_Q_DATA qd;

	while (TRUE) {
		OSTimeDly(LOOP_DELAY);
		//Build Queue data from LoraQHandle
		OSQQuery(LoraQHandle,&qd);
		if (qd.OSNMsgs > 0)
		{
			while (msglen <= (LORA_DATASIZE - DATASIZE) &&
					msglen < (DATASIZE*qd.OSNMsgs))
			{
				//Collect Queue data from LoraQHandle
				strcpy((buffer+msglen),(char*)OSQAccept(LoraQHandle, &error));
				msglen += DATASIZE;
			}
			msglen = 0;

		//Combine RN2483 command 'mac tx uncnf 1 ' with buffered data
		LORA_combine(LORA_TX, buffer, msg);

		UART_puts("\r\nprint LORA_combine: ");
		UART_puts(msg);

		//MailBox post on LORA_WriteHandle
		OSMboxPost(LORA_WriteHandle, msg);

		//Wait for FlagWriteHandle is READY by LoraRead_PendTask
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

		//Receive MailBox from LoraWrite_PostTask
		command = OSMboxPend(LORA_WriteHandle, WAIT_FOREVER, &error);
		UART_puts(MSGEND); UART_puts("uC/OS-II Write to Lora: ");
		UART_puts(command);UART_puts(MSGEND);

		LORA_puts(command);
		LORA_gets(response);
		//RN2483 confirm a correct received command with an OK. LOK
		if (strstr(response, LOK) != NULL)
		{
			UART_puts(MSGEND);
			UART_puts("LORA response: ");
			UART_puts(response);

			//Set flag FlagReadHandle READY for LoraWrite_PostTask
			OSFlagPost(FlagReadHandle, READY, OS_FLAG_SET, &error);
		}
	}
}

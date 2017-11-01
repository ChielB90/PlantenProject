//////////////////////////////////////////////////////////////////////////////
// prog: lora.c
//       uart3/LORA driver for ARM-board v5
// note: directed from ports/source/uart.c!
// auth: CB
//////////////////////////////////////////////////////////////////////////////

#include "includes.h"

void LORA_init()
{
	// TODO
	//pin initalisatie van UART3

	//	---	System Clocks Configuration ---
	//USART3 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//GPIOB clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	//	---	GPIO Configuration	---
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Connect USART pins to AF
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	USART_InitTypeDef LORA_InitStruct;

	//UART interface RN2483
	LORA_InitStruct.USART_BaudRate = 57600;
	LORA_InitStruct.USART_WordLength = USART_WordLength_8b;
	LORA_InitStruct.USART_StopBits = USART_StopBits_1;
	LORA_InitStruct.USART_Parity = USART_Parity_No;
	LORA_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	LORA_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &LORA_InitStruct);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
}

void LORA_putchar(char c)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, c);
}

void LORA_puts(char *buffer)
{
	volatile unsigned int i;
	for (i = 0; buffer[i]; i++)
	{
		LORA_putchar(buffer[i]);
	}
}

// Ontvang een karakter via de UART3
// niet echt nodig als routine maar als wrapper voor compatabiliteit. Let op geen -1 als er geen char is ontvangen!
char LORA_get(void)
{
    char uart_char = -1;
    if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE)== SET)  // check for data available
    	 uart_char= USART3->DR & 0xFF; // and read the data from peripheral
    return uart_char;
}

// void LORA_gets
// args: char *readbuffer
// remark: ARM sends -1 if buffer is empty
//         LF is cleared if set in terminal-program
void LORA_gets(char *s)
{
	while (TRUE)
	{
	 	*s = LORA_get();
	 	if (*s==-1)             // check for data available
	 		continue;

	 	if (*s==0xff || *s==LF) // if no data or LF, continue
			continue;

		if (*s==CR)            // if enter pressed
		{
			*s = '\0';         // ignore char and close string
		    return;            // buf ready, exit loop
		}
		s++;
	}
}

//designed by tijmen
void LORA_combine(INT8U command, char *parameter, char *target)
{
	char errorCode[10];

	switch(command & 0xF0)
	{
	//sys
	case 0xF0: //get hweui
		strcpy(target, LSYS);
		strcat(target, SPACE);
		strcat(target, LGET);
		strcat(target, SPACE);
		strcat(target, LHWEUI);
		break;

		//radio
	case 0xE0: //rx
		strcpy(target, LRADIO);
		strcat(target, SPACE);
		strcat(target, LRX);
		strcat(target, SPACE);
		strcat(target, parameter);
		break;

		//mac
	case 0x00: //standard
		strcpy(target, LMAC);
		strcat(target, SPACE);

		//standard
		switch(command & 0x0F)
		{
		case 0x01: //join
			strcat(target, LJOIN);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		case 0x02: //save
			strcat(target, LSAVE);
			break;

		case 0x03: //tx
			strcat(target, LTX);
			strcat(target, SPACE);
			strcat(target, LUNCNF);
			strcat(target, SPACE);
			strcat(target, LORA_PORT);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		case 0x04: //pause
			strcat(target, LPAUSE);
			break;

		case 0x05: //resume
			strcat(target, LRESUME);
			break;

		default:
			break;
		}
		break;

	case 0x10: //get
		strcpy(target, LMAC);
		strcat(target, SPACE);
		strcat(target, LGET);
		strcat(target, SPACE);

		//get
		switch(command & 0x1F)
		{

		//deveui
		case 0x10:
			strcat(target, LDEVEUI);
			break;

			//appeui
		case 0x11:
			strcat(target, LAPPEUI);
			break;

			//status
		case 0x12:
			strcat(target, LSTATUS);
			break;

			//ch
		case 0x13:
			strcat(target, LCH);
			break;

		case 0x14:
			strcat(target, LDEVADDR);
			break;

		default: break;
		}
		break;

	case 0x20: //set
		strcpy(target, LMAC);
		strcat(target, SPACE);
		strcat(target, LSET);
		strcat(target, SPACE);

		//set
		switch(command & 0x2F)
		{

		//deveui
		case 0x20:
			strcat(target, LDEVEUI);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		//appeui
		case 0x21:
			strcat(target, LAPPEUI);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		//appkey
		case 0x22:
			strcat(target, LAPPKEY);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		//ch
		case 0x23:
			strcat(target, LCH);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		//devaddr
		case 0x24:
			strcat(target, LDEVADDR);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		//appskey
		case 0x25:
			strcat(target, LAPPSKEY);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		//nwkskey
		case 0x26:
			strcat(target, LNWKSKEY);
			strcat(target, SPACE);
			strcat(target, parameter);
			break;

		default: break;
		}
		break;

	//command error
	default:

		itoa(command, errorCode, 10);
		strcat(target, "\r\ndefault error got: ");
		strcat(target, errorCode);
		strcat(target, MSGEND);
		UART_puts(target);
		return;

	}
	strcat(target, MSGEND);
}

void LORA_register(char *mode)
{
	char msg[LORA_SIZE];
	char buffer[LORA_SIZE];
	char dev_eui[LORA_SIZE];
	char save[LORA_SIZE];
	memset(save, 0, LORA_SIZE);

	if ((strstr(mode, LABP)) != NULL)
	{
		LORA_combine(LORA_GETDEVADDR, NULL, msg);
		UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

		LORA_puts(msg);
		LORA_gets(buffer);
		UART_puts("\r\nget dev_addr got: "); UART_puts(buffer);

		LORA_combine(LORA_SETDEVADDR, LORA_NETWORK, msg);
		UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

		LORA_puts(msg);
		LORA_gets(buffer);
		UART_puts("\r\ndev_addr got: "); UART_puts(buffer);

		while(strstr(save, LOK) == NULL)
		{

			LORA_combine(LORA_SETAPPSKEY, LORA_APPSKEY, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(buffer);
			UART_puts("set appskey got: "); UART_puts(buffer);

			LORA_combine(LORA_SETNWKSKEY, LORA_NWKSKEY, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(buffer);
			UART_puts("set nwkskey got: "); UART_puts(buffer);

			LORA_combine(LORA_SAVE, NULL, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(save);
			UART_puts("save got: "); UART_puts(save);
		}
	}

	if ((strstr(mode, LOTAA)) != NULL)
	{
		//SYS GET HWEUI
		LORA_combine(LORA_GETHWEUI, NULL, msg);
		UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

		LORA_puts(msg);
		LORA_gets(dev_eui);
		UART_puts("dev_eui got: "); UART_puts(dev_eui);

		UART_puts("\r\nstrstr(save, LOK) = "); UART_putint(*strstr(save, LOK)); UART_puts("\r\n");
		while(strstr(save, LOK) == NULL)
		{

			LORA_combine(LORA_SETDEVEUI, dev_eui, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(buffer);
			UART_puts("set deveui got: "); UART_puts(buffer);

			LORA_combine(LORA_SETAPPEUI, LORA_APPEUI, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(buffer);
			UART_puts("set appeui got: "); UART_puts(buffer);

			LORA_combine(LORA_SETAPPKEY, LORA_APPKEY, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(buffer);
			UART_puts("set appkey got: "); UART_puts(buffer);

			LORA_combine(LORA_SAVE, NULL, msg);
			UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

			LORA_puts(msg);
			LORA_gets(save);
			UART_puts("save got: "); UART_puts(save);
		}
	}
}

INT8U LORA_connect(void)
{
	//INT8U error = 1;
	char msg[LORA_SIZE];
	char buffer[LORA_SIZE];

	//LORA join
	LORA_combine(LORA_JOIN, LABP, msg);
	UART_puts("\r\nprint LORA_combine: "); UART_puts(msg);

	LORA_puts(msg);
	LORA_gets(buffer);
	UART_puts("\r\njoin got: "); UART_puts(buffer);

	if (strstr(buffer, LOK) == NULL)
	{
		//LORA module is not registered
		if(strstr(buffer, LKEYSNOTINIT))
			LORA_register(LABP);

		UART_puts("\r\ngot error: "); UART_puts(buffer);
		return FALSE;
	}

	LORA_gets(buffer);
	UART_puts("got: "); UART_puts(buffer);
	if (strstr(buffer, LACCEPTED) == NULL)
		return FALSE;

	return TRUE;
}

#define INT8U    unsigned char

#define LSYS    "sys"
#define LMAC    "mac"

#define LGET    "get" 
#define LSET    "set"
#define LJOIN   "join"
#define LSAVE   "save"
#define LTX     "tx"

#define LHWEUI  "hweui"
#define LDEVEUI "deveui"
#define LAPPKEY "appkey"
#define LAPPEUI "appeui"
#define LSTATUS "status"
#define LCH     "ch"

#define LSPACE	" "
#define LEND    "\r\n"


#include <string.h> //strcpy
#include <stdio.h>
#include <stdlib.h>


void LORA_combine(INT8U command, char *parameter, char *target)
{
	
	switch(command & 0x01)
	{
		//sys
		case 0:
		strcpy(target, LSYS);
		strcat(target, LSPACE);
		strcat(target, LGET);
		strcat(target, LSPACE);
		strcat(target, LHWEUI);
		strcat(target, LSPACE);
		strcat(target, LEND);
		break;
		
		//mac
		case 1:
		strcpy(target, LMAC);
		strcat(target, LSPACE);
		
		switch(command & 0x0E)
		{
			//join
			case 0:
			strcat(target, LJOIN);
			strcat(target, LSPACE);
			strcat(target, parameter);
			strcat(target, LSPACE);
			strcat(target, LEND);
			break;
			
			//save
			case 2:
			strcat(target, LSAVE);
			strcat(target, LSPACE);
			strcat(target, parameter);
			strcat(target, LSPACE);
			strcat(target, LEND);
			break;
			
			//tx
			case 4:
			strcat(target, LTX);
			strcat(target, LSPACE);
			strcat(target, parameter);	
			strcat(target, LSPACE);
			strcat(target, LEND);
			break;
			
			//get
			case 6:
			strcat(target, LGET);
			strcat(target, LSPACE);
			
			switch(command&0xF0)
			{
				//deveui
				case 0:
				strcat(target, LDEVEUI);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
				
				//appeui
				case 16:
				strcat(target, LAPPEUI);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
				
				//status
				case 32:
				strcat(target, LSTATUS);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
				
				//ch
				case 48:
				strcat(target, LCH);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
			}
			break;
			
			//set
			case 8:
			strcat(target, LSET);
			strcat(target, LSPACE);
			
			switch(command&0xF0)
			{	//deveui
				case 0:
				strcat(target, LDEVEUI);
				strcat(target, LSPACE);
				strcat(target, parameter);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
				
				//appeui
				case 16:
				strcat(target, LAPPEUI);
				strcat(target, LSPACE);
				strcat(target, parameter);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
				
				//appkey
				case 32:
				strcat(target, LAPPKEY);
				strcat(target, LSPACE);
				strcat(target, parameter);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;
				
				//ch
				case 48:
				strcat(target, LCH);
			    strcat(target, LSPACE);
				strcat(target, parameter);
				strcat(target, LSPACE);
				strcat(target, LEND);
				break;			
			}
			break;
		}
		break;
	}
}


void main()
{
	while(1)
	{
	char s[80];
	char s2[80];
	char s3[80];
	char c;
	
	gets(s);
	gets(s2);
	
	c = atoi(s);
	
	if(c==99)
		return;
	
	LORA_combine(c,s2,s3);
	
	printf(s3);
	}
}

#include <includes.h>
#include "main.h"
#include <math.h>

#define BEGINADDRESS 20
#define SENSORAMOUNT 1 //3 to test with 3 dummy sensors, endversion has 1

typedef union Vochtigheid
{
   float floatvalue;
   uint32_t ulongvalue;
}UVOCHTIGHEID;

typedef struct data
{
  char address;
  UVOCHTIGHEID vochtigheid;
  char CayennePercentage;
  char datamsg[DATASIZE+1];		//+1 for '/0' character
}DATA,*PDATA;

DATA Data[SENSORAMOUNT];

void floattohexstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x%08x",(unsigned int)pdata->address,(unsigned int)pdata->vochtigheid.ulongvalue);
}

void CpercentagetoChayenneLPPstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x68%02x",(unsigned int)pdata->address, pdata->CayennePercentage);
}

void floattoCpercentage(PDATA pdata)
{
	/*insert eiklijn etc												     *
	 * example:																 *
	 * if(pdata->vochtigheid.floatvalue<x ||pdata->vochtigheid.floatvalue>y) *
	 * 		pdata->CayennePercentage = 20*2;								 */
	if(pdata->vochtigheid.floatvalue>7800) pdata->CayennePercentage = 0 *2;
	if(pdata->vochtigheid.floatvalue<7800 && pdata->vochtigheid.floatvalue<=7600) pdata->CayennePercentage = 10 *2;
	if(pdata->vochtigheid.floatvalue<7600 && pdata->vochtigheid.floatvalue<=7300) pdata->CayennePercentage = 20 *2;
	if(pdata->vochtigheid.floatvalue<7300 && pdata->vochtigheid.floatvalue<=7100) pdata->CayennePercentage = 30 *2;
	if(pdata->vochtigheid.floatvalue<7100 && pdata->vochtigheid.floatvalue<=6800) pdata->CayennePercentage = 40 *2;
	if(pdata->vochtigheid.floatvalue<6800 && pdata->vochtigheid.floatvalue<=6600) pdata->CayennePercentage = 50 *2;
	if(pdata->vochtigheid.floatvalue<6600 && pdata->vochtigheid.floatvalue<=6400) pdata->CayennePercentage = 60 *2;
	if(pdata->vochtigheid.floatvalue<6400 && pdata->vochtigheid.floatvalue<=6200) pdata->CayennePercentage = 70 *2;
	if(pdata->vochtigheid.floatvalue<6200 && pdata->vochtigheid.floatvalue<=6000) pdata->CayennePercentage = 80 *2;
	if(pdata->vochtigheid.floatvalue<6000 && pdata->vochtigheid.floatvalue<=5900) pdata->CayennePercentage = 90 *2;
	if(pdata->vochtigheid.floatvalue<5900) pdata->CayennePercentage = 100 *2;

}


void SensorRead_Task(void *pdata)
{
	for(int i = 0; i<SENSORAMOUNT; i++)
		Data[i].address= i+BEGINADDRESS;

	PDATA pData = &Data[0];
	while(1)
	{
	  	OSTimeDly(12000); // 12000 ticks == 1 minute -> 200 ticks/s
	  	UART_puts("getting data...");
	  	UART_puts(MSGEND);
	  	for(int i = 0; i<SENSORAMOUNT; i++)
	  	{
	  		(pData+i)->vochtigheid.floatvalue =getData((pData+i)->address);	//to test without sensor command this line
			#ifndef CAYENNE
	  		floattohexstring((pData+i));							//
			#endif
			#ifdef CAYENNE
	  		floattoCpercentage((pData+i)); 			//for Cayenne
	  		CpercentagetoChayenneLPPstring((pData+i));	//for Cayenne
			#endif
	  		OSQPost(LoraQHandle, (pData+i)->datamsg);
		}

		for(int i = 0; i < 9; i++ )
			OSTimeDly(12000);	//14 minute delay
	}
}

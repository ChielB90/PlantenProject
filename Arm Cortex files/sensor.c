#include <includes.h>
#include "main.h"

#define SIZE 11
#define BEGINADDRESS 20
#define SENSORAMOUNT 1

typedef union Vochtigheid
{
   float floatvalue;
   uint32_t ulongvalue;
}UVOCHTIGHEID;

typedef struct data
{
  char address;
  UVOCHTIGHEID vochtigheid;
  char datamsg[SIZE];
}DATA,*PDATA;

DATA Data[SENSORAMOUNT];

void floattohexstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x%08x",(unsigned int)pdata->address,(unsigned int)pdata->vochtigheid.ulongvalue);
}

void SensorRead_Task(void *pdata)
{
	for(int i = 0; i<SENSORAMOUNT; i++)
		Data[i].address= i+BEGINADDRESS;

	PDATA pData = &Data[0];
	while(1)
	{
	  	OSTimeDly(12000); // 12000 ticks == 1 minute -> 200 ticks/s
		for(int i = 0; i<SENSORAMOUNT; i++)
	  	{
	  		(pData+i)->vochtigheid.floatvalue =getData((pData+i)->address);
			floattohexstring((pData+i));
			OSQPost(LoraQHandle, (pData+i)->datamsg);
		}
		for(int i = 0; i < 14; i++ )
			OSTimeDly(12000);	//14 minute delay
	}
}

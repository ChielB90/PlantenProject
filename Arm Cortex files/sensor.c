#include <includes.h>
#include "main.h"
#include <math.h>

#define BEGINADDRESS 20 //here you can set the beginaddress of the sensors
#define SENSORAMOUNT 1 //here you can set the amount of sensors needed statically


/* Union to switch between float and ulong values
 * (c) Tijmen van der Leest
 */
typedef union Humidity
{
   float floatvalue;
   uint32_t ulongvalue;
}UHUMIDITY;


/* DATA struct to store all necessary data about the sensors
 * (c) Tijmen van der Leest
 */
typedef struct data
{
  char address;
  UHUMIDITY humidity;
  char CayennePercentage;
  char datamsg[DATASIZE+1];		//+1 for '/0' character
}DATA,*PDATA;

DATA Data[SENSORAMOUNT];

/* void floattohexstring
 * args: PDATA pdata
 * remarks:
 * makes from a float and address a HEXString.
 * needs the PDATA type to work and needs values for address and humidity (see blueprint above)
 * (c) Tijmen van der Leest
 */
void floattohexstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x%08x",(unsigned int)pdata->address,(unsigned int)pdata->humidity.ulongvalue);
}

/* void floattoCpercentage
 * args: PDATA pdata
 * remarks:
 * makes from a float a Cayenne Percentage.
 * needs PDATA to work (see blueprint above)
 * A Cayenne Percentage is a char with double the needed percentage.
 * (c) Tijmen van der Leest
 */
void floattoCpercentage(PDATA pdata)
{
	if(pdata->humidity.floatvalue>7800) pdata->CayennePercentage = 0 *2;
	if(pdata->humidity.floatvalue<7800 && pdata->humidity.floatvalue<=7600) pdata->CayennePercentage = 10 *2;
	if(pdata->humidity.floatvalue<7600 && pdata->humidity.floatvalue<=7300) pdata->CayennePercentage = 20 *2;
	if(pdata->humidity.floatvalue<7300 && pdata->humidity.floatvalue<=7100) pdata->CayennePercentage = 30 *2;
	if(pdata->humidity.floatvalue<7100 && pdata->humidity.floatvalue<=6800) pdata->CayennePercentage = 40 *2;
	if(pdata->humidity.floatvalue<6800 && pdata->humidity.floatvalue<=6600) pdata->CayennePercentage = 50 *2;
	if(pdata->humidity.floatvalue<6600 && pdata->humidity.floatvalue<=6400) pdata->CayennePercentage = 60 *2;
	if(pdata->humidity.floatvalue<6400 && pdata->humidity.floatvalue<=6200) pdata->CayennePercentage = 70 *2;
	if(pdata->humidity.floatvalue<6200 && pdata->humidity.floatvalue<=6000) pdata->CayennePercentage = 80 *2;
	if(pdata->humidity.floatvalue<6000 && pdata->humidity.floatvalue<=5900) pdata->CayennePercentage = 90 *2;
	if(pdata->humidity.floatvalue<5900) pdata->CayennePercentage = 100 *2;

}

/* void CpercentagetoChayenneLPPstring
 * args: PDATA pdata
 * remarks:
 * makes from a the address and CayennePercentage a Cayenne LPP string.
 * needs PDATA to work (see blueprint above)
 * (c) Tijmen van der Leest
 */
void CpercentagetoChayenneLPPstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x68%02x",(unsigned int)pdata->address, pdata->CayennePercentage);
}

/* void SensorRead_Task
 * args: void *pdata
 * remarks:
 * data gets acquired through getData(char address) function.
 * This data will be set into a HEXString (the type is dependent on the define CAYENNE)
 * and the string will be posted to a queue.
 * there is one minute wait at the beginning so the sensors can stabilise after the system is turned on
 * (c) Tijmen van der Leest
 */
void SensorRead_Task(void *pdata)
{
	for(int i = 0; i<SENSORAMOUNT; i++)	//setting address from all sensors
		Data[i].address= i+BEGINADDRESS;

	PDATA pData = &Data[0];
	while(1)
	{
	  	OSTimeDly(12000); //  200 ticks/s -> 12000 ticks == 1 minute
	  	UART_puts("getting data...");
	  	UART_puts(MSGEND);
	  	for(int i = 0; i<SENSORAMOUNT; i++)
	  	{
	  		(pData+i)->humidity.floatvalue =getData((pData+i)->address);	//to test without sensor command this line
			#ifndef CAYENNE
	  		floattohexstring((pData+i));									//sets address and float to a HEXString
			#endif
			#ifdef CAYENNE
	  		floattoCpercentage((pData+i)); 									//sets Hz to CayennePercentage
	  		CpercentagetoChayenneLPPstring((pData+i));						//sets address + sensor ID + CayennePercentage into a CLPP HEXString
			#endif
	  		OSQPost(LoraQHandle, (pData+i)->datamsg);
		}

		for(int i = 0; i < 9; i++ )
			OSTimeDly(12000);	//14 minute delay
	}
}

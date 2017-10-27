#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#define SIZE 11

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


void floattohexstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x%08x", pdata->address,pdata->vochtigheid.ulongvalue);   
}


void main() {
  // put your main code here, to run repeatedly:
  DATA data;
  PDATA pdata = &data;
  memset(pdata->datamsg,0,SIZE);
  pdata->vochtigheid.floatvalue = 99.99;
  pdata->address = 5;
  
floattohexstring(pdata);
printf("%s\n",pdata->datamsg);

}



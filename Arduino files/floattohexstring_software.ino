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

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  PDATA pdata = (PDATA)malloc(sizeof(DATA));
  memset(pdata->datamsg,1,SIZE);
  pdata->vochtigheid.floatvalue = 99.99;
  pdata->address = 5;

  floattohexstring(pdata);

  while(1)
  {
    Serial.println(pdata->datamsg);
    
  }
  free(pdata);
}

void floattohexstring(PDATA pdata)
{
sprintf(pdata->datamsg,"%02x%08x", pdata->address,pdata->vochtigheid.ulongvalue);   
}



/* 
 * In deze file staat de getData functie. 
 * Als de gebruiker een sensorwaarde wil aanvragen aan een van de I2C verbonden
 * devices kan dat met de functie getDat(). Deze functie heeft als enige 
 * parameter het adres van het I2C device in de standaardvorm (niet bitshifted).
 * 
 * Als dit adres niet bestaat aan de I2C bus zal deze functie vast lopen in zijn
 * huidige staat. Voor instructie voor een fix neem contact op met Kaya Hartwig.
 * 
 * De return is een float waarde die door het I2C device is over gestuurt.
 * 
 * Voor het aanroepen van deze functie moet de I2Cinit() functie een maal
 * aangeroepen zijn geweest. Deze functie staat beschreven in de file I2C_init.c
 * 
 * (c) Kaya Hartwig 
 */



#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define SLAVE_ADDRESS 20
#define AMOUNT 4

union makefloat {
	float f;
	char str[AMOUNT];
};

float getData(char address)
{
	union makefloat make;

	address = address << 1; // ruimte maken voor de R/W bit

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); //wacht tot I2C niet busy is

	I2C_GenerateSTART (I2C1, ENABLE); 		    //stuur START

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); //wachten tot de slave ACKt

	I2C_Send7bitAddress (I2C1, address, I2C_Direction_Receiver); 	//stuur adres en stel in als receiver

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //wacht op ACK master receiver mode

	I2C_AcknowledgeConfig(I2C1, ENABLE); //voor de zekerheid ACK enablen

	int i;

	for (i = 0;  i < AMOUNT; i++)
	{
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); //loop tot er een byte is received

		make.str[i] = I2C_ReceiveData (I2C1);	 				    //leest de i2c buffer uit MISSCHIEN IS DIE NIET ZO SNEL VOL PAS OP
	}

	I2C_GenerateSTOP(I2C1, ENABLE); //tijd om de i2C communicatie stop te zetten

	return make.f;


}

#include <includes.h>

/*	in dit bestand staat de I2Cinit functie. deze moet voor het gebruiken van I2C
 * 	aangeroepen worden.
 */



void I2Cinit(void) {

	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	// enable APB1 peripheral clock voor I2C1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	// enable clock voor SCL en SDA pinnen
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	// SCL on PB6 and SDA on PB7

	// configure GPIO
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // we gebruiken PB6 en PB7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// de pins worden alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// de output moet open drain zijn
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up
	GPIO_Init(GPIOB, &GPIO_InitStruct);					// init GPIOB

	// configure I2C1
	I2C_InitStruct.I2C_ClockSpeed = 100000; 		// 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			// I2C modus
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// 50% duty cycle
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			// master heeft 0 als adres
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;		// ACK is uit
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7 bits adressen
	I2C_Init(I2C1, &I2C_InitStruct);				// init I2C1

	// enable I2C1
	I2C_Cmd(I2C1, ENABLE);


}

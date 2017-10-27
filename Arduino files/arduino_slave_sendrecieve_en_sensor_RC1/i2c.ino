/* de functie startI2C krijgt een byte tussen 1 en 127. Dit wordt 
 * het adres van de arduino slave. de functie returnt 0 als het
 * geslaagt is en 1 als de gegeven byte niet binnen het I2C
 * adresdomein voor een slave past (8 tot 120)
 */

#include <Wire.h>

#define I2C_REQUEST_INTERRUPT sendFreq //de naam van de aan te roepen functie als er een i2c request is zonder "()" 
#define I2C_RECIEVE_INTERRUPT I2CRecieve

extern union I2C buff;


int startI2C(byte address) { // in de toekomst willen we misschien via seriele communicatie de arduino een adres geven. daarom is het adres niet vast
  
  if (address > 7 && address < 121) {
    
    Wire.begin(address);          //het adres word de gegeven int address
    Wire.onRequest(I2C_REQUEST_INTERRUPT); //maak een interrupt voor als er een i2c request is binnengekomen
    Wire.onReceive(I2C_RECIEVE_INTERRUPT); //interrupt als er data ontvangen word
    return 0;
  }
  return 1;
}

void sendFreq() {
  Wire.write(buff.I2Cdata, 4);
}

void I2CRecieve() { //lees de string uit voor je nieuwe data ontvangt. de string word overschreven
   for(int i = 0; Wire.available(); i++)
   {
    recieveBuff[i] = Wire.read();
   }
}


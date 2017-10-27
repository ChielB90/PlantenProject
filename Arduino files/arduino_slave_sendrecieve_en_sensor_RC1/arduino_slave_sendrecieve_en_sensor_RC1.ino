#define SENSORPIN 5
#define SLAVE_ADDRESS 20 

char recieveBuff[256]; //voor de I2CRecieve interrupt. 

union I2C {
  float f;
  byte I2Cdata[4];
}; //omdat de i2c send functie een groep bytes verwacht

uint8_t SensorPin_mask;
volatile uint8_t *SensorPin_port;

void setup() {
  Serial.begin(9600);  //bij voorkeur maak ik deze sneller (bijv 250000)
  Serial.print("startI2C returns: ");
  Serial.println(startI2C(SLAVE_ADDRESS));

  SensorPin_mask = digitalPinToBitMask(SENSORPIN);
  SensorPin_port = portInputRegister(digitalPinToPort(SENSORPIN));

}

void loop() {
  prepareI2Cdata(checkHumidity());
  
  
}

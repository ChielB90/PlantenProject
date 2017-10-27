/* deze functie checkt een millisecone lang hoeveel statechanges
 * er zijn op de pin. gedeelt door twee om van de state changes
 * aantal periodes te maken en keer duizend om er het aantal 
 * periodes per seconde van te maken.
 * de functie returnt een float. de ferquentie.
 * SENSORPIN moet in de main file gedefinet worden.
 */

float checkHumidity() //checkDankness of checkDank zou beter kloppen
{
  float count = 0;
  int pinState = (*SensorPin_port & SensorPin_mask);
  //int pinState = digitalRead(SENSORPIN);
  
  //unsigned long test1 = micros();
    
  for(unsigned long i = micros() + 1000; i > micros();) 
  {
    if (pinState != (*SensorPin_port & SensorPin_mask)) count++;
    //if (pinState != digitalRead(SENSORPIN)) count++;
    pinState = (*SensorPin_port & SensorPin_mask);
    //pinState = digitalRead(SENSORPIN);
  }

  //unsigned long test2 = micros();

  Serial.print("frequency is: ");
  Serial.println(count * 500);

  //Serial.println(test1);
  //Serial.println(test2);
  
  return count * 500; 
}

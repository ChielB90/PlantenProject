int interruptPin = 2;
volatile unsigned long beg = 0;
volatile unsigned long tot = 0;
unsigned long tijd = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPin), rise1, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis()-tijd > 1000){
   Serial.println(tot);
   tijd = millis();
  }
}
void rise1(void){
  beg = micros();
  attachInterrupt(digitalPinToInterrupt(interruptPin), rise2, RISING);
}

void rise2(void){
    tot = micros() - beg;
    attachInterrupt(digitalPinToInterrupt(interruptPin), rise1, RISING);
}


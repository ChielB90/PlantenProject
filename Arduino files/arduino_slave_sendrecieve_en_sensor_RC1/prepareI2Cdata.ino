union I2C buff; //globaal omdat je geen params mee kan geven in een interrupt

void prepareI2Cdata(float frequency){
  buff.f = frequency;
}


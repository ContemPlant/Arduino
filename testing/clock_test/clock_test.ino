//----rt clock-----
  #include "DS1307.h"
  DS1307 clock;//define a object of DS1307 class


void printTime() {
    Serial.print(clock.hour);
    Serial.print(":");
    Serial.println(clock.minute);
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);

    clock.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  clock.getTime();
  printTime();
  delay(1000);
}

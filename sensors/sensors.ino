#include "DHT.h"

#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 4

SoftwareSerial xbee(rxPin, txPin);

//define sensor pins
#define MOISTURE_SENSOR A0
#define HUMIDITY_TEMPERATURE 2
#define CLOCK D1

//setup temperature and humidity sensor
#define DHTTYPE DHT22
DHT dht(HUMIDITY_TEMPERATURE, DHTTYPE);


//setup radiation sensor
#include <Wire.h>
#include "Arduino.h"
#include "SI114X.h"

SI114X radsens = SI114X();

//setp lcd display
#include <I2C_LCD.h>
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

typedef struct _data {
  uint8_t flags;
  uint8_t pid;
  unsigned long timestamp;
  uint8_t health;
  uint8_t psize;
  float temp;
  float hum;
  float rad; 
}data;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOISTURE_SENSOR, INPUT);
  pinMode(HUMIDITY_TEMPERATURE, INPUT);
  
  Serial.begin(9600);
  xbee.begin(9600);
  
  dht.begin();
  //start i2c communication
  Wire.begin();

  //start radiation sensor
  while (!radsens.Begin()) {
    delay(500);
  }

  //setup LCD
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    
  //8*16 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  
  LCD.println("Temp: ");
  LCD.println("Humidity: ");
  LCD.println("Radiation: ");
}

float temperature() {
  return dht.readTemperature();
}

float humidity() {
  return dht.readHumidity();
}


float radiation() {
  return (radsens.ReadVisible() + radsens.ReadIR() + (radsens.ReadUV()/100.0))/3.0;
}

int timestamp () {
  
}

int health () {
  return 0;
}

char* marshall () {
  
}

void sendStruct(data* d) {
  xbee.write((uint8_t*) d, sizeof(data));
  xbee.print("\n");
}


void loop() {
 
  if(true || xbee.available() && xbee.read() > 0) {
    data* it = calloc(sizeof(data), 1);
    it->flags = (int) 0b11111111;
    it->pid = 1;
    it->timestamp = 1526299756;
    it->health = health();
    it->psize = 1;
    it->temp = temperature();
    it->hum = humidity();
    it->rad = radiation();
    
    sendStruct(it);
    
    free(it);
    delay(3000);
  }
      
  LCD.CharGotoXY(80,0);
  LCD.print(temperature());
  LCD.CharGotoXY(80,16);
  LCD.print(humidity());
  LCD.CharGotoXY(80,32);
  LCD.print(radiation());

}

#define AID 0 // Unique arduino id

// Temperatur und Luftfeutchtigkeit Lib
#include "DHT.h"

// Xbee API Lib
#include <XBee.h>
#define PI_ADR 0x0

// Software serial com with xbee
#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 4
SoftwareSerial softwareSerial(rxPin, txPin);

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
  uint8_t aid;
  unsigned long timestamp;
  uint8_t health;
  uint8_t psize;
  float temp;
  float hum;
  float rad; 
}data;

// Create an XBee object at the top of your sketch
XBee xbee = XBee();

void setup() {
  // put your setup code here, to run once:
  pinMode(MOISTURE_SENSOR, INPUT);
  pinMode(HUMIDITY_TEMPERATURE, INPUT);
  
  Serial.begin(9600);
  softwareSerial.begin(9600);
  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(softwareSerial);
  
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

bool sendStructTo(uint16_t addr16, data* payload) {
//  softwareSerial.write((uint8_t*) d, sizeof(data));
//  softwareSerial.print("\n");
  
  // Create a TX Request
  Tx16Request zbTx = Tx16Request(addr16, (uint8_t*) payload, sizeof(data));
  // Send your request
  xbee.send(zbTx);

//  if (xbee.getResponse().isAvailable()) {
//    Rx16Response rx16 = Rx16Response();
//    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
//      xbee.getResponse().getRx16Response(rx16);
//      Serial.print(rx16.getRemoteAddress16());
//      //Serial.print(rx16);
//      return true;
//    }
//  }
  return false;
  
}

void structToLCD(data* d) {
  LCD.CharGotoXY(80,0);
  LCD.print(d->temp);
  LCD.CharGotoXY(80,16);
  LCD.print(d->hum);
  LCD.CharGotoXY(80,32);
  LCD.print(d->rad);
}


void loop() {
  data* it = calloc(sizeof(data), 1);
  it->flags = (int) 0b11111111;
  it->pid = 1;
  it->aid = AID;
  it->timestamp = 1526299756;
  it->health = health();
  it->psize = 1;
  it->temp = temperature();
  it->hum = humidity();
  it->rad = radiation();
  
  sendStructTo(PI_ADR, it);
  structToLCD(it);
  
  free(it);
  delay(3000);

}

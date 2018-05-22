
#include "DHT.h"

//define sensor pins
#define HUMIDITY_TEMPERATURE 2
#define CLOCK D1
#define LOUDNESS A3

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
I2C_LCD LCD2;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

int loudness_db = 0;
float temp_deg = 23.0;
float humidity_proz = 50.0;
float sunlight_lumen = 260.0;

float temp_weight = 0.4;
float humidity_weight = 0.2;
float sunlight_weight = 0.4;

void setup() {
  // put your setup code here, to run once:
  pinMode(HUMIDITY_TEMPERATURE, INPUT);
  
  Serial.begin(9600);
  dht.begin();
  //start i2c communication
  Wire.begin();

  //start radiation sensor
  while (!radsens.Begin()) {
    delay(500);
  }

  //setup LCD
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
  LCD2.CleanAll(WHITE);
    
  //8*16 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  LCD.println("Temp: ");
  LCD.println("Hum: ");
  LCD.println("Rad: ");
  LCD.println("Env: ");
}

float temperature() {
  return dht.readTemperature();
}

float humidity() {
  return dht.readHumidity();
}

float radiation() {
  return radsens.ReadVisible();
}

int timestamp () {
  
}

float health (float temp, float hum, float light) {
  float diff_temp = abs(temp - dht.readTemperature());
  float diff_hum = abs(hum - dht.readHumidity());
  float diff_light = abs(light - radsens.ReadVisible());

  return (temp_weight * healthFnct(10.0, diff_temp) + humidity_weight * healthFnct(20.0, diff_hum) + 
          sunlight_weight * healthFnct(100.0, diff_light));
}

float healthFnct (float maxi, float deviation) {
  if (maxi < deviation) return 0;
  
  return ((2/(pow(maxi,3))) * (pow(deviation,3))) - ((3/(pow(maxi,2)))*(pow(deviation,2))) + 1;
}

char* marshall () {
  
}



void loop() {
  // put your main code here, to run repeatedly:
  LCD.CharGotoXY(80,0);
  LCD.print(temperature());
  LCD.CharGotoXY(80,16);
  LCD.print(humidity());
  LCD.CharGotoXY(80,32);
  LCD.print((int) radiation());
  LCD.CharGotoXY(80,48);
  LCD.print(analogRead(LOUDNESS));
  //LCD.print(health(temp_deg, humidity_proz, sunlight_lumen) * 100);

  delay(100);
}

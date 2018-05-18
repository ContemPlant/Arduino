//[16:05, 12.5.2018] Timon Palme: 
/*#include "DHT.h"

//define sensor pins
#define MOISTURE_SENSOR A0
#define HUMIDITY_TEMPERATURE 2
#define CLOCK D1

//setup temperature and humidity sensor
#define DHTTYPE DHT22
//DHT dht(HUMIDITY_TEMPERATURE, DHTTYPE);


//setup radiation sensor
#include <Wire.h>
#include "Arduino.h"
#include "SI114X.h"

SI114X radsens = SI114X();

//setp lcd display
#include <I2C_LCD.h>
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

float temp_deg = 20.0;
float humidity_proz = 0.3;
float sunlight_lumen = 400;
float moisture_proz = 0.1;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOISTURE_SENSOR, INPUT);
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
    
  //8*16 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  LCD.println("Temp: ");
  LCD.println("Humidity: ");
  LCD.println("Moisture: ");
  LCD.println("Radiation: ");
}

float temperature() {
  return dht.readTemperature();
}

float humidity() {
  return dht.readHumidity();
}

float moisture() {
  return analogRead(MOISTURE_SENSOR)/1023.0;
}

float radiation() {
  return (radsens.ReadVisible() + radsens.ReadIR() + (radsens.ReadUV()/100.0))/3.0;
}

int timestamp () {
  
}

float health (float temp, float hum, float light, float moist) {
  float diff_temp = temp - dht.readTemperature();
  float diff_hum = hum - dht.readHumidity();
  float diff_light = light - radsens.ReadVisible();
  float diff_moist = moist - analogRead(MOISTURE_SENSOR)/1023.0;
  
  diff_temp *= diff_temp;
  diff_hum *= diff_hum;
  diff_light *= diff_light;
  diff_moist *= diff_moist;
  

  return (healthFnct(30, diff_temp) + healthFnct(0.3, diff_hum) + healthFnct(200, diff_light) + healthFnct(0.2, diff_moist))/4;
}

float healthFnct (float maxi, float deviation) {
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
  LCD.print(moisture());
  LCD.CharGotoXY(80,48);
  LCD.print(radiation());
  Serial.print(health(temp_deg, humidity_proz, sunlight_lumen, moisture_proz));

}
*/
#include "DHT.h"

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

float temp_deg = 27.0;
float humidity_proz = 30.0;
float sunlight_lumen = 250.0;
float moisture_proz = 0.1;

float temp_weight = 1.0;
float humidity_weight = 1.0;
float sunlight_weight = 1.0;
float moisture_weight = 1.0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOISTURE_SENSOR, INPUT);
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
    
  //8*16 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  LCD.println("Temp: ");
  LCD.println("Humidity: ");
  LCD.println("Moisture: ");
  LCD.println("Radiation: ");
}

float temperature() {
  return dht.readTemperature();
}

float humidity() {
  return dht.readHumidity();
}

float moisture() {
  return analogRead(MOISTURE_SENSOR)/1023.0;
}

float radiation() {
  return radsens.ReadVisible();
}

int timestamp () {
  
}

float health (float temp, float hum, float light, float moist) {
  float diff_temp = abs(temp - dht.readTemperature());
  float diff_hum = abs(hum - dht.readHumidity());
  float diff_light = abs(light - radsens.ReadVisible());
  float diff_moist = abs(moist - analogRead(MOISTURE_SENSOR)/1023.0);

  return (temp_weight * healthFnct(10.0, diff_temp) + humidity_weight * healthFnct(20.0, diff_hum) + 
          sunlight_weight * healthFnct(100.0, diff_light) + moisture_weight * healthFnct(0.2, diff_moist))/4;
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
  LCD.print(moisture());
  LCD.CharGotoXY(80,48);
  LCD.print(radiation());
  Serial.println(health(temp_deg, humidity_proz, sunlight_lumen, moisture_proz));

}

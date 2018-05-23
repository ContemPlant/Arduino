#include "DHT.h"

//define sensor pins
  #define HUMIDITY_TEMPERATURE 2
  #define CLOCK D1
  #define LOUDNESS A3

//----humidity and temerature sensor----
  #define DHTTYPE DHT22
  DHT dht(HUMIDITY_TEMPERATURE, DHTTYPE);


//----radiation sensor----
  #include <Wire.h>
  #include "Arduino.h"
  #include "SI114X.h"
  SI114X radsens = SI114X();

//----lcd display----
  #include <I2C_LCD.h>
  I2C_LCD LCD;
  I2C_LCD LCD2;
  uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

//----best values----
  int loudness_db = 0;
  float temp_deg = 23.0;
  float humidity_proz = 50.0;
  float sunlight_lumen = 260.0;

//----weights----
  float temp_weight = 0.4;
  float humidity_weight = 0.2;
  float sunlight_weight = 0.4;
  
// struct for storing data
typedef struct data_ data;
struct data_{
  uint32_t time;      //minutes since 1900-01-01
  uint8_t comp;       //amount of packets merged into one
  int8_t temp;        //temperature in deegrees celsius *10
  uint8_t hum;        //humidity in percent
  uint16_t rad;       //visible light in lumen
  uint8_t loud;       //loudness in decibel
  uint8_t health;     //plant health in percent
  uint8_t weight;     //plant weight in kilogram
};

void setup();
void loop();



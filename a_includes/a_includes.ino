//----EEPROM----
  #include <EEPROM.h>

//define sensor pins
  #define HUMIDITY_TEMPERATURE 2
  #define CLOCK D1
  #define LOUDNESS A3

//----humidity and temerature sensor----
  #include "DHT.h"
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
  uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

//----define----
  #define MEMORY_SIZE 1024
  #define TEMP_MEMORY_SIZE 16 //measured in packets
  #define DEFAULT_PLANT_ID 0
  
//----structs----
typedef struct data_{
  uint32_t time;      //minutes since 1900-01-01
  uint8_t comp;       //amount of packets merged into one
  int16_t temp;        //temperature in deegrees celsius *10
  uint8_t hum;        //humidity in percent
  uint16_t rad;       //visible light in lumen
  uint8_t loud;       //loudness in decibel
}data;

typedef struct plant_info_{
  uint32_t id;
  float temp_opt;
  float temp_weight;
  float hum_opt;
  float hum_weight;
  float rad_opt;
  float rad_weight;
  float loud_opt;
  float loud_weight;
}plant_info;

//----global variables----
  int startWriteAddress;
  int currentWriteAddress;
  int currentWriteAddressTempMem;
  int currentCompressionLevel;
  int maxCompressionLevel;
  data** temp_mem;  //"temporary memory" for saving data packets
  plant_info* plant;  //store info about current plant
  int loopno = 0;
  
void setup(){
  setup2();
}
void loop(){
  loop2();
}



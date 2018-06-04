//----EEPROM----
  #include <EEPROM.h>

//define sensor pins
  #define HUMIDITY_TEMPERATURE 2
  #define CLOCK D1
  #define LOUDNESS A3

//--xbee--
  // Xbee API Lib
  #include <XBee.h>
  #define PI_ADR 0x0
  #define ARD_ADR 0x0001

  // Software serial com with xbee
  #include <SoftwareSerial.h>
  #define rxPin 3
  #define txPin 4
  SoftwareSerial softwareSerial(rxPin, txPin);

  // Create an XBee object at the top of your sketch
  XBee xbee = XBee();

  // Setup response memory
  Rx16Response rx16 = Rx16Response();


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

//----rt clock-----
  #include "DS1307.h"
  DS1307 clock;//define a object of DS1307 class

//----define----
  #define MEMORY_SIZE 1024    //measured in bytes
  #define TEMP_MEMORY_SIZE 1 //measured in packets
  #define DEFAULT_PLANT_ID 0
  // flags
  #define SIGNIN  0b00000001
  #define SIGNOFF 0b00000010
  #define DATA    0b00000011
  
//----structs----
typedef struct data_{
  uint32_t timestamp;      //minutes since 1900-01-01
  uint8_t comp;       //amount of packets merged into one
  float temp;        //temperature in deegrees celsius *10
  float hum;        //humidity in percent
  float rad;       //visible light in lumen
  float loud;       //loudness in decibel
}data;

typedef struct msg_ {
  uint8_t flags;
  uint16_t sourceID;
  int32_t timestamp;
  uint8_t compression;
  float temp;
  float hum;
  float rad;
  float loud;
}msg;

typedef struct plant_info_{
  uint8_t flags;
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
  int loopno = 0;   //number of loops executed
  boolean active;
  
void setup(){
    Serial.begin(9600);
    softwareSerial.begin(9600);
    Serial.println("Setting up...");

    // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
    xbee.setSerial(softwareSerial);

    //start i2c communication (LCD and Sunlight Sensor)
    Wire.begin();

    clock.begin();
    //set up sensors
    setup_sensors();

    //initialize global variables and load plant
    setup_vars();

    // set up lcd screen
    setup_lcd();

    Serial.println("Setup finished.");

}

void loop(){
    // allocate memory for new data packet
  data* new_data = (data*) calloc(sizeof(data), 1);

  fill_in_sensor_data(new_data);

  msg* payload = packMsg(new_data);
  Serial.println("sending message.");
  print_msg(payload);
  sendStructTo(PI_ADR, payload);
  free(payload);
  
  // receive data and update plant
  Serial.println("receiving data...");
  receiving();

  // print environment parameters
  print_on_lcd();

  printTime();

  // write to eeprom if temp mem is full
  if (currentWriteAddressTempMem >= TEMP_MEMORY_SIZE)
  {
    Serial.println("temp mem full. writing to eeprom...");
    write_temp_to_perm();
  }

  loopno++;
  Serial.println("------end-of-loop------");
  free(new_data);
  delay(1000);
}

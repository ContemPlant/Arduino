//----OLED----
#include <SeeedGrayOLED.h>

//----EEPROM----
#include <EEPROM.h>

// Define sensor pins.
#define HUMIDITY_TEMPERATURE 2
#define BUTTON 5
#define CLOCK D1
#define LOUDNESS A3

//----XBEE----
// Xbee API Lib
#include <XBee.h>
#define PI_ADR 0x0
#define ARD_ADR 0x0002

// Software serial com with xbee
#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 4
SoftwareSerial softwareSerial(rxPin, txPin);

// Create an XBee object at the top of your sketch.
XBee xbee = XBee();

// Set up response memory.
Rx16Response rx16 = Rx16Response();
TxStatusResponse txStatus = TxStatusResponse();

//----Humidity and Temerature sensor----
#include "DHT.h"
#define DHTTYPE DHT22
DHT dht(HUMIDITY_TEMPERATURE, DHTTYPE);

//----Radiation sensor----
#include <Wire.h>
#include "Arduino.h"
#include "SI114X.h"
SI114X radsens = SI114X();

//----LCD display----
#include <I2C_LCD.h>
I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

//----RT Clock-----
#include "DS1307.h"
DS1307 clock;//define a object of DS1307 class

//----define----
#define MEMORY_SIZE 1024    //measured in bytes
#define TEMP_MEMORY_SIZE 2 //measured in packets
#define QUEUE_SIZE 16
#define DEFAULT_PLANT_ID 0
// flags
#define SIGNIN  0b00000001
#define SIGNOFF 0b00000010
#define DATA    0b00000011

#define MAX_TRIES 3
#define END_CHAR 0b00000111

//----structs----
typedef struct data_ {
  uint32_t timestamp;     //minutes since 1900-01-01
  uint8_t comp;           //amount of packets merged into one
  float temp;             //temperature in deegrees celsius
  float hum;              //humidity in percent
  float rad;              //visible light in lumen
  float loud;             //loudness in decibel
} data;

typedef struct queue_elem_ queue_elem;
struct queue_elem_ {
  data* body;
  queue_elem* next;
};

typedef struct queue_ {
  queue_elem* first;
  queue_elem* last;
  uint8_t count;
} queue;

typedef struct msg_ {     //data packet Ardu -> PI
  uint8_t flags;          //DATA
  uint16_t sourceID;      //ARD_ADR
  int32_t timestamp;
  uint8_t compression;
  float temp;
  float hum;
  float rad;
  float loud;
} msg;

typedef struct plant_info_ {
  uint8_t flags;          //e.g. SIGNIN
  float temp_opt;
  float temp_weight;
  float hum_opt;
  float hum_weight;
  float rad_opt;
  float rad_weight;
  float loud_opt;
  float loud_weight;
} plant_info;

//----global variables----
int startWriteAddress;
int currentWriteAddress;
int currentWriteAddressTempMem;
int currentCompressionLevel;
int maxCompressionLevel;

uint16_t eepromPacketSpace = sizeof(plant_info) + 1;
uint16_t eepromMaxPackets = (EEPROM.length() - eepromPacketSpace) / sizeof(data);
uint16_t eepromOldestPacket = eepromPacketSpace;
uint16_t eepromNumPackets = 0;

uint8_t tries = 0;  //stores the number of sending attempts

plant_info* plant;  //store info about current plant
int loopno = 0;     //number of loops executed
boolean active;     //true if plant is loaded on to Ardu

const char qrcode[] PROGMEM = "111111100000001111111100000100000101000001101110101001001011101101110101111101011101101110100110101011101100000100000001000001111111101010101111111000000000101100000000000110110101100001100011010010111001010101011100110110100011001101111010110110001100111100111100011001111000000001000111101011111111101011011110010100000100010011111011101110101100110101101101110101110010011100101110100110110000111100000100111111010101111111100100011001110";

queue* packetQueue;

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up...");

  // Tell XBee to use Software Serial.
  softwareSerial.begin(9600);
  xbee.setSerial(softwareSerial);

  // Start I2C communication (LCD and Sunlight Sensor).
  Wire.begin();

  // Set up button.
  pinMode(BUTTON, INPUT);

  // Set up sensors.
  setup_sensors();

  // Set up clock.
  clock.begin();

  // Initialize global variables and load plant.
  setup_vars();

  // Set up LCD screen. This needs to be called after setup_vars()!
  setup_lcd();

  // Set up OLED display.
  setup_oled();

  Serial.println("Setup finished.");
}

void loop() {
  if (active) {
    // allocate memory for new data packet
    data* new_data = (data*) calloc(sizeof(data), 1);

    Serial.println("test");
    //fill in sensor data and append packet to queue
    fill_in_sensor_data(new_data);
    queue_append(packetQueue, new_data);

    /*if (eepromNumPackets != 0){
      Serial.println("sending eeprom.");
      send_eeprom();
    }*/
    send_queue();

    if (packetQueue->count >= QUEUE_SIZE) {
      Serial.println("moving packets from queue to eeprom");
      packet_to_eeprom(queue_compress(packetQueue));
      Serial.println("empyting queue");
      queue_empty(packetQueue);
    }

    // print environment parameters
    lcd_print_sensors();
    lcd_print_loop();
    lcd_print_clock();
    lcd_print_eeprom();

    //check button press
    read_button();
  }
  else {
    Serial.println("plant not active. Doing nothing.");
  }

  // receive data and update plant
  Serial.println("receiving data...");
  receiving();

  // print time on serial
  printTime();

  loopno++;
  Serial.println("------end-of-loop------");
  delay(500);
}

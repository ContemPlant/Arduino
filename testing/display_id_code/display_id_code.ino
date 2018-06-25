#include <qrcodegen.h>

//----XBEE----
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

//----lcd display----
  #include <I2C_LCD.h>
  I2C_LCD LCD;
  uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

char* recv_data(){
  char* buffer = (char*) calloc(10, 1); 

  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      for (int i = 0; i < rx16.getDataLength(); i++) { 
        Serial.print(rx16.getData(i)); 
        buffer[i] = rx16.getData(i);
      } 
    }
  }

  return buffer;
}

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(void) {
  const char *text = "Hello, world!";  // User-supplied text
  enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level
  
  // Make and print the QR Code symbol
  uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
  bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
    qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
  if (ok)
    printQr(qrcode);
  else
    printf("error\n");
}


void setup(){
  Serial.begin(9600);
  softwareSerial.begin(9600);
  Serial.println("Setting up...");

  Wire.begin();

  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(softwareSerial);

  Serial.println("Setup finished.");

  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
  LCD.DrawVLineAt(0, 63, 20, BLACK);
}

void loop(){
  delay(1000);
}

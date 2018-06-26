#include <qrcodegen.h>

//----lcd display----
  #include <I2C_LCD.h>
  I2C_LCD LCD;
  uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.

const char qrcode[] = "1111111001000011001111111100000101001001100100000110111010010100001010111011011101000000111001011101101110100111011100101110110000010100111101010000011111111010101010101111111000000001111111110000000000001111001011000011000101111010100101101101000001110010111011011011001111001110001100010011001111100001011000101001101010001100111010101010001111000100011010010001110011001100001110101110101111001110111000111010011011111000100000000100001001000111011111111010100011101011010100000101011010010001001010111010100000111111100111011101000110000110110001101110100000000001011011010000010001000101100111101111111001011010011001011";

void lcd_setup_and_print_qr(char* qr, int len){
  int vert_size = sqrt(len);
  for(int i = 0; i < len; i++)
  {
    if (qr[i] == '1') {
      // draw 2x2 rectangle
      LCD.DrawRectangleAt(2 * (i % vert_size), 2 * (i / vert_size), 2, 2, BLACK_FILL);
    }
  }
  
}

void setup(){
  Serial.begin(9600);
  Serial.println("Setting up...");

  Wire.begin();

  Serial.println("Setup finished.");

  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
  LCD.DrawDotAt(127, 63, BLACK);
  int len = strlen(qrcode);
  Serial.print("länge: ");
  Serial.println(len);
  lcd_setup_and_print_qr(qrcode, strlen(qrcode));
}

void loop(){
  delay(1000);
}

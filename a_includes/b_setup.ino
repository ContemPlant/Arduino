void setup() {
  // put your setup code here, to run once:
  pinMode(HUMIDITY_TEMPERATURE, INPUT);
  
  Serial.begin(9600);
  dht.begin();
  //start i2c communication
  Wire.begin();

  //----radiation sensor----
    while (!radsens.Begin()) {
      delay(500);
    }

  //----LCD----
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

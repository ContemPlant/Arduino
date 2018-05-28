// set up sensors
void setup_sensors(){
  // temperature and humidity
  pinMode(HUMIDITY_TEMPERATURE, INPUT);
  dht.begin();
  
  //radiation sensor
  while (!radsens.Begin()) {
    delay(500);
  }
}

//initialize global variables
void setup_vars(){
  startWriteAddress = sizeof(plant_info);
  currentWriteAddress = startWriteAddress;
  currentWriteAddressTempMem = 0;
  currentCompressionLevel = 0;
  maxCompressionLevel = 1;
  temp_mem = (data**) malloc(TEMP_MEMORY_SIZE * sizeof(data*));
  plant = (plant_info*) malloc(sizeof(plant_info));

  // check if a plant is saved in eeprom
  uint32_t i;
  if (EEPROM.get(0,i) == DEFAULT_PLANT_ID)
  {
    Serial.println("no plant in eeprom -> loading default values...");
    plant->id = DEFAULT_PLANT_ID;

    plant->temp_opt = 23.0;
    plant->hum_opt = 50.0;
    plant->rad_opt = 500.0;
    plant->loud_opt = 0.0;

    plant->temp_weight = 1;
    plant->hum_weight = 0.5;
    plant->rad_weight = 0.7;
    plant->loud_weight = 1;
  }
  else
  {
    // load plant from eeprom
    Serial.print("plant found in eeprom -> loading plant...");
    read_data(0, (char*) plant, sizeof(plant));
    Serial.print("loaded plant with id=");
    Serial.print(plant->id);
    Serial.print("\n");
  }

}

// set up lcd screen
void setup_lcd(){
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    
  //8*16 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  LCD.println("ID: ");
  LCD.println("Temp: ");
  LCD.println("Hum: ");
  LCD.println("Rad: ");
  LCD.println("Loud: ");
  LCD.println("Env: ");
  LCD.println("loop: ");
  LCD.println("memory t:    p:");
  // print units
  LCD.CharGotoXY(100,8);
  LCD.print("C");
  LCD.CharGotoXY(100,16);
  LCD.print("%");
  LCD.CharGotoXY(100,24);
  LCD.print("lm");
  LCD.CharGotoXY(100,32);
  LCD.print("db");
  LCD.CharGotoXY(100,40);
  LCD.print("%");

}

void setup2() {
  Serial.begin(9600);
  Serial.println("Setting up...");

  //start i2c communication (LCD and Sunlight Sensor)
  Wire.begin();

  //set up sensors
  setup_sensors();

  //initialize global variables and load plant
  setup_vars();

  // set up lcd screen
  setup_lcd();

  Serial.println("Setup finished.");
}

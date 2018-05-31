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

void load_default_plant(){
  plant->temp_opt = 23.0;
  plant->hum_opt = 50.0;
  plant->rad_opt = 500.0;
  plant->loud_opt = 0.0;

  plant->temp_weight = 1;
  plant->hum_weight = 0.5;
  plant->rad_weight = 0.7;
  plant->loud_weight = 1;
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
  if (EEPROM[0] == DEFAULT_PLANT_ID)
  {
    Serial.println("no plant in eeprom -> loading default values...");
    load_default_plant();
    active = false;
  }
  else
  {
    // load plant from eeprom
    Serial.print("plant found in eeprom -> loading plant...");
    read_data(0, (uint8_t*) plant, sizeof(plant));
    Serial.print("loaded plant with temp_opt=");
    Serial.print(plant->temp_opt);
    Serial.print("\n");
    active = true;
  }

}

// set up lcd screen
void setup_lcd(){
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    
  //6*8 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  LCD.println("temp_opt: ");
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

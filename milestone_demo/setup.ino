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
  //temp_mem = (data**) malloc(TEMP_MEMORY_SIZE * sizeof(data*));
  plant = (plant_info*) malloc(sizeof(plant_info));
  packetQueue = queue_create();

  // check if a plant is saved in eeprom
  if (EEPROM[0] == SIGNOFF)
  {
    Serial.println("no active plant in eeprom -> loading default values...");
    load_default_plant();
    active = false;
  }
  else
  {
    // load plant from eeprom
    Serial.print("plant found in eeprom -> loading plant...");
    read_eeprom(0, (char*) plant, sizeof(plant_info));
    print_plant_info(plant);
    active = true;
  }

}

// set up lcd screen
void setup_lcd(){
  if (active){
    lcd_setup_sensors();
    lcd_setup_loop();
    lcd_setup_clock();
  }
  else{
    lcd_setup_id(); //prepare screen for displaying arduino id
  }
}

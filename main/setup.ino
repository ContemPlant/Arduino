// set up sensors
void setup_sensors(){
  // temperature and humidity
  pinMode(HUMIDITY_TEMPERATURE, INPUT);
  dht.begin();
  
  //radiation sensor
  while (!radsens.Begin()) {
    Serial.println("Setting up Radiation Sensor failed! Waiting 500ms and trying again.");
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
  plant = (plant_info*) malloc(sizeof(plant_info));
  packetQueue = queue_create();

  // check if a plant is saved in eeprom
  if (EEPROM[0] == SIGNOFF){
    Serial.println("No active plant in EEPROM. Deactivate plant.");
    deactivate_plant();
  }
  else{
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
    deactivate_plant();
  }
}

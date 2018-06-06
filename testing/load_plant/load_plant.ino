//----EEPROM----
  #include <EEPROM.h>


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

plant_info* plant;  //store info about current plant

void load_default_plant(){
	plant->flags = SIGNOFF;
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
  plant = (plant_info*) malloc(sizeof(plant_info));
  load_default_plant();
  EEPROM.put(0, plant);
}

void setup(){
    Serial.begin(9600);
    Serial.println("Setting up...");
    //initialize global variables and load plant
    setup_vars();
    Serial.println("Setup finished.");
}

void loop(){
	delay(10000);
}
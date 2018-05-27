// returns all data packets that were recorded since last successful sending operation in a single buffer
char* select_data_to_send(){
  char* buffer = (char*) calloc(currentWriteAddress + currentWriteAddressTempMem, 1);

  // get data from eeprom
  for (int i = 0; i < currentWriteAddress; ++i)
  {
    buffer[i] = EEPROM[i];
  }

  // get data from temp mem
  char* read = (char*) temp_mem[0];
  for (int j = currentWriteAddress; j < currentWriteAddress + currentWriteAddressTempMem * sizeof(data); ++j)
  {
    buffer[j] = read[j - currentWriteAddress];
  }

  return buffer;
}

// 0 = success, -1 = failure
int sending(){
  return 0;
}

char* recv_data(){
  return "buffer";
}

// update plant values
void receiving(){
  plant_info* new_plant = (plant_info*) recv_data();

  float temp_opt = new_plant->temp_opt;
  float hum_opt = new_plant->hum_opt;
  float rad_opt = new_plant->rad_opt;
  float loud_opt = new_plant->loud_opt;

  float temp_weight = new_plant->temp_weight;
  float hum_weight = new_plant->hum_weight;
  float rad_weight = new_plant->rad_weight;
  float loud_weight = new_plant->loud_weight;
}

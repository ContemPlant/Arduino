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

// 1 = success, 0 = failure
int sending(){
  return 0;
}

//TODO
char* recv_data(){
  char* buffer = (char*) malloc(sizeof(plant_info));
  return buffer;
}

// update plant values
void receiving(){
  plant_info* new_plant = (plant_info*) recv_data();

  // only update plant if all data points of the old plant have been sent
  if (currentWriteAddress == startWriteAddress && currentWriteAddressTempMem == 0)
  {
    Serial.println("received new plant. updating values.");
    plant->temp_opt = new_plant->temp_opt;
    plant->hum_opt = new_plant->hum_opt;
    plant->rad_opt = new_plant->rad_opt;
    plant->loud_opt = new_plant->loud_opt;

    plant->temp_weight = new_plant->temp_weight;
    plant->hum_weight = new_plant->hum_weight;
    plant->rad_weight = new_plant->rad_weight;
    plant->loud_weight = new_plant->loud_weight;

    // save new plant to eeprom
    write_data(0, (char*) plant, sizeof(plant_info));
  }
  else
  {
    Serial.println("not all data points of old plant have been sent. doing nothing...");
  }

  free(new_plant);
}

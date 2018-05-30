void fill_in_sensor_data(data* new_data) {
  // fill struct fields
  Serial.println("getting sensor data...");
  new_data->time = get_time();
  new_data->comp = 1;
  new_data->temp = temperature() * 10; //save a decimal place by multiplying by 10
  new_data->hum = humidity();
  new_data->rad = radiation();
  new_data->loud = loudness();
}

void loop2() {
  // allocate memory for new data packet
  data* new_data = (data*) calloc(sizeof(data), 1);

  fill_in_sensor_data(new_data);

  // save data in temp memory
  temp_mem[currentWriteAddressTempMem] = new_data;
  currentWriteAddressTempMem++;

  // send data
  Serial.println("sending data...");
  if (sending())
  {
    Serial.println("Send Success.");
    // erase memory
    Serial.println("Clear Temp Mem.");
    clear_temp_mem();
    Serial.println("Clear Perm Mem.");
    clear_perm_mem();
  }
  else
  {
    Serial.println("Send Failure.");
  }

  // receive data
  Serial.println("receiving data...");
  receiving();

  // print environment parameters
  print_on_lcd();

  // write to eeprom if temp mem is full
  if (currentWriteAddressTempMem >= TEMP_MEMORY_SIZE)
  {
    Serial.println("temp mem full. writing to eeprom...");
    write_temp_to_perm();
  }
  loopno++;
  Serial.println("------end of loop------");
  delay(1000);
}


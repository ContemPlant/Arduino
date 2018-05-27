void loop2() {

	// allocate memory for new data packet
	data* new_data = (data*) calloc(sizeof(data), 1);

	// fill struct fields
	new_data->time = get_time();
	new_data->comp = 1;
	new_data->temp = temperature();
	new_data->hum = humidity();
	new_data->rad = radiation();
	new_data->loud = loudness();

	// save data in temp memory
	temp_mem[currentWriteAddressTempMem] = new_data;
	currentWriteAddressTempMem++;

	// send data
	send_data();
	
  // print environment parameters
  print_on_lcd();

  // write to eeprom if temp mem is full
  if (currentWriteAddressTempMem >= TEMP_MEMORY_SIZE)
  {
  	write_temp_to_perm();
  }
  delay(1000);
}


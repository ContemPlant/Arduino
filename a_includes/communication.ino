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
int send_data(){
  return 0;
}

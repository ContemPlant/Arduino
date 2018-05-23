// returns all data packets that were recorded since last successful sending operation in a single buffer
char* select_data_to_send(){
  char* buffer = (char*) calloc(currentWriteAddress, 1);
  char* write = buffer;
  char databyte;
  for (int i = 0; i < currentWriteAddress; ++i)
  {
    databyte = EEPROM[i];         //write one byte of data into databyte
    memcpy(write, &databyte, 1);  //copy into buffer
    write++;      //increment write pointer
  }
  return buffer;
}
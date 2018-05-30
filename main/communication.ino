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

void sendStructTo(uint16_t addr16, data* payload) {
  // Create a TX Request
  Tx16Request zbTx = Tx16Request(addr16, (uint8_t*) payload, sizeof(data));
  // Send your request
  xbee.send(zbTx);
}


// 1 = success, 0 = failure
int sending(){
  int num_packets = currentWriteAddressTempMem + ((currentWriteAddress - startWriteAddress) / sizeof(plant_info));
  data** buffer = (data**) select_data_to_send();
  for (int i = 0; i < num_packets; ++i)
  {
    Serial.print("sending packet number ");
    Serial.println(i);
    sendStructTo(PI_ADR, buffer[i]);
    delay(500);
  }
  return 1;
}

//TODO
char* recv_data(){
  char* buffer = (char*) malloc(sizeof(plant_info) + 1); //+1 for flags

  xbee.readPacket();
  Serial.println("test1");
  if (xbee.getResponse().isAvailable()) {
    Serial.println("test2");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      Serial.println("test3");
      xbee.getResponse().getRx16Response(rx16);
      for (int i = 0; i < rx16.getDataLength(); i++) { 
        Serial.print(rx16.getData(i)); 
        buffer[i] = rx16.getData(i);
      } 
    }
  }

  return buffer;
}

// update plant values
void receiving(){
  char* buffer = recv_data();
  // new plant request
  if (buffer[0] & NEW_PLANT_REQUEST){
    Serial.println("received new plant.");
    // only update plant if all data points of the old plant have been sent
    if (currentWriteAddress == startWriteAddress && currentWriteAddressTempMem == 0)
    {
      Serial.println("updating values.");
      memcpy(plant, &buffer[1], sizeof(plant_info));
      print_plant_info(plant);

      // save new plant to eeprom
      write_data(0, (char*) plant, sizeof(plant_info));
    }
    else
    {
      Serial.println("not all data points of old plant have been sent. doing nothing.");
    }
  }

  free(buffer);
}

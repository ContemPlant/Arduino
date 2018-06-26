msg* packMsg(data* payload){
  msg* msg_buffer = (msg*) calloc(sizeof(msg), 1);
  msg_buffer->flags = DATA;
  msg_buffer->sourceID = ARD_ADR;
  msg_buffer->timestamp = payload->timestamp;
  msg_buffer->compression = payload->comp;
  msg_buffer->temp = payload->temp;
  msg_buffer->hum = payload->hum;
  msg_buffer->rad = payload->rad;
  msg_buffer->loud = payload->loud;
  return msg_buffer;
}

boolean sendStructTo(uint16_t addr16, msg* payload) {
  // Create a TX Request
  Tx16Request zbTx = Tx16Request(addr16, (uint8_t*) payload, sizeof(msg));
  // Send your request
  xbee.send(zbTx);
  return true;
}


// 1 = success, 0 = failure
int sending(){
  return 1;
}

//TODO
char* recv_data(){
  char* buffer = (char*) calloc(sizeof(plant_info), 1); 

  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
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
  if (buffer[0] & SIGNIN){
    Serial.println("received new plant.");
    // only update plant if all data points of the old plant have been sent
    if (true){
      Serial.println("updating values.");
      memcpy(plant, buffer, sizeof(plant_info));
      print_plant_info(plant);

      // save new plant to eeprom
      write_eeprom(0, (char*) plant, sizeof(plant_info));

      // activate plant
      active = true;
      // switch lcd display
      setup_lcd();
    }
    else{
      Serial.println("not all data points of old plant have been sent. doing nothing.");
    }
  }
  else if (buffer[0] & SIGNOFF){
    Serial.println("Deactivated plant. Stop saving data.");
    deactivate_plant();
  }
  else{
    Serial.println("Received packet with unknown flag or received nothing this time");
  }

  free(buffer);
}

void send_queue() {
  tries = 0;
  
  //try to send packets in queue
  while (packetQueue->count && tries < MAX_TRIES) {
    msg* payload = packMsg(queue_peek(packetQueue));
    Serial.println("sending message.");

    if (sendStructTo(PI_ADR, payload)) {
      free(queue_pop(packetQueue));
      tries = 0;
    }
    else {
      tries++;
    }

    free(payload);
  }
}

int send_eeprom() {
  data* buffer = (data*) malloc(sizeof(data));
  tries = 0;
  uint8_t packet = 0;
  uint16_t index = 0;
  
  while(eepromNumPackets - packet && tries < MAX_TRIES) {

    index = eepromPacketSpace + (eepromOldestPacket - eepromPacketSpace + (packet * sizeof(data)) % (EEPROM.length() - eepromPacketSpace));
    EEPROM.get(index, buffer);
    msg* payload = packMsg(buffer);

    if (sendStructTo(PI_ADR, payload)) {
      free(payload);
      tries = 0;
      packet++;
    }
    else {
      tries++;      
    }
  }

  if  (tries == MAX_TRIES) {
    eepromOldestPacket = index;
    eepromNumPackets -= packet;
    free(buffer);
    return -1;
  }

  eepromOldestPacket = 0;
  eepromNumPackets = 0;
  free(buffer);
  return 1;
}

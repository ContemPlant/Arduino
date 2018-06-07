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
//  int num_packets = currentWriteAddressTempMem + ((currentWriteAddress - startWriteAddress) / sizeof(plant_info));
//  data** buffer = (data**) select_data_to_send();
//  for (int i = 0; i < num_packets; ++i)
//  {
//    Serial.print("sending packet number ");
//    Serial.println(i);
//    msg* payload = packMsg(buffer[i]);
//    Serial.println("Sending packagae:\n\n");
//    print_packet(buffer[i]);
//    print_msg(payload);
//    sendStructTo(PI_ADR, payload);
//    free(payload);
//    delay(500);
//  }
  return 1;
}

//TODO
char* recv_data(){
  char* buffer = (char*) calloc(sizeof(plant_info), 1); 

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
  if (buffer[0] & SIGNIN){
    Serial.println("received new plant.");
    // only update plant if all data points of the old plant have been sent
    if (currentWriteAddress == startWriteAddress && currentWriteAddressTempMem == 0)
    {
      Serial.println("updating values.");
      memcpy(plant, buffer, sizeof(plant_info));
      print_plant_info(plant);

      // save new plant to eeprom
      write_eeprom(0, (char*) plant, sizeof(plant_info));

      // activate plant
      active = true;
    }
    else
    {
      Serial.println("not all data points of old plant have been sent. doing nothing.");
    }
  }
  else if (buffer[0] & SIGNOFF)
  {
    Serial.println("Deactivated plant. Loading default plant. Stop saving data.");
    active = false;
    EEPROM[0] = SIGNOFF;
    load_default_plant();
  }
  else
  {
    Serial.println("Received packet with unknown flag or received nothing this time");
  }

  free(buffer);
}

void send_queue() {
  //try to send packets in queue
  while (packetQueue->count && tries < MAX_TRIES) {
      msg* payload = packMsg(queue_peek(packetQueue));
      Serial.println("sending message.");

      if (sendStructTo(PI_ADR, payload)) {
        free(queue_pop(packetQueue));
        free(payload);
        tries = 0;
      }
      else {
        tries++;
      }
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

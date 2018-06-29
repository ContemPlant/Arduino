// write size bytes from buffer to index
void write_eeprom(int index, char* buffer, int size) {
  for (int i = 0; i < size; ++i)
  {
    EEPROM[index + i] = buffer[i];
  }
}

// read size bytes from eeprom at index and write the content into buffer
void read_eeprom(int index, char* buffer, int size) {
  for (int i = 0; i < size; ++i)
  {
    buffer[i] = EEPROM[index + i];
  }
}

// merges two packets a,b by building the average c. a should be recorded prior to b, also works if a and c ar identical
void merge_packets(data* a, data* b, data* c) {
  Serial.println("merging packets...");
  if (a->timestamp < b->timestamp)
    c->timestamp = a->timestamp;
  c->temp = (a->comp * a->temp + b->comp * b->temp) / (a->comp + b->comp);
  c->hum = (a->comp * a->hum + b->comp * b->hum) / (a->comp + b->comp);
  c->rad = (a->comp * a->rad + b->comp * b->rad) / (a->comp + b->comp);
  c->loud = (a->comp * a->loud + b->comp * b->loud) / (a->comp + b->comp);
  c->comp = a->comp + b->comp;
}


int packet_to_eeprom(data* packet) {
  if (eepromMaxPackets <= eepromNumPackets || !packet) {
    return -1;
  }
  uint16_t index = eepromPacketSpace + (eepromOldestPacket - eepromPacketSpace + (eepromNumPackets * sizeof(data)) % (EEPROM.length() - eepromPacketSpace));
  EEPROM.put(index, *packet);
  EEPROM[index + sizeof(data)] = '\0';
  eepromNumPackets++;

  return 1;
}

uint8_t count_packets() {
  uint16_t index = eepromOldestPacket;
  uint8_t numPackets;

  while (EEPROM[index + sizeof(data)] != '\0') {
    numPackets++;
    index = eepromPacketSpace + (eepromOldestPacket - eepromPacketSpace + (numPackets * sizeof(data)) % (EEPROM.length() - eepromPacketSpace));
  }
  return numPackets;
}


// write size bytes from buffer to index
void write_data(int index, char* buffer, int size){
	for (int i = 0; i < size; ++i)
	{
		EEPROM[index+i] = buffer[i];
	}
}

// merges two packets a,b by building the average c. a should be recorded prior to b
void merge_packets(data* a, data* b, data* c){
	c->time = a->time;
	c->comp = a->comp + b->comp;
	c->temp = (a->comp * a->temp + b->comp * b->temp) / (a->comp + b->comp);
	c->hum = (a->comp * a->hum + b->comp * b->hum) / (a->comp + b->comp);
	c->rad = (a->comp * a->rad + b->comp * b->rad) / (a->comp + b->comp);
	c->loud = (a->comp * a->loud + b->comp * b->loud) / (a->comp + b->comp);
}

// compresses memory if full and updates currentWriteAddress
int compress_memory(){
	// check if memory is full
	if (currentWriteAddress + sizeof(data) < MEMORY_SIZE){
		return -1;
	}

	data a, b, c;
	int j = 0;
	// merge pairs of neighbouring packets
	for (int i = 0; i < MEMORY_SIZE/2; i += 2 * sizeof(data), j += sizeof(data))
	{
		EEPROM.get(i, a);	//read packet a
		EEPROM.get(i + sizeof(data), b);	//read neighbouring packet b
		merge_packets(&a, &b, &c);	//combine into c
		EEPROM.put(j, c);	//write back into memory
	}

	currentWriteAddress = j;
	maxCompressionLevel *= 2;	//double maximum compression
}

// stores new data packet at the right position
void store_packet(data* packet){
	if (currentCompressionLevel = 0){	//first packet to be saved at currentWriteAddress
		write_data(currentWriteAddress, (char*) packet, sizeof(data));
	}
	else{	//there are already packets stored and the new one needs to be merged with the older ones
		data a, c;
		merge_packets(&a, packet, &c);
		EEPROM.put(currentWriteAddress, c);
	}

	// increment currentCompressionLevel after write operation
	currentCompressionLevel++;

	if (currentCompressionLevel = maxCompressionLevel){
		currentWriteAddress += sizeof(data);
	}

	compress_memory();
}

// write from temporary to permanent memory (EEPROM) and reset currentWriteAddressTempMem
void write_temp_to_perm(){
	for (int i = 0; i < TEMP_MEMORY_SIZE; ++i)
	{
		store_packet(temp_mem[i]);
		free(temp_mem[i]);
	}

	currentWriteAddressTempMem = 0;
}

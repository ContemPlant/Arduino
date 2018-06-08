
float temperature() {
  return dht.readTemperature();
}

float humidity() {
  return dht.readHumidity();
}

float radiation() {
  return radsens.ReadVisible();
}

float loudness(){
  return analogRead(LOUDNESS);
}

void fill_in_sensor_data(data* new_data) {
  // fill struct fields
  Serial.println("getting sensor data...");
  new_data->timestamp = timestamp();
  new_data->comp = 1;
  //new_data->temp = temperature() * 10; //save a decimal place by multiplying by 10
  new_data->temp = temperature();
  new_data->hum = humidity();
  new_data->rad = radiation();
  new_data->loud = loudness();
}


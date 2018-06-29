float temperature() {
  for (int i = 0; i < 5; i++) {
    float temp = dht.readTemperature();
    if (validData('t', temp)) return temp;
  }
  return 0.0;//return something schlau
}

float humidity() {
  for (int i = 0; i < 5; i++) {
    float hum = dht.readHumidity();
    if (validData('h', hum)) return hum;
  }
  return 0.0;//s. temp
}

float radiation() {
  for (int i = 0; i < 5; i++) {
    float rad = radsens.ReadVisible();
    if (validData('r', rad)) return rad;
  }
  return 0.0;//s. temp
}

float loudness() {
  for (int i = 0; i < 5; i++) {
    float loud = analogRead(LOUDNESS);
    if (validData('l', loud)) return loud;
  }
  return 0.0;//s. temp
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

boolean validData(char sensor, float data) {
  switch (sensor) {
    case 't':
      if (data < -40.0 || data > 80.0) return false;
      return true;
    case 'h':
      if (data < 0.0 || data > 100.0) return false;
      return true;
    case 'r':
      if (data < 0.0) return false;
      return true;
    case 'l':
      if (data < 0.0) return false;
      return true;
  }
}



float temperature() {
  return dht.readTemperature();
}

float humidity() {
  return dht.readHumidity();
}

float radiation() {
  return radsens.ReadVisible();
}

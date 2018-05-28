float env_factor (float temp, float hum, float light) {
  float diff_temp = abs(temp - dht.readTemperature());
  float diff_hum = abs(hum - dht.readHumidity());
  float diff_light = abs(light - radsens.ReadVisible());

  return (plant->temp_weight * fnct(10.0, diff_temp) + plant->hum_weight * fnct(20.0, diff_hum) + 
          plant->rad_weight * fnct(100.0, diff_light));
}

float fnct (float maxi, float deviation) {
  if (maxi < deviation) return 0;
  
  return ((2/(pow(maxi,3))) * (pow(deviation,3))) - ((3/(pow(maxi,2)))*(pow(deviation,2))) + 1;
}

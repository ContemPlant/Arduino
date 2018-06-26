uint32_t get_time(){
  return loopno;
}

void print_packet(data* p){
  Serial.print("#########\n");
  Serial.print("time: ");
  Serial.print(p->timestamp);
  Serial.print("\ncomp lvl: ");
  Serial.print(p->comp);
  Serial.print("\ntemp: ");
  Serial.print(p->temp);
  Serial.print("\nhum: ");
  Serial.print(p->hum);
  Serial.print("\nrad: ");
  Serial.print(p->rad);
  Serial.print("\nloud: ");
  Serial.print(p->loud);
  Serial.print("\n#########\n");
}

void print_plant_info(plant_info* p){
  Serial.print("#########\n");
  Serial.print("flags: ");
  Serial.print(p->flags);
  Serial.print("\ntemp opt: ");
  Serial.print(p->temp_opt);
  Serial.print("\ntemp weight: ");
  Serial.print(p->temp_weight);
  Serial.print("\nhum opt: ");
  Serial.print(p->hum_opt);
  Serial.print("\nhum weight: ");
  Serial.print(p->hum_weight);
  Serial.print("\nrad opt: ");
  Serial.print(p->rad_opt);
  Serial.print("\nrad weight: ");
  Serial.print(p->rad_weight);
  Serial.print("\nloud opt: ");
  Serial.print(p->loud_opt);
  Serial.print("\nloud weight: ");
  Serial.print(p->loud_weight);
  Serial.print("\n#########\n"); 
}

void print_msg(msg* payload) {
  Serial.print("\nFlags: ");
  Serial.print(payload->flags);
  Serial.print("\nSource id: ");
  Serial.print(payload->sourceID);
  Serial.print("\ntimestamp: ");
  Serial.print(payload->timestamp);
  Serial.print("\ncompression: ");
  Serial.print(payload->compression);
  Serial.print("\nTemp: ");
  Serial.print(payload->temp);
  Serial.print("\nradt: ");
  Serial.print(payload->temp);
  Serial.print("\nHumidity: ");
  Serial.print(payload->hum);
  Serial.print("\nRadiation: ");
  Serial.print(payload->rad);
  Serial.print("\nLoud: ");
  Serial.print(payload->flags);
  Serial.print("\n#########\n"); 
}


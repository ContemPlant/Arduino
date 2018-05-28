uint32_t get_time(){
  return loopno;
}

void print_on_lcd() {
  LCD.CharGotoXY(60,0);
  LCD.print(plant->id);
  LCD.CharGotoXY(60,8);
  LCD.print(temperature());
  LCD.CharGotoXY(60,16);
  LCD.print(humidity());
  LCD.CharGotoXY(60,24);
  LCD.print(radiation());
  LCD.CharGotoXY(60,32);
  LCD.print(analogRead(LOUDNESS));
  LCD.CharGotoXY(60,40);
  LCD.print(env_factor(plant->temp_opt, plant->hum_opt, plant->rad_opt) * 100);
  LCD.CharGotoXY(35,48);
  LCD.print(loopno);
  LCD.CharGotoXY(55,56);
  LCD.print("   ");  //clear number
  //print amount of packets that are currently in temp memory
  LCD.CharGotoXY(55,56);
  LCD.print(currentWriteAddressTempMem);
  LCD.CharGotoXY(90,56);
  LCD.print("    ");  //clear number
  LCD.CharGotoXY(90,56);
  //print amount of packets that are currently in eeprom
  LCD.print((currentWriteAddress - sizeof(plant_info)) / sizeof(data));
}

void print_packet(data* p){
  Serial.print("#########\n");
  Serial.print("time: ");
  Serial.print(p->time);
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
  Serial.print("id: ");
  Serial.print(p->id);
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
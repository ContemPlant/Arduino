
void print_on_lcd() {
  LCD.CharGotoXY(60,0);
  //LCD.print(plant->id);
  LCD.print(plant->temp_opt);
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

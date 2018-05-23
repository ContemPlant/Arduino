
void print_on_lcd() {
  
  LCD.CharGotoXY(80,0);
  LCD.print(temperature());
  LCD.CharGotoXY(80,16);
  LCD.print(humidity());
  LCD.CharGotoXY(80,32);
  LCD.print((int) radiation());
  LCD.CharGotoXY(80,48);
  //LCD.print(analogRead(LOUDNESS));
  LCD.print(env_factor(temp_deg, humidity_proz, sunlight_lumen) * 100);

}

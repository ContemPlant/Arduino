void lcd_print_sensors() {
  LCD.CharGotoXY(60,0);
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
 
}

void lcd_setup_sensors(){
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    
  //6*8 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 

  //Set the start coordinate.
  LCD.CharGotoXY(0,0);
  LCD.println("temp_opt: ");
  LCD.println("Temp: ");
  LCD.println("Hum: ");
  LCD.println("Rad: ");
  LCD.println("Loud: ");
  LCD.println("Env: ");

  // print units
  LCD.CharGotoXY(100,8);
  LCD.print("C");
  LCD.CharGotoXY(100,16);
  LCD.print("%");
  LCD.CharGotoXY(100,24);
  LCD.print("lm");
  LCD.CharGotoXY(100,32);
  LCD.print("db");
  LCD.CharGotoXY(100,40);
  LCD.print("%");
}

void lcd_print_loop(){
  LCD.CharGotoXY(35,56);
  LCD.print(loopno);
}

void lcd_setup_loop(){
  LCD.CharGotoXY(0,56);
  LCD.println("loop: ");
}

void lcd_print_id(){
  LCD.CharGotoXY(16,32);
  char str[4];
  sprintf(str, "%04d", ARD_ADR);
  LCD.println(str); 
}

void lcd_setup_id(){
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
      
  //6*8 font size, auto new line, black character on white back ground.
  LCD.FontModeConf(Font_16x32, FM_ANL_AAA, BLACK_BAC);   

  LCD.CharGotoXY(8,0);
  LCD.println("ArduID:");
}

void lcd_print_clock(){
  LCD.CharGotoXY(94,56);
  char str[5];
  sprintf(str, "%02d:%02d", clock.hour, clock.minute);
  LCD.println(str); 
}

void lcd_setup_clock(){
  LCD.CharGotoXY(64,56);
  LCD.println("time:");
}

void lcd_setup_and_print_qr(const char* qr, int len){
  for (int i = 0; i < len; i++){
    Serial.print(pgm_read_byte_near(qr + i));
  }
  LCD.CleanAll(WHITE);    //Clean the screen with black or white.
  int vert_size = sqrt(len);
  for(int i = 0; i < len; i++)
  {
    
    if (pgm_read_byte_near(qr + i) == '1') {
      // draw 2x2 rectangle
      LCD.DrawRectangleAt(2 * (i % vert_size), 2 * (i / vert_size), 2, 2, BLACK_FILL);
    }
  }
  
}

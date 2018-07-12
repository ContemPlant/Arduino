void deactivate_plant() {
  EEPROM[0] = SIGNOFF;
  active = false;
  lcd_setup_and_print_qr(qrcode, strlen_P(qrcode));
  print_X_on_oled();
  send_signoff();
}

void read_button() {
  bool unload = unload_button_pressed();
  if (unload) {
    Serial.println("let go of button. deactivating plant");
    deactivate_plant();
  }
}

bool unload_button_pressed() {
  return !digitalRead(BUTTON);
}


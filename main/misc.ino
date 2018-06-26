void deactivate_plant(){
	EEPROM[0] = SIGNOFF;
	active = false;
	lcd_setup_and_print_qr(qrcode, strlen_P(qrcode));
  send_signoff();
}

void read_button(){
	if (digitalRead(BUTTON)){
		Serial.println("pressed button. deactivating plant");
		deactivate_plant();
    }
}

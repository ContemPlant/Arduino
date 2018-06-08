#define BUTTON 5

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
   Serial.begin(9600);
    // initialize the pushbutton pin as an input:
    pinMode(BUTTON, INPUT);
}

void loop(){
    // read the state of the pushbutton value:
//    buttonState = digitalRead(BUTTON);
//
//    // check if the pushbutton is pressed.
//    // if it is, the buttonState is HIGH:
//    if (digitalRead(BUTTON)) {
//      Serial.println(digitalRead(BUTTON));
//    }
//    else {
//        // turn LED off:
//      Serial.println(0);
//    }
    Serial.println(digitalRead(BUTTON));
    //delay(10);
}

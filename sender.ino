#include <SoftwareSerial.h>

SoftwareSerial HC12(2, 3); 

const int button1 = 6;
const int button2 = 5;
bool state1 = false;
bool state2 = false;
bool lastButton1State = HIGH;
bool lastButton2State = HIGH;

void setup() {
    pinMode(button1, INPUT_PULLUP); 
    pinMode(button2, INPUT_PULLUP);
    
    Serial.begin(9600);  
    HC12.begin(9600);    
    Serial.println("Sender ready. Type a message to send!");
}

void loop() {
    int sensorValue = analogRead(A0);

    HC12.print("A0: ");
    HC12.println(sensorValue);
    
    Serial.print("Sent A0: ");
    Serial.println(sensorValue);

    // Read buttons
    bool button1State = digitalRead(button1);
    bool button2State = digitalRead(button2);

    // Toggle state1 
    if (button1State == LOW && lastButton1State == HIGH) {
        state1 = !state1;
        HC12.print("Btn1: ");
        HC12.println(state1);
        Serial.print("Sent Btn1: ");
        Serial.println(state1);
        delay(200);  
    }

    // Toggle state2 
    if (button2State == LOW && lastButton2State == HIGH) {
        state2 = !state2;
        HC12.print("Btn2: ");
        HC12.println(state2);
        Serial.print("Sent Btn2: ");
        Serial.println(state2);
        delay(200);  
    }

    lastButton1State = button1State;
    lastButton2State = button2State;

    // Read user input from Serial Monitor and send via HC-12
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n'); 
        HC12.print("Msg: ");
        HC12.println(input); 
        Serial.print("Sent Msg: ");
        Serial.println(input); 
    }

    delay(500); 
}

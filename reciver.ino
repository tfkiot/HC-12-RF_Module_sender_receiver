#include <SoftwareSerial.h>

SoftwareSerial HC12(2, 3);  

const int led1 = A0;  
const int led2 = A5; 
const int led3 = 5;

const int signalLed = 12;

bool state1 = false;
bool state2 = false;
int brightness = 0;

const unsigned int MAX_MESSAGE_LENGTH = 32;
char receivedChars[MAX_MESSAGE_LENGTH];
bool newData = false;
unsigned long lastReceiveTime = 0; 
const unsigned long SIGNAL_TIMEOUT = 2000; 

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(signalLed, OUTPUT);
  
  digitalWrite(led1, LOW);  
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(signalLed, LOW);

  Serial.begin(9600);
  HC12.begin(9600);
  Serial.println("Receiver ready. Waiting for data...");
}

void loop() {
  receiveData();
  processData();
  checkSignalStatus();
}

void receiveData() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (HC12.available() > 0 && !newData) {
    rc = HC12.read();

    if (rc != endMarker) {
      if (ndx < MAX_MESSAGE_LENGTH - 1) {
        receivedChars[ndx++] = rc;
      }
    } else {
      receivedChars[ndx] = '\0';  
      ndx = 0;
      newData = true;
      lastReceiveTime = millis(); 
    }
  }
}

void processData() {
  if (newData) {
    Serial.print("Received: ");
    Serial.println(receivedChars);
    digitalWrite(signalLed, HIGH);

    if (strncmp(receivedChars, "A0: ", 4) == 0) {
      brightness = atoi(receivedChars + 4);
      brightness = constrain(brightness, 0, 1023);
      brightness = map(brightness, 0, 1023, 0, 255);
      analogWrite(led3, brightness);
      Serial.print("Brightness set to: ");
      Serial.println(brightness);

    } else if (strncmp(receivedChars, "Btn1: ", 6) == 0) {
      state1 = atoi(receivedChars + 6);
      digitalWrite(led1, state1);
      Serial.print("LED1 toggled to: ");
      Serial.println(state1);

    } else if (strncmp(receivedChars, "Btn2: ", 6) == 0) {
      state2 = atoi(receivedChars + 6);
      digitalWrite(led2, state2);
      Serial.print("LED2 toggled to: ");
      Serial.println(state2);
    } else {
      Serial.println("Unknown command");
    }

    newData = false;
  }
}

void checkSignalStatus() {
  if (millis() - lastReceiveTime > SIGNAL_TIMEOUT) {
    digitalWrite(signalLed, LOW); // Signal lost
  }
}

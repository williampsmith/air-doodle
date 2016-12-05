#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(2);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void requestEvent() {
  Wire.write("hello");
}

void receiveEvent() {
  char c = Wire.read();
  Serial.print(c);
}

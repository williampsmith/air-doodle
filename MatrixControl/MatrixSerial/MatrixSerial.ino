// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

#define numBytes 66
#define dimX 24
#define dimY 21

void setup() {
  matrix.begin();
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()) {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    byte x = 0;
    byte y = 0;
    byte bitmap[numBytes]; // = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    byte i = 0;

    while (Serial.available() > 0) {
        bitmap[i] = Serial.read();
        i++;
        delay(1);
    }
    
    matrix.drawBitmap(4, 5, bitmap, dimX, dimY, matrix.Color333(0,1,0));
    
//    byte buff1[numData+4];
//    for (int j = 0; j < numData; j++) {
//      buff1[j] = buff[i];
//        Serial.write(byte(buff[j]));
//    }
    
//    for (int j = 0; j < numBytes; j++) {
//      byte z = 128;
//      for (byte k = 0; k < 8; k++) {
//        if (bitmap[j] & z) { // if kth bit is 1
//          matrix.drawPixel(x, y, matrix.Color333(7, 7, 7));
//        }
//        z /= 2;
//        x += 1;
//      }
//      if (x >= 32) {
//        x = 0;
//        y += 1;
//      }
//    }
  }
}

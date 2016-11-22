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

void setup() {

  matrix.begin();
//  matrix.print("A");
  // draw a pixel in solid white
  // matrix.drawPixel(16, 16, matrix.Color333(7, 7, 7)); 
//  delay(500);
//
//  // fix the screen with green
//  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 7, 0));
//  delay(500);
//
//  // draw a box in yellow
//  matrix.drawRect(0, 0, 32, 16, matrix.Color333(7, 7, 0));
//  delay(500);
//  
//  // draw an 'X' in red
//  matrix.drawLine(0, 0, 31, 15, matrix.Color333(7, 0, 0));
//  matrix.drawLine(31, 0, 0, 15, matrix.Color333(7, 0, 0));
//  delay(500);
//  
//  // draw a blue circle
//  matrix.drawCircle(7, 7, 7, matrix.Color333(0, 0, 7));
//  delay(500);
//  
//  // fill a violet circle
//  matrix.fillCircle(23, 7, 7, matrix.Color333(7, 0, 7));
//  delay(500);
//  
//  // fill the screen with 'black'
//  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
//  // draw some text!
//  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
//  matrix.setTextSize(1);    // size 1 == 8 pixels high
//  
  // print each letter with a rainbow color
//  matrix.setTextColor(matrix.Color333(0,0,7));
//  matrix.print('W');
//  matrix.print('I');
//  matrix.print('L');
//  matrix.print('L'); 
//  matrix.setCursor(1, 9); 
//  matrix.print('I');
//  matrix.print('A');
//  matrix.print('M');
  
    // next line
//  matrix.setTextColor(matrix.Color333(0,7,7)); 
//  matrix.print('*');
//  matrix.setTextColor(matrix.Color333(0,4,7)); 
//  matrix.print('R');
//  matrix.setTextColor(matrix.Color333(0,0,7));
//  matrix.print('G');
//  matrix.setTextColor(matrix.Color333(4,0,7)); 
//  matrix.print("B");
//  matrix.setTextColor(matrix.Color333(7,0,4)); 
//  matrix.print("*");

  // whew!
}

void loop() {
  // do nothing
}

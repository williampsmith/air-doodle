#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int buttonPin = 2;

void setup(void) 
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  while(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.println("No BNO055 detected!");
    delay(1000);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

void loop(void) 
{
  /* Get a new sensor event */
  while (digitalRead(buttonPin)) {
    sensors_event_t event;
    bno.getEvent(&event);
    
    /* Display the floating point data */
    //Serial.print("GX: ");
    Serial.print(event.orientation.x, 4);
    Serial.print(",");
    //Serial.print("\tGY: ");
    Serial.print(event.orientation.y, 4);
    Serial.print(",");
    //Serial.print("\tGZ: ");
    Serial.print(event.orientation.z, 4);
    Serial.print(",");
    //Serial.println("");
  
    //Serial.print("AX: ");
    Serial.print(event.acceleration.x, 4);
    Serial.print(",");
    //Serial.print("\tAY: ");
    Serial.print(event.acceleration.y, 4);
    Serial.print(",");
    //Serial.print("\tAZ: ");
    Serial.print(event.acceleration.z, 4);
    //Serial.println("");
    
    Serial.println("");
    
    delay(100);
  }
}

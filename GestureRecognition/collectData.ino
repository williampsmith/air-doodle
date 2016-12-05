#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int buttonPin = 2;
const int NUM_OF_SAMPLES = 20;
int buttonState = 0;
bool finishedSampling = false;

const char sampleLetters[] = {'0', '8', '9'};
int remainingSamples = NUM_OF_SAMPLES + 1;
int sampleIndex = 0;
 
void setup(void) 
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  Serial.println("Beginning output of accel and orientation. Note that gravity has been removed from samples.");
  Serial.println("Format: x_accel, y_accel, z_accel, x_or, y_or, z_or.");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  delay(2000);
    
  bno.setExtCrystalUse(true);
}
 
void loop(void) 
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    /* Get a new sensor event */ 
    sensors_event_t event; 
    bno.getEvent(&event);
    
    // display x,y,z orientation
    Serial.print(event.orientation.x, 4);
    Serial.print(",");
    Serial.print(event.orientation.y, 4);
    Serial.print(",");
    Serial.print(event.orientation.z, 4);
    Serial.print(",");
    
    // display x,y,z acceleration
    Serial.print((bno.getVector(bno.VECTOR_LINEARACCEL))[0]);
    Serial.print(",");
    Serial.print((bno.getVector(bno.VECTOR_LINEARACCEL))[1]);
    Serial.print(",");
    Serial.print((bno.getVector(bno.VECTOR_LINEARACCEL))[2]);
    Serial.println("");
      
    delay(50);
  }
  else {
    remainingSamples--;

    if (remainingSamples <= 0) {
      remainingSamples = NUM_OF_SAMPLES;

      if (sampleIndex < 3) {
        sampleIndex++;
      }
      else {
        Serial.println("\n\n------------- FINISHED TRAINING ----------------\n\n");
        finishedSampling = true;
      }
    }
    
    prepareForNextSample();
  }
}

void prepareForNextSample() {
  buttonState = digitalRead(buttonPin);

  if (!finishedSampling) {
    Serial.print("\n\nPrepare for sample letter:\t");
    Serial.print(sampleLetters[sampleIndex]);
    Serial.print("\tnumber:\t");
    Serial.println(remainingSamples);
    Serial.println("\n\n");
  }

  while(buttonState == LOW) {
    delay(3000);
    buttonState = digitalRead(buttonPin);
  }

  // for parsing for the automation script
    Serial.print("%");
    Serial.println(sampleLetters[sampleIndex]);
}


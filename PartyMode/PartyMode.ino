/* This Sketch Requires the GY-61 module and an Arduino Mega
 * 
 * Reading X, Y, Z axis values
 */

#include <Streaming.h>

#include "ADXL335.h"

int rLED = 49;
int yLED = 50;
int gLED = 51;
int bLED = 52;
int wLED = 53;

float prevX, prevY, prevZ;
int lightDelay = 45;
ADXL335 accelerometer;

void setup()
{
  Serial.begin(9600);
  accelerometer.begin();
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
  pinMode(wLED, OUTPUT);
}

void lightShow() {
    digitalWrite(rLED, HIGH);
    delay(lightDelay);
    digitalWrite(yLED, HIGH);
    delay(lightDelay);
    digitalWrite(gLED, HIGH);
    delay(lightDelay);
    digitalWrite(bLED, HIGH);
    delay(lightDelay);
    digitalWrite(wLED, HIGH);
    delay(lightDelay);
    digitalWrite(rLED, LOW);
    delay(lightDelay);
    digitalWrite(yLED, LOW);
    delay(lightDelay);
    digitalWrite(gLED, LOW);
    delay(lightDelay);
    digitalWrite(bLED, LOW);
    delay(lightDelay);
    digitalWrite(wLED, LOW);
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop()
{
  float ax,ay,az;
  accelerometer.getAcceleration(&ax,&ay,&az);
 
  Serial.println("accleration of X/Y/Z: ");
  Serial.print(ax);
  Serial.println(" g");
  Serial.print(ay);
  Serial.println(" g");
  Serial.print(az);
  Serial.println(" g");
  if ( abs(ax-prevX) > 1 || abs(ay-prevY) > 1 || abs(az-prevZ) > 1 ) {
    lightShow();
  }
   prevX = ax;
   prevY = ay;
   prevZ = az;
   
   delay(300);
}

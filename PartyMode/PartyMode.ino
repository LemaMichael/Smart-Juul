
/* This Sketch Requires the GY-61 module and a Wifi Lora 32
 * 
 * Reading X, Y, Z axis values
 */
#include "Arduino.h"
#include "heltec.h"
#include <WiFi.h>
#include <Streaming.h>
#include "ADXL335.h"
#include "images.h"

int rLED = 27;
int yLED = 17;
int gLED = 25;
int bLED = 2;
int wLED = 23;

float prevX, prevY, prevZ;
int lightDelay = 30;
ADXL335 accelerometer;

/*******************************************************************
 * OLED Arguments
 */
#define OLED_UPDATE_INTERVAL 500        

/**********************************************  WIFI Client *********************************
 * wifi client
 */
const char* ssid = "XXXX"; 
const char* password = "XXXX"; 

/********************************************************************
 * setup oled
 */
void setupOLED()
{
  pinMode(RST_OLED, OUTPUT);
  //OLED
  digitalWrite(RST_OLED, LOW);        // turn D16 low to reset OLED
  delay(50);
  digitalWrite(RST_OLED, HIGH);       // while OLED is running, must set D16 in high
  
  Heltec.display->init();
  Heltec.display->flipScreenVertically();           //倒过来显示内容
  Heltec.display->setFont(ArialMT_Plain_10);        //设置字体大小
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);//设置字体对齐方式

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Initialize...");
}

/*********************************************************************
 * setup wifi
 */
 void drawWifiImage() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    Heltec.display->drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void setupWIFI()
{
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connecting...");
  Heltec.display->drawString(0, 10, String(ssid));
  Heltec.display->display();
  

  WiFi.disconnect(true);
  delay(1000);
  
  WiFi.mode(WIFI_STA);  
  WiFi.setAutoConnect(true);      
  WiFi.setAutoReconnect(true);    
  WiFi.begin(ssid, password);

  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 10)
  {
    count ++;
    delay(500);
    Serial.print(".");
  }

  Heltec.display->clear();
  if(WiFi.status() == WL_CONNECTED) 
  {
    Heltec.display->drawString(0, 0, "Connecting...OK."); 
    Serial.println("Done!");
    // clear the display
    Heltec.display->clear();
    drawWifiImage();
    Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
    Heltec.display->drawString(10, 128, String(millis()));
    // write the buffer to the display
    Heltec.display->display();
  }  
  else {
    Heltec.display->drawString(0, 0, "Connecting...Failed");
  }
  Heltec.display->display();
}


void setup()
{
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Enable*/);
  Serial.println("Initialize...");

  Serial.begin(115200);
  accelerometer.begin();
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
  pinMode(wLED, OUTPUT);
  setupOLED();
  setupWIFI();
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
 /*
  Serial.println("accleration of X/Y/Z: ");
  Serial.print(ax);
  Serial.println(" g");
  Serial.print(ay);
  Serial.println(" g");
  Serial.print(az);
  Serial.println(" g");
  */
  
  if ( abs(ay-prevY) >= 4.0 || abs(az-prevZ) >= 4.0 ) {
    //Serial <<  "x: " << ax  << " "<< prevX <<  " " << abs(ax-prevX) << " y:" << abs(ay-prevY) << " z: " << abs(az-prevZ) << endl;
    lightShow();
    delay(300);
  }
   prevX = ax;
   prevY = ay;
   prevZ = az;
   
}

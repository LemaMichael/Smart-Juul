
/* This Sketch Requires the GY-61 module and a Wifi Lora 32
 * 
 * Reading X, Y, Z axis values
 */
#include "Arduino.h"
#include "heltec.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
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
const char* ssid = "xxx"; 
const char* password = "xxx"; 
WebServer server(80);

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
  Heltec.display->flipScreenVertically();           
  Heltec.display->setFont(ArialMT_Plain_10);        
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Initialize...");
}

/*********************************************************************
 * wifi image
 */
 void drawWifiImage() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    Heltec.display->drawXbm(34, 25, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

/*********************************************************************
 * Client
 */
 void handleRoot() {
  server.send(200, "text/plain", "hello from smart Juul!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupClient() 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if(WiFi.status() == WL_CONNECTED) 
  {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Connecting to " + String(ssid)); 
    Heltec.display->drawString(0, 10, "IP address:" + WiFi.localIP().toString()); 
    drawWifiImage();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString(20, 128, String(millis()));
  }  
  else {
    Heltec.display->drawString(0, 0, "Connecting...Failed");
  }
  Heltec.display->display();
  

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
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
  setupClient();
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
  server.handleClient();
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

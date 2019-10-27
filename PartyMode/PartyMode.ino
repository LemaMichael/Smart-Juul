
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
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "" // Input AIO Credentials here:
#define AIO_KEY         ""

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);       
Adafruit_MQTT_Subscribe JUUL_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/JuulSwitch", MQTT_QOS_1);

const int activateLED = 0;
const int rLED = 27;
const int yLED = 17;
const int gLED = 25;
const int bLED = 2;
const int wLED = 23;

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
const char* ssid = ""; 
const char* password = ""; 
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

void handlePod(){
  Serial.println("hear you!!!");
  digitalWrite(activateLED, HIGH);
  server.send(200, "text/plain", "activated your smart juul!");
  //delay(2500);
  delay(15000);
  digitalWrite(activateLED, LOW);
  Serial.println("ENDED!!!");
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

  server.on("/activate", handlePod);

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
  pinMode(activateLED, OUTPUT);
  digitalWrite(activateLED, LOW);
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
  pinMode(wLED, OUTPUT);
  setupOLED();
  setupClient();
  mqtt.subscribe(&JUUL_Control);
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

//https://learn.adafruit.com/mqtt-adafruit-io-and-you/more-on-subscriptions
void AdaSubscription() {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    // Check if its the JUUL_Control  feed
    if (subscription == &JUUL_Control) {
      Serial.print(F("JUUL_Control: "));
      Serial.println((char *)JUUL_Control.lastread);

      //https://io.adafruit.com/eeUser/dashboards/juulswitch
      if (strcmp((char *)JUUL_Control.lastread, "ON") == 0) {
        handlePod();
      }
      if (strcmp((char *)JUUL_Control.lastread, "OFF") == 0) {
        handlePod();
      }
    }
}
}


void loop()
{
  //MQTT_connect();
  //AdaSubscription();
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

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

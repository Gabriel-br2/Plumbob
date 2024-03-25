/*
 * Project: Exemple of IOT device
 * Description: Trab AIC
      A exemple of IOT device with the following features:
      wifi config, wifi reset, button for reset, OTA, Serial->wifi
      
 * Creation date: 30/07/2023
 * Author: Gabriel, GRS
 * GitHub: Gabriel-br2
 * License: GPL-3.0
 */

#include <WiFiManager.h> 
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#define BUTTON_PINr 5
#define LED_BUTTONr 16 
#define RED_PIN 22
#define GREEN_PIN 3 
#define BLUE_PIN 21

struct RGB{
  int red;
  int green;
  int blue;
};

// configs
const int n = 6;
int count_buttonR_pressed = 0;

//Names for Project
String Proj_Name = "Plumboob";
String Autores =   "GRS";
String Version =   "0";
String data =      "13:09:2023";
String host =      "plumboob";
String User =      "rafoguete";
String Password =  "canteiro";

// variable to handle function
bool res;
float timeVec[n];
bool last_buttonR_state = true;
String hexa = "000000";

//main object
WiFiManager wm;
AsyncWebServer server(80);
RGB rgb;

//Html template
extern String loginPage;
extern String loginPageError;
extern String mainPage_template;

//function
RGB hexToRGB(const String& hexCode);
void checkButton(float time);
void startServer();

void setup() {
  Serial.begin(115200);    
  
  pinMode(BUTTON_PINr, INPUT_PULLUP);
  pinMode(LED_BUTTONr, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(LED_BUTTONr, HIGH);
  //wm.resetSettings();

  wm.setClass("invert");
  res = wm.autoConnect("Plumb_WifiConfig");

  if(!res){
    Serial.println("Failed to connect, reseting");
    ESP.restart();
  }
  
  Serial.println("Connected to wifi");
  delay(150);

  //connect to wi-fi
  WiFi.begin();
  unsigned long begin_Time = millis();
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    if ((millis() - begin_Time) > 30000){
      Serial.println("Fail to connect, reseting");
      wm.resetSettings();
      ESP.restart();
    }
  }

  //hold for ip adress
  Serial.println("hold for IP");
  IPAddress localIP = WiFi.localIP();
  while (localIP == IPAddress(0,0,0,0)){
    Serial.print(".");
    localIP = WiFi.localIP();
    delay(150);
  }
  Serial.print("\nConectado IP: ");
  Serial.println(localIP);

  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    unsigned long begin_Time2 = millis();
    while (1){
      if (!MDNS.begin(host)){
        if ((millis() - begin_Time2) > 30000){
          Serial.println("Fail to inicialize mDNS. Passing");
          break;
        }
      }
      else{
        Serial.println("mDNS responder started");
        break;
      }
    }
  }
  else{
    Serial.println("mDNS responder started");
  }
  
  startServer();
  digitalWrite(LED_BUTTONr, LOW);
}

void loop() {
  // reconect wi-fi in case of disconnect 
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("lose connect, trying reconect");
    WiFi.begin();
    while(WiFi.status() != WL_CONNECTED){
      checkButton(15);
      Serial.print(".");
      delay(500);
    }
    Serial.println("\nReconnected");
    startServer();
  }
  
  else{
    checkButton(15);    
    RGB rgb = hexToRGB(hexa);
    analogWrite(RED_PIN, rgb.red);
    analogWrite(GREEN_PIN, rgb.green);
    analogWrite(BLUE_PIN, rgb.blue);
  }
}



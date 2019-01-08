/*
This example will transmit a universe via Art-Net into the Network.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/

 bool isButton1Down = false;
 bool isButton0Down = false;
 int currentLight = 0;

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

//Wifi settings
const char* ssid = "metalab"; // CHANGE FOR YOUR SETUP
const char* password = ""; // CHANGE FOR YOUR SETUP

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.
const char host[] = "10.20.255.255"; // CHANGE FOR YOUR SETUP your destination
   
int lastPot = 0;

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin(host);
  artnet.setLength(60);
  artnet.setUniverse(3);

  {
  pinMode(D1, INPUT_PULLUP);        
  pinMode(D2, INPUT_PULLUP) ;
}

}

void loop()
{
  uint8_t i, j;

  
  if(digitalRead(D2) == 0)
  {

    isButton0Down = true;
  }
  else if(isButton0Down == true)
  {
    isButton0Down = false;
    changeLight(60); 
  }

  if(digitalRead(D1) == 0)
  {
    
    isButton1Down = true;
  }
  else if(isButton1Down == true)
  {
    isButton1Down = false;
    changeLight(-60); 
  }
  
  delay(20);
  
  
}

void changeLight(int amount)
{
  
  currentLight += amount;
  

  if(currentLight >  250)
    currentLight = 250;
  if(currentLight < 0)
    currentLight = 0;
    //Serial.println(currentLight);
    int j,i;
    for(j = 0; j < 6; j++)
    {
         for (i = 0; i < 60; i++) 
         {
           artnet.setByte(0 + (i*5), (byte)0);
           artnet.setByte(1 + (i*5), (byte)0);
           artnet.setByte(2 + (i*5), (byte)0);
           artnet.setByte(3 + (i*5), (byte)currentLight);
           artnet.setByte(4 + (i*5), (byte)currentLight);
           artnet.write();
         }
         delay(20);
    }
  
}



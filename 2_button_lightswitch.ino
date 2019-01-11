/*
This example will transmit a universe via Art-Net into the Network.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
*/

bool isButton0Down = false;
bool isButton1Down = false;
bool isButton2Down = false;
int currentLight = 0;
int currentMode = 9;

int lookupR[] = {1,0,0,1,1,0,1,0,0,0};
int lookupG[] = {0,1,0,1,0,1,1,0,0,0};
int lookupB[] = {0,0,1,0,1,1,1,0,0,0};
int lookupA[] = {0,0,0,0,0,0,0,1,0,1};
int lookupW[] = {0,0,0,0,0,0,0,0,1,1};

#define BUTTON0 4
//D2 on the ESP8266 D1 mini
#define BUTTON1 5
//D1
#define BUTTON2 0
//D3

//fourth button is reset

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

//Wifi settings
const char* ssid = "metalights"; // CHANGE FOR YOUR SETUP
const char* password = "metalights"; // CHANGE FOR YOUR SETUP

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.
const char host[] = "10.20.255.255"; // CHANGE FOR YOUR SETUP your destination


/*IPAddress ip(10,20,30,69);
//IPAddress broadcastIp(10,20,255,255);
IPAddress broadcastIp(255,255,255,255);

IPAddress netmask(255, 255, 0, 0);
char packet[]="Art-Net\0\0\x50\0\x0d\0\0\0\x03\0\x3caaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//char packet[]="Art-Net\0\0\x50\0\x0d\0\0\x03\0\0\x3caaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

WiFiUDP Udp;
unsigned int localUdpPort = 4211;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
*/
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
  //Udp.begin(localUdpPort);
  {
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
}

}

void loop()
{
  uint8_t i, j;

  
  if(digitalRead(BUTTON0) == 0)
  {

    isButton0Down = true;
  }
  else if(isButton0Down == true)
  {
    isButton0Down = false;
    changeLight(25);
  }

  if(digitalRead(BUTTON1) == 0)
  {
    
    isButton1Down = true;
  }
  else if(isButton1Down == true)
  {
    isButton1Down = false;
    changeLight(-25);
  }

  if(digitalRead(BUTTON2) == 0)
  {
    
    isButton2Down = true;
  }
  else if(isButton2Down == true)
  {
    isButton2Down = false;
    changeMode();
  }
  
  delay(20);
  
  
}

void changeLight(int amount)
{
  Serial.println("changeLight");
  currentLight += amount;
  

  if(currentLight >  250)
    currentLight = 250;
  if(currentLight < 0)
    currentLight = 0;

  sendOut();
}

void changeMode(){
  
  Serial.println("changeMode");
  currentMode = (currentMode + 1) % 10;
  sendOut();
  
}

void sendOut(){
    Serial.println("currentLight:");
    Serial.println(currentLight);
    Serial.println("currentMode:");
    Serial.println(currentMode);
    
    int j,i;
    /*for(i = 18; i < 78; i = i + 5 ){
      packet[ i ] = char(currentLight * lookupR[currentMode]);
      packet[i+1] = char(currentLight * lookupG[currentMode]);
      packet[i+2] = char(currentLight * lookupB[currentMode]);
      packet[i+3] = char(currentLight * lookupA[currentMode]);
      packet[i+4] = char(currentLight * lookupW[currentMode]);
    }*/

    
    for(j = 0; j < 6; j++)
    {
         for (i = 0; i < 60; i++)
         {
           artnet.setByte(0 + (i*5), (byte)(currentLight * lookupR[currentMode]));
           artnet.setByte(1 + (i*5), (byte)(currentLight * lookupG[currentMode]));
           artnet.setByte(2 + (i*5), (byte)(currentLight * lookupB[currentMode]));
           artnet.setByte(3 + (i*5), (byte)(currentLight * lookupA[currentMode]));
           artnet.setByte(4 + (i*5), (byte)(currentLight * lookupW[currentMode]));
           artnet.write();           
           //Udp.beginPacket(broadcastIp, 6454);
           //Udp.write(packet);
           //Udp.endPacket();
           
         }
         delay(20);
    }
  
}

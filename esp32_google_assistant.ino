#include<WiFi.h>
#include<Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define SSID "your wifi network"              //wifi network
#define Password "your wifi password"      // wifi password

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "your username"   //enter your adafruit username here
#define AIO_KEY  "your Adafruit IO key "           // copy paste your adafruit key 

int led = 2;
int led2 = 4;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);

Adafruit_MQTT_Subscribe LED_Control =Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME"/feeds/led-control");   //enter your feed key
Adafruit_MQTT_Subscribe Fan_Control =Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME"/feeds/fan-control");   // enter your feed key 

void MQTT_connect();

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(led,OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.println(); Serial.println();
  Serial.print("Connecting to WiFi");
  WiFi.begin(SSID,Password);
  while(WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wifi Connected");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());
  
  mqtt.subscribe(&LED_Control);   //feed name
  mqtt.subscribe(&Fan_Control);   //feed name
}

uint32_t x=0;

void MQTT_connect()
{
  int8_t again;
  if(mqtt.connected())
  {
    return;
  }
  Serial.print("Connecting to Adafruit IO");
  uint8_t retry = 5;
  while((again = mqtt.connect())!=0)
  {
    Serial.println(mqtt.connectErrorString(again));
    Serial.println("Retrying Adafruit connection in 5 seconds ");
    mqtt.disconnect();
    delay(5000);
    retry--;
    if(retry == 0)
    {
      while(1);
    }
  }
  Serial.println("");
  Serial.println("Adafruit IO is connected!");
}

void loop()
{
 MQTT_connect();
 
  Adafruit_MQTT_Subscribe * subscription ;
  while((subscription = mqtt.readSubscription(500)))
  {
    if(subscription == &LED_Control)
    {
      Serial.print(F("Got:"));
      Serial.println((char*)LED_Control.lastread);
      if(!strcmp((char*)LED_Control.lastread,"ON"))
      {
        digitalWrite(led,HIGH); 
      }
      else
      {
        digitalWrite(led,LOW);
      }
      
  }
  if(subscription == &  Fan_Control)
    {
      Serial.print(F("Got:"));
      Serial.println((char*)Fan_Control.lastread);
      if(!strcmp((char*)Fan_Control.lastread,"ON"))
      {
        digitalWrite(led2,HIGH); 
      }
      else
      {
        digitalWrite(led2,LOW);
      }
    }
  }
}

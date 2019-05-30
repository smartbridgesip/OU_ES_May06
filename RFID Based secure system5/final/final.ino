#include <ThingSpeak.h>

#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

const char* ssid ="poojitha";  
const char* password = "poojareddy";  
WiFiClient client;  
unsigned long myChannelNumber = 787442;  
const char * myWriteAPIKey = "Z7LHDZUYNKBEU8NA";  


int freq = 2000;
int channel = 0;
int resolution = 8;
int smoke1 = 4;
int smoke2 = 12;
// Your threshold value
int sensorThres1 = 2500;
 int sensorThres2 = 2500;
void sensor1(int );
void sensor2(int );


void setup() {

 
  // Connect to WiFi network  
  Serial.println();  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED)  
  {  
   delay(500);  
   Serial.print(".");  
  }  
  Serial.println("");  
  Serial.println("WiFi connected");  
  // Print the IP address  
  Serial.println(WiFi.localIP());  
  ThingSpeak.begin(client);   
  pinMode(smoke1, INPUT);
  pinMode(smoke2,INPUT);
  pinMode(5,OUTPUT);
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
}

void loop()
{
   static boolean data_state = false;  
  int analogSensor1 = analogRead(smoke1);
  int analogSensor2 = analogRead(smoke2);
  sensor1(analogSensor1);
  sensor2(analogSensor2);
  ThingSpeak.writeField(myChannelNumber, 1, analogSensor1, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, analogSensor2, myWriteAPIKey);
  delay(30000); 
}

void sensor1(int a)
{
  Serial.print("Pin A0: ");
  Serial.println(a);
  delay(1000);
  if (a > sensorThres1)
  {
    digitalWrite(5, HIGH);
    ledcWriteTone(channel, 2000);
  }
  else
  {
    digitalWrite(5, LOW);
  
  }
  
}

void sensor2(int b)
{
  Serial.print("Pin A2: ");
  delay(1000);
  Serial.println(b);
  if (b > sensorThres2)
  {
    digitalWrite(5, HIGH);
    ledcWriteTone(channel, 2000);
  }
  else
  {
    digitalWrite(5, LOW);
  
  }

  
}

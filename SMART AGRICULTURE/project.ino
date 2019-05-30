#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 2    // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11 
#include<ThingSpeak.h>
DHT dht(DHTPIN, DHTTYPE);
const char* ssid     = "SmartBridge"; // Your ssid
const char* password = "smartbridge@sb"; // Your Password
WiFiClient client;
unsigned long mychannelnumber=787423;
const char* APIkey="P25B5U1MQY00OID7";
float temp_celsius=0;
float temp_fahrenheit=0;
WiFiServer server(80);
int s;
int g;
int t;
float h;
void setup() {
  // put your setup code here, to run once:
  pinMode(33,INPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(34,INPUT);
  pinMode(35,INPUT);
  pinMode(2,INPUT);
  Serial.begin(115200);
  ThingSpeak.begin(client);
  Serial.println();
  Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
dht.begin();
}
Serial.println("");
Serial.println("WiFi is connected");
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
}
//lm35:
 void temparature(){
  int t=analogRead(33);
  temp_celsius=t/9.31;
  temp_fahrenheit = temp_celsius * 1.8 + 32.0;
Serial.print("  Temperature = ");
Serial.print(temp_celsius);
Serial.print(" Celsius, ");
Serial.print(temp_fahrenheit);
Serial.println(" Fahrenheit");
//delay(1000);
 }
//air qualifier:
void airqualifier(){
  g=analogRead(34);
  Serial.print("air qualifier:");
  Serial.println(g);
 // delay(1000);
  }
//soil moisture:
void moisture(){
  s=analogRead(35);
  Serial.print("moisture:");
  Serial.println(s);
 // ThingSpeak.writeField(mychannelnumber,3,s,APIkey);
  //delay(1000);
}
//humidity
void humidity(){
   float h = dht.readHumidity();
  Serial.print("Humidity:");
  Serial.println(h);
  
 ThingSpeak.writeField(mychannelnumber,4,h,APIkey);
  //delay(500);
}
//motor
void motor(){
  if(s<3000){
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  //delay(1000);
  }
  else{
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  //delay(1000);
  }
 }
void loop(){
  temparature();
  airqualifier();
  moisture();
   humidity();
  motor();
  //humidity();
  WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 10");  // update the page after 10 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>Digital Temperature meter</strong></span></p>");
client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>Temperature (*C)= ");
client.println(temp_celsius);
client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>Temperature (F) = ");
client.println(temp_fahrenheit);
client.print("</p>");
client.println("</html>");
//delay(5000);
//ThingSpeak.writeField(mychannelnumber,1,temp_celsius,APIkey);
//delay(30000);
client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>Air Qualifier</strong></span></p>");
client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>air qualifier= ");
client.println(g);
//delay(5000);
//ThingSpeak.writeField(mychannelnumber,2,g,APIkey);
//delay(30000);
client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>Digital Soil Moisture meter</strong></span></p>");
client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>Soil Moisture= ");
client.println(s);
//delay(5000);
 client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>Digital Humidity meter</strong></span></p>");
  client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>humidity= ");
  client.println(h);
 
//delay(30000);
//delay(5000);

delay(30000);

}

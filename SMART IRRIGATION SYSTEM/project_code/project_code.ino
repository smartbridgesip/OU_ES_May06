#include <ThingSpeak.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2    // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
 
const char* ssid     = "chinnu"; // Your ssid
const char* password = "12345678"; // Your Password

WiFiClient client;  
unsigned long myChannelNumber = 787456;  
const char * myWriteAPIKey = "STYGE3JD5TTGOGDK";  

#include <LiquidCrystal.h>
float temp_celsius = 0;
float temp_fahrenheit = 0;
int soil_val=0;
int  air_val=0;
WiFiServer server(80);

const int rs = 15, en = 4, d4 = 5, d5 = 18, d6 = 19, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
pinMode(35,INPUT);//soil sensor pin
pinMode(32,INPUT);//temperature sensor pin
pinMode(12,OUTPUT);//DC MOTOR INPUT
pinMode(13,OUTPUT);//DC MOTOR INPUT
pinMode(34,INPUT); // AIR SENSOR PIN
Serial.begin(115200);

dht.begin();

 //wifi//
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi is connected");
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
ThingSpeak.begin(client);  
}
void loop() {
 temp();
  soil();
  air();
 
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
client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>digital soil sensor</strong></span></p>");
client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>soil sensor values= ");
client.println(soil_val);
client.print("<p style='text-align: center;'><span style='font-size: x-large;'><strong>digital air quality sensor</strong></span></p>");
client.print("<p style='text-align: center;'><span style='color: #0000ff;'><strong style='font-size: large;'>air sensor values= ");
client.println(air_val);
client.print("</p>");
client.println("</html>");
delay(5000);
  // Turn off the display:
  lcd.noDisplay();
  delay(500);
  // Turn on the display:
  lcd.display();
  delay(500);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("T:");
  lcd.setCursor(2,0);
  lcd.print(temp_celsius);
  lcd.setCursor(7,0);
  lcd.print("C");
  lcd.setCursor(9,0);
  lcd.print(temp_fahrenheit);
  lcd.setCursor(14,0);
  lcd.print("F");
 
 
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different  
  // pieces of information in a channel. Here, we write to field 1. 
   ThingSpeak.writeField(787456, 1, temp_celsius, "STYGE3JD5TTGOGDK");  
   delay(20000); // ThingSpeak will only accept updates every 15 seconds.
    
   ThingSpeak.writeField(787456, 2, soil_val, "STYGE3JD5TTGOGDK"); 
   delay(20000); // ThingSpeak will only accept updates every 15 seconds.  
   
   ThingSpeak.writeField(787456, 3, air_val, "STYGE3JD5TTGOGDK");  
   delay(20000); // ThingSpeak will only accept updates every 15 seconds.  
}
void temp(){
  float a = analogRead(32);
  temp_celsius =  (a/9.31) ; 
  temp_fahrenheit = temp_celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(temp_celsius);
  Serial.print(" Celsius, ");
  Serial.print(temp_fahrenheit);
  Serial.println(" Fahrenheit");
  delay (1000);
  float h = dht.readHumidity();
  Serial.print("Humidity:");
  Serial.println(h);
  delay(500);  
}

void soil(){
 
 soil_val=analogRead(35);
 Serial.print("soil sensor value: ");
  Serial.println(soil_val);
  
 if ((soil_val)<<100)
  {
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
  }
  else
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
  }
}
  

void air(){
  
  air_val=analogRead(34);
  Serial.print("air sensor value: ");
  Serial.println(air_val);
 
}

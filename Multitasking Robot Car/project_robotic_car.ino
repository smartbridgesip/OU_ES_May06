
// Load Wi-Fi library
#include <WiFi.h>
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

// Replace with your network credentials
const char* ssid     = "SmartBridge";
const char* password = "smartbridge@sb";
WiFiClient client;  
unsigned long myChannelNumber = 787432;  
const char * myWriteAPIKey = "T6Z9TNKBFTFYDJHL";  

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output2State = "off";
String output4State = "off";
String output5State = "off";
String output18State = "off";


// Assign output variables to GPIO pins
const int output2= 2;
const int output4 = 4;
const int output5 = 5;
const int output18 = 18;
const int trigPin=19;
const int echoPin=21;
long duration;
int distance;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output2, OUTPUT);
   pinMode(output4, OUTPUT);
    pinMode(output5, OUTPUT);
   pinMode(output18, OUTPUT);
   pinMode(34, INPUT);
   pinMode(35, INPUT);
    pinMode(22,OUTPUT);
   pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);

     pinMode(22, OUTPUT);
   pinMode(23, OUTPUT);

   
  // Set outputs to LOW
  digitalWrite(output2, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(output18, LOW);
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(5000);
  server.begin();
    ThingSpeak.begin(client);  
}

void loop(){
  

 gas();
 delay(600);
 dist();
 delay(600);
 
fire();
delay(600);
robo();
delay(600);
 
}
void robo()
{
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output2, HIGH);
              digitalWrite(output4, LOW);
               digitalWrite(output5, HIGH);
              digitalWrite(output18, LOW);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
        
               digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
            }
             if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
                digitalWrite(output2, LOW);
              digitalWrite(output4, HIGH);
               digitalWrite(output5, LOW);
              digitalWrite(output18, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
            }
             if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              output2State = "on";
                digitalWrite(output2, HIGH);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              output2State = "off";
              digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
            }
             if (header.indexOf("GET /18/on") >= 0) {
              Serial.println("GPIO 18 on");
              output18State = "on";
                digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, HIGH);
              digitalWrite(output18, LOW);
            } else if (header.indexOf("GET /18/off") >= 0) {
              Serial.println("GPIO 18 off");
              output18State = "off";
              digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 2  
            client.println("<p>GPIO 2 - State " + output5State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">FRONT</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">STOP</button></a></p>");
            } 
             if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">BACK</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">STOP</button></a></p>");
            }  
            if (output2State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">RIGHT</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">STOP</button></a></p>");
            } 
             if (output18State=="off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">LEFT</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">STOP</button></a></p>");
            } 
               
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
   
  }
}

void gas()
{
  int gas=analogRead(35);
   Serial.print("gas");
  
  Serial.println(gas);
    ThingSpeak.writeField(myChannelNumber, 1,gas, myWriteAPIKey);  
               delay(30000);
   if(gas>=3000){
       Serial.println("HARMFUL GAS ALERT");
        digitalWrite(23, HIGH);
              digitalWrite(22, HIGH);
     digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
               
            
      
      
  
    }
  
  
  }
 void fire()
  {
    int fireval=analogRead(34);
     Serial.print("firevalue");
    Serial.println(fireval);
     ThingSpeak.writeField(myChannelNumber, 2, fireval, myWriteAPIKey); 
     delay(30000);
    if(fireval<=2500){
       Serial.println("FIRE ALERT");
        digitalWrite(23, HIGH);
              digitalWrite(22, HIGH);
     digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
             
      
      
  
    }
  }
void dist()
  {
    
     digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
   digitalWrite(trigPin,HIGH);
  delayMicroseconds(1000);
   digitalWrite(trigPin,LOW);
  duration=pulseIn(echoPin,HIGH);
 distance=duration*0.034/2;
  Serial.print("distance");
  Serial.println(distance);
   ThingSpeak.writeField(myChannelNumber, 3, distance, myWriteAPIKey);  
  delay(30000);
  if(distance<=100){
     Serial.println("OBSTACLE DETECTED");
     digitalWrite(output2, LOW);
              digitalWrite(output4, LOW);
               digitalWrite(output5, LOW);
              digitalWrite(output18, LOW);
    
  }
 }

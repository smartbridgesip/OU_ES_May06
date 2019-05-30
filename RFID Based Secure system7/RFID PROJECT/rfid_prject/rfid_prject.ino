#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>


#include<Servo.h>
#include<WiFi.h>
#include<WiFiClient.h>
#include<WiFiServer.h>
#include<WiFiUDP.h>
#include<ThingSpeak.h>

const char* ssid = "Redmi";  
const char* password = "sowmya123";  
WiFiClient client;  
unsigned long myChannelNumber = 787417;  
const char * myWriteAPIKey = "PVPK0W00F53QPV3S";  


Servo myservo;
int pos=0;

constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;   // Configurable, see typical pin layout above

const int led_1=2;
const int buz=12;
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
byte nuidPICC2[4]={251,43,20,53};
int count=0;

void setup() { 
  Serial.begin(115200);
  delay(10);
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
  pinMode(led_1,OUTPUT);
  pinMode(buz,OUTPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

 myservo.attach(13);

 // Serial.println(F("This code scan the MIFARE Classsic NUID."));
 // Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {

  
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

 // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
   // Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
   // Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    for(byte i=0; i< 4; i++)
    {
      if(nuidPICC[i]==nuidPICC2[i])
      {
            count++;
     // Serial.println("Welcome to Smart Bridge");
      }
    }
    if(count==4)
    {
       Serial.println("Authentication successfull");
       for(pos=0;pos<=180;pos=pos+1)
       {
        myservo.write(pos);
        
        delay(15);
   
       }
       delay(1000);
       for(pos=180;pos<=0;pos=pos-1)
       {
        myservo.write(pos);
        delay(15);
   
       }
       ThingSpeak.writeField(myChannelNumber, 1, 1, myWriteAPIKey);  
       ThingSpeak.writeField(myChannelNumber, 2, 1, myWriteAPIKey);
  
       delay(30000); // ThingSpeak will only accept updates every 15 seconds.  
   
       count=0;

       
    }
    else
      {
        Serial.println("Access Denined.....");
        digitalWrite(led_1,HIGH); 
        digitalWrite(buz,HIGH);
        count=0;
        delay(1000);
        digitalWrite(led_1,LOW); 
        digitalWrite(buz,LOW);
        ThingSpeak.writeField(myChannelNumber, 1, 0, myWriteAPIKey);  
        ThingSpeak.writeField(myChannelNumber, 2, 0, myWriteAPIKey);
  
        delay(30000); // ThingSpeak will only accept updates every 15 seconds.  
   
        return;
      }
   
  
    
    Serial.println(F("The RFID tag is:"));
   // Serial.print(F("In hex: "));
    //printHex(rfid.uid.uidByte, rfid.uid.size);
   // Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
 // else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

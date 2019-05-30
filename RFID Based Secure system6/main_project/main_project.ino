#include <Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
 
#include <ThingSpeak.h>  
  
const char* ssid = "Realme U1";  
const char* password = "saisri4435";  
WiFiClient client;  
unsigned long myChannelNumber = 787421;  
const char * myWriteAPIKey = "SXB13CA5TFBUAXN7";  


#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;  
Servo myservo;
// Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
byte nuidPICC2[4]={203 , 64, 167, 54};
int count=0;
int pos=0;

void setup() { 
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(33,OUTPUT);
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
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
   myservo.attach(13);
   
   

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }


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

    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
   

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    for(byte i=0; i< 4; i++)
    {
      if(nuidPICC[i]==nuidPICC2[i])
      {
            count++;
          }
    }

    if(count==4)
    {
      
       Serial.println("Authentication successfull");
       ThingSpeak.writeField(myChannelNumber, 1, 1,myWriteAPIKey ); 
        digitalWrite(2,LOW);
        digitalWrite(33,LOW); 
  
       
        for(pos=0;pos<=180;pos +=1)
  {
  myservo.write(pos);
  if(pos==180)
  delay(200);
  delay(15);
   }
  delay(2000);
 
  
  for(pos=180;pos>=0;pos -=1)
  {
    myservo.write(pos);
    if(pos==180)
    delay(200);
    delay(15);
  }
    }
      else
      {
        Serial.println("Access Denined.....");
        ThingSpeak.writeField(myChannelNumber, 1, 0,myWriteAPIKey );
        digitalWrite(2,HIGH);
         delay(1000);
         digitalWrite(2,LOW);
         digitalWrite(33,HIGH);
         delay(1000);
         digitalWrite(33,LOW);  
        
        
        count=0;
        return;
      }
    
    Serial.println(F("The RFID tag is:"));
 
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

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
  

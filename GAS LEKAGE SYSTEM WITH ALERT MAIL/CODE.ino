int freq = 2000;
int channel = 0;
int resolution = 8;
int smoke = 4;
// Your threshold value
int sensorThres = 2500;

void setup() {
  
  pinMode(smoke, INPUT);
  pinMode(5,OUTPUT);
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
}

void loop() {
  int analogSensor = analogRead(smoke);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  delay(1000);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    digitalWrite(5, HIGH);
    ledcWriteTone(channel, 2000);
  }
  else
  {
    digitalWrite(5, LOW);
  
  }
  delay(100);
}

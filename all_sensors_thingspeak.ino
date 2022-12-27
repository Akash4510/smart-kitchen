#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"

const int gasPin = A0;
const int flamePin = D0;
const int IRPin = D1;
const int DHTPin = D2;

const int led1 = D5;
const int led2 = D6;
const int led3 = D7;
const int led4 = D8;

// Variables to store different parameters
float temperature, humidity;
int flame = HIGH;
int IRValue, gasVal;

float tempThreshold = 24.50;

DHT dht(DHTPin, DHT11);  // Creating a DHT object

const char* ssid = "Kakoli";  // Your network SSID here
const char* password = "j2901das";  // Your network's password here
WiFiClient client;  // Creating the WIFIClient object

unsigned long channelNumber =  1990582;  // ThingSpeak Channel ID here
const char * writeAPIKey = "91LKBDA57MTZYY4Q";  // ThingSpeak write API key here

void blinkLed(int ledPin) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  digitalWrite(ledPin, LOW);
}

void blinkLed2(int ledPin) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, LOW);
    delay(200);
    digitalWrite(ledPin, HIGH);
    delay(200);
  }
  digitalWrite(ledPin, HIGH);
}

void setup()
{
  Serial.begin(9600);

  dht.begin();
  pinMode(flamePin, INPUT);
  pinMode(IRPin, INPUT);
  pinMode(gasPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  WiFi.begin(ssid, password);  // Begin the WIFI connection
  ThingSpeak.begin(client);  // Begin the ThingSpeak client
}

void loop()
{
  // Reading temperature and humidity from DHT pin
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  flame = digitalRead(flamePin);
  IRValue = digitalRead(IRPin);
  gasVal = analogRead(gasPin);

  // -------------Display----------------//

  // Display the readings in the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("ºC");
  Serial.print("Humidity: ");
  Serial.println(humidity);

  if (flame == LOW) {
    Serial.println("FLAME, FLAME, FLAME");
  }
  else {
    Serial.println("No flame");
  }

  Serial.print("IR Value: ");
  Serial.println(IRValue);

  Serial.print("Gas Value: ");
  Serial.println(gasVal);

  // -------------LED----------------//

  if (!IRValue) {
    blinkLed2(led4);
  }

  if (temperature > tempThreshold) {
    blinkLed(led3);
  }

  if (gasVal > 200) {
    blinkLed(led2);
  }

  if (flame == LOW) {
    blinkLed(led3);
  }

  // -------------Cloud----------------//

  // Setting the data to be send to different fields in ThingSpeak channel
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, IRValue);
  ThingSpeak.setField(4, flame);
  ThingSpeak.setField(5, gasVal);

  // Write the data to the channel's field
  ThingSpeak.writeFields(channelNumber, writeAPIKey);

  delay(15000);  // Adding a delay of 15s
}

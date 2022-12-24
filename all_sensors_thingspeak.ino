#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include "DHT.h"

const int DHTPin = D0;
const int flamePin = D1;
const int IRPin = D2;
const int gasPin = A0;

float temperature, humidity;
int flame = HIGH;
int IRValue, gasVal;

DHT dht(DHTPin, DHT11);  // Creating a DHT object

const char* ssid = "";  // Your network SSID here
const char* password = "";  // Your network's password here
WiFiClient client;  // Creating the WIFIClient object

unsigned long channelNumber =  0000000;  // ThingSpeak Channel ID here
const char * writeAPIKey = "";  // ThingSpeak write API key here

void setup()
{
  Serial.begin(9600);  // Setting up the baud rate
  dht.begin();  // Begin DHT
  WiFi.begin(ssid, password);  // Begin the WIFI connection
  ThingSpeak.begin(client);  // Begin the ThingSpeak client

  pinMode(flamePin, INPUT);
  pinMode(IRPin, INPUT);
  pinMode(gasPin, INPUT);
}

void loop()
{
  // Reading temperature and humidity from DHT pin
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  flame = digitalRead(flamePin);
  IRValue = digitalRead(IRPin);
  gasVal = analogRead(gasPin);

  // Display the readings in the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("ºC");
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.println();

  // Setting the data to be send to different fields in ThingSpeak channel
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  // Write the data to the channel's field
  ThingSpeak.writeFields(channelNumber, writeAPIKey);


  if (flame == LOW) {
    Serial.println("FLAME, FLAME, FLAME");
  }
  else {
    Serial.println("No flame");
  }
  delay(3000);

  Serial.print("IR Value: ");
  Serial.println(IRValue);

  Serial.println("Gas Value: ");
  Serial.println(gasVal);

  delay(15000);  // Adding a delay of 15s
}

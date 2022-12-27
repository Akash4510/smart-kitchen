//------------- INCLUDE LIBRARIES -------------//
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"

//------------- DEFINE SENSOR PINS ------------//
#define GAS_PIN A0
#define FLAME_PIN D0
#define IR_PIN D1
#define DHT_PIN D2

//-------------- DEFINE LED PINS --------------//
#define LED1 D5
#define LED2 D6
#define LED3 D7
#define LED4 D8

//---------- DEFINE THINGSPEAK FIELDS ---------//
#define THINGSPEAK_GAS_FIELD 1
#define THINGSPEAK_TEMPERATURE_FIELD 2
#define THINGSPEAK_HUMIDITY_FIELD 3
#define THINGSPEAK_IR_FIELD 4
#define THINGSPEAK_FLAME_FIELD 5

//-- VARIABLES TO STORE DIFFERENT PARAMETERS --//
int gasValue;
float temperatureValue;
float humidityValue;
int IRValue;
int flameValue;

//---- THRESHOLD FOR DIFFERENT PARAMETERS -----//
const int gasThreshold = 200;
const float temperatureThreshold = 50.00;
const float humidityThreshold = 50.00;

//------------- DEFINE DHT SENSOR -------------//
DHT dht(DHT_PIN, DHT11);

//---------- DEFINE WIFI CREDENTIALS ----------//
const char *wifiSSID = "";     // Your network SSID here
const char *wifiPassword = ""; // Your network's password here

//----------- CREATING WIFI CLIENT ------------//
WiFiClient client;

//------ DEFINE THINGSPEAK CREDENTIALS --------//
unsigned long tsChannelNumber = 1990582;        // ThingSpeak Channel ID here
const char *tsWriteAPIKey = "91LKBDA57MTZYY4Q"; // ThingSpeak write API key here

//-------- DEFINE LED BLINKING FUNCTION -------//
void blinkLed(int ledPin)
{
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}

//------------- SETUP FUNCTION ---------------//
void setup()
{
  Serial.begin(9600);

  dht.begin();
  WiFi.begin(wifiSSID, wifiPassword);
  ThingSpeak.begin(client);

  pinMode(GAS_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

//-------------- LOOP FUNCTION ----------------//
void loop()
{
  // Reading the sensor values
  temperatureValue = dht.readTemperature();
  humidityValue = dht.readHumidity();

  flameValue = digitalRead(FLAME_PIN);
  IRValue = digitalRead(IR_PIN);
  gasValue = analogRead(GAS_PIN);

  // Display the readings in the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperatureValue);
  Serial.println("ºC");
  Serial.print("Humidity: ");
  Serial.println(humidityValue);

  if (flameValue == LOW)
    Serial.println("Flame detected");
  else
    Serial.println("No flame detected");

  Serial.print("IR Value: ");
  Serial.println(IRValue);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);
  Serial.println();

  // Blink the LED if the threshold is crossed
  if (gasValue > gasThreshold)
    blinkLed(LED1);

  if (temperatureValue > temperatureThreshold)
    blinkLed(LED2);

  if (!IRValue)
    blinkLed(LED3);

  if (flameValue == LOW)
    blinkLed(LED4);

  // Send the sensor values to ThingSpeak
  ThingSpeak.setField(THINGSPEAK_GAS_FIELD, gasValue);
  ThingSpeak.setField(THINGSPEAK_TEMPERATURE_FIELD, temperatureValue);
  ThingSpeak.setField(THINGSPEAK_HUMIDITY_FIELD, humidityValue);
  ThingSpeak.setField(THINGSPEAK_IR_FIELD, IRValue);
  ThingSpeak.setField(THINGSPEAK_FLAME_FIELD, flameValue);

  ThingSpeak.writeFields(tsChannelNumber, tsWriteAPIKey);

  delay(10000);
}

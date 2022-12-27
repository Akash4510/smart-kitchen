//--------------- BLYNK APP SETUP -------------//
// (MUST BE IN THE VERY TOP OF THE FILE)
#define BLYNK_TEMPLATE_ID "TMPLywJyKzdu"
#define BLYNK_DEVICE_NAME "Smart Kitchen"
#define BLYNK_AUTH_TOKEN "RE38yM7eNc0QsBTosMUMY36nGhhGSnlB"

#define variable_label_1 "Gas"
#define variable_label_2 "Temperature"
#define variable_label_3 "Humidity"
#define variable_label_4 "IR"
#define variable_label_5 "Flame"

//------------- INCLUDE LIBRARIES -------------//
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
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

//------------- DEFINE BLYNK PINS -------------//
#define BLYNK_GAS_PIN V0
#define BLYNK_TEMPERATURE_PIN V1
#define BLYNK_HUMIDITY_PIN V2
#define BLYNK_IR_PIN V3
#define BLYNK_FLAME_PIN V4

//------- DEFINE BLYNK CONTROL SWITCHES -------//
#define BLYNK_SWITCH_PIN_1 V5
#define BLYNK_SWITCH_PIN_2 V6
#define BLYNK_SWITCH_PIN_3 V7
#define BLYNK_SWITCH_PIN_4 V8

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
const char *wifiSSID = "POCO M2 Pro";      // Your network SSID here
const char *wifiPassword = "unlockby8800"; // Your network's password here

//----------- CREATING WIFI CLIENT ------------//
WiFiClient client;

//------ DEFINE THINGSPEAK CREDENTIALS --------//
unsigned long tsChannelNumber = 1990582;        // ThingSpeak Channel ID here
const char *tsWriteAPIKey = "91LKBDA57MTZYY4Q"; // ThingSpeak write API key here

//------- SETUP BLYNK CONTROL SWITCHES --------//
int bSwitch1Status = 0;
int bSwitch2Status = 0;
int bSwitch3Status = 0;
int bSwitch4Status = 0;

BLYNK_WRITE(BLYNK_SWITCH_PIN_1)
{
  bSwitch1Status = param.asInt();
}

BLYNK_WRITE(BLYNK_SWITCH_PIN_2)
{
  bSwitch2Status = param.asInt();
}

BLYNK_WRITE(BLYNK_SWITCH_PIN_3)
{
  bSwitch3Status = param.asInt();
}

BLYNK_WRITE(BLYNK_SWITCH_PIN_4)
{
  bSwitch4Status = param.asInt();
}

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
  Blynk.begin(BLYNK_AUTH_TOKEN, wifiSSID, wifiPassword);

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
  // Run the Blynk client
  Blynk.run();

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
  {
    Serial.println("Flame detected");
  }
  else
  {
    Serial.println("No flame detected");
  }

  Serial.print("IR Value: ");
  Serial.println(IRValue);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);
  Serial.println();

  // Blink the LED if the threshold is crossed
  // if (!IRValue)
  //   blinkLed(LED3);

  // if (temperatureValue > temperatureThreshold)
  //   blinkLed(LED2);

  // if (gasValue > gasThreshold)
  //   blinkLed(LED1);

  // if (flameValue == LOW)
  //   blinkLed(LED4);

  // Control the LED using the Blynk app
  digitalWrite(LED1, bSwitch1Status);
  digitalWrite(LED2, bSwitch2Status);
  digitalWrite(LED3, bSwitch3Status);
  digitalWrite(LED4, bSwitch4Status);

  // Send the sensor values to ThingSpeak
  ThingSpeak.setField(THINGSPEAK_GAS_FIELD, gasValue);
  ThingSpeak.setField(THINGSPEAK_TEMPERATURE_FIELD, temperatureValue);
  ThingSpeak.setField(THINGSPEAK_HUMIDITY_FIELD, humidityValue);
  ThingSpeak.setField(THINGSPEAK_IR_FIELD, IRValue);
  ThingSpeak.setField(THINGSPEAK_FLAME_FIELD, flameValue);

  ThingSpeak.writeFields(tsChannelNumber, tsWriteAPIKey);

  // Send the sensor values to Blynk
  Blynk.virtualWrite(BLYNK_GAS_PIN, gasValue);
  Blynk.virtualWrite(BLYNK_TEMPERATURE_PIN, temperatureValue);
  Blynk.virtualWrite(BLYNK_HUMIDITY_PIN, humidityValue);
  Blynk.virtualWrite(BLYNK_IR_PIN, IRValue);
  Blynk.virtualWrite(BLYNK_FLAME_PIN, flameValue);
}

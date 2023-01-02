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
#define DHT_PIN D2
#define FLAME_PIN D1
#define IR_PIN D0

//------------- DEFINE RELAY PINS -------------//
#define BUZZER D5
#define WATER_PUMP D6
#define EXHAUST_FAN_1 D7
#define EXHAUST_FAN_2 D8

//-------------- DEFINE LOOP LED --------------//
#define LOOP_LED D4

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

//--------- DEFINE RELAY HIGH AND LOW ---------//
// Relay works in reverse logic
// (RELAY HIGH = LOW, RELAY LOW = HIGH)
#define RELAY_HIGH LOW
#define RELAY_LOW HIGH

//-- VARIABLES TO STORE DIFFERENT PARAMETERS --//
int gasValue;
float temperatureValue;
float humidityValue;
int IRValue;
int flameValue;

//---- THRESHOLD FOR DIFFERENT PARAMETERS -----//
const int gasThreshold = 300;
const float temperatureThreshold = 25.00;
const float humidityThreshold = 40.00;

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

//-------------- DEFINE FUNCIONS --------------//
void blinkLoopLed()
{
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(LOOP_LED, HIGH);
    delay(100);
    digitalWrite(LOOP_LED, LOW);
    delay(100);
  }
  digitalWrite(LOOP_LED, LOW);
}

void playBuzzer()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(BUZZER, RELAY_HIGH);
    delay(100);
    digitalWrite(BUZZER, RELAY_LOW);
    delay(100);
  }
  digitalWrite(BUZZER, RELAY_LOW);
}

//-------- DEFINE LED BLINKING FUNCTION -------//

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

  pinMode(BUZZER, OUTPUT);
  pinMode(WATER_PUMP, OUTPUT);
  pinMode(EXHAUST_FAN_1, OUTPUT);
  pinMode(EXHAUST_FAN_2, OUTPUT);

  digitalWrite(LOOP_LED, LOW);

  digitalWrite(BUZZER, RELAY_LOW);
  digitalWrite(WATER_PUMP, RELAY_LOW);
  digitalWrite(EXHAUST_FAN_1, RELAY_LOW);
  digitalWrite(EXHAUST_FAN_2, RELAY_LOW);

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  blinkLoopLed();
  playBuzzer();
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
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Serial.print("Temperature: ");
  Serial.print(temperatureValue);
  Serial.println("ºC");
  Serial.print("Humidity: ");
  Serial.print(humidityValue);
  Serial.println("%");

  if (flameValue == LOW)
    Serial.println("Flame detected");
  else
    Serial.println("No flame detected");

  if (IRValue == LOW)
    Serial.println("Someone is there :(");
  else
    Serial.println("I'm Alone :)");

  Serial.println();

  // Control the relay using the Blynk app
  digitalWrite(BUZZER, !bSwitch1Status);
  digitalWrite(WATER_PUMP, !bSwitch2Status);
  digitalWrite(EXHAUST_FAN_1, !bSwitch3Status);
  digitalWrite(EXHAUST_FAN_2, !bSwitch4Status);

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

  // Blink the led once the loop executed successfully
  blinkLoopLed();
}

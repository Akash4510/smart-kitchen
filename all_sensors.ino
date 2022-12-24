#include "DHT.h"

// Setting up the pins
const int gasPin = A0;
const int flamePin = D0;
const int IRPin = D1;
const int DHTPin = D2;

// Variables to store different parameters
float temperature, humidity;
int flame = HIGH;
int IRValue, gasVal;

DHT dht(DHTPin, DHT11);  // Creating a DHT object

void setup()
{
  Serial.begin(9600);

  dht.begin();
  pinMode(flamePin, INPUT);
  pinMode(IRPin, INPUT);
  pinMode(gasPin, INPUT);
}

void loop()
{
  // Reading different parameters with different sensors
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  flame = digitalRead(flamePin);
  IRValue = digitalRead(IRPin);
  gasVal = analogRead(gasPin);

  // Displaying temperature and humidity
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("ºC");
  Serial.print("Humidity: ");
  Serial.println(humidity);

  // Checking for flame
  if (flame == LOW) {
    Serial.println("FLAME, FLAME, FLAME");
  }
  else {
    Serial.println("No flame");
  }

  // Displaying IR Value
  Serial.print("IR Value: ");
  Serial.println(IRValue);

  // Displaying Gas Value
  Serial.print("Gas Value: ");
  Serial.println(gasVal);
  Serial.println();

  delay(3000);  // Adding a delay of 3s
}

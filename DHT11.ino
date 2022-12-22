#include "DHT.h";

const int DHTPin = D2;
float temperature, humidity;

DHT dht(DHTPin, DHT11);  // Creating a DHT object

void setup()
{
  Serial.begin(9600);  // Setting up the baud rate
  dht.begin();  // Begin DHT
}

void loop()
{
  // Reading temperature and humidity from DHT pin
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Display the readings in the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("ºC");
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.println();

  delay(2000);  // Adding a delay of 2s
}

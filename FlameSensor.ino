const int flamePin = D3;
int flame = HIGH;

void setup() {
  pinMode(flamePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  flame = digitalRead(flamePin);

  if (flame == LOW) {
    Serial.println("FLAME, FLAME, FLAME");
  }
  else {
    Serial.println("No flame");
  }

  delay(1000);
}

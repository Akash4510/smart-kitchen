const int IRPin = D1;
int IRValue;

void setup() {
  Serial.begin(9600);
  pinMode(IRPin, INPUT);
}

void loop() {
  IRValue = digitalRead(IRPin);
  if (IRValue == 0) {
    Serial.println("Someone is there :(");
  }
  else {
    Serial.println("I'm Alone :)");
  }
  delay(2000);
}

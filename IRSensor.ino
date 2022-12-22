#define IRPin D2;
int IRValue;

void setup() {
  Serial.begin(9600);
  pinMode(IRPin, INPUT);
}

void loop() {
  IRValue = digitalRead(IRPin);
  Serial.println(IRValue);
  delay(2000);
}

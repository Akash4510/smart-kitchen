const int gasPin = A0;
float val;

void setup() {
  Serial.begin(9600);
}

void loop() {
  val = analogRead(gasPin);
  Serial.println(val);
  delay(1000);
}

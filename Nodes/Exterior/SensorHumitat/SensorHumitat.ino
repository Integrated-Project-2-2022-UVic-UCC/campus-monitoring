int Humitat = A0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.print("Valor humitat del sol: ");
  Serial.println(analogRead(Humitat));
  delay(1000);
}

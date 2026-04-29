
int input1 = 14;
int input2 = 15;
int enable = 36;

void setup() {
  Serial.begin(9600);
  pinMode(enable, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);

  digitalWrite(enable, HIGH);
  digitalWrite(input2, LOW);
}

void loop() {
   if (Serial.available() > 0) {
    int sel = Serial.parseInt();
    // Clear buffer
    while (Serial.available()) {
      Serial.read();
    }
    Serial.println(sel);
    analogWrite(input1, sel);
}
}

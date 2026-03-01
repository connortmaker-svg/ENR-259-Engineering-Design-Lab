void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);  
}

void loop() {
  turnRight();
  delay(2000);
  turnOff();
  delay(2000);
  turnLeft();
  delay(2000);
  turnOff();
  delay(2000);
}

void turnRight(){
  digitalWrite(2, HIGH);
  digitalWrite(3,LOW);
}

void turnLeft(){
  digitalWrite(3, HIGH);
  digitalWrite(2,LOW);
}

void turnOff(){
  digitalWrite(3, LOW);
  digitalWrite(2,LOW);
}
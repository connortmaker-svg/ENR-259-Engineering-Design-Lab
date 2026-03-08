
#define Enable 10
#define In_Cw 9
#define In_CCW 8 

void setup() {
  pinMode(Enable, OUTPUT);
  pinMode(In_Cw, OUTPUT);  
  pinMode(In_CCW, OUTPUT);  
}

void loop() {
  analogWrite(In_Cw, )
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
  digitalWrite(Enable, LOW);
}

void turnOn(){
  digitalWrite(Enable, LOW);
}
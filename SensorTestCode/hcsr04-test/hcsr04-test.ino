int trig=9;
int echo=8;
int duration;
float distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  pinMode(echo, INPUT);
  delay(6000);
  Serial.println("Distance:");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  if (duration >= 38000) 
  {
    Serial.print("Out of range");
  } /* if */
  else 
  {
    distance = duration / (5.8); // (convert to mm)
    Serial.print(distance);
    Serial.println("mm");
  } /* else */
  delay(1000);
}

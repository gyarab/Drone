void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  pinMode(A4,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("CH4 : ");
  Serial.print(pulseIn(A5,HIGH));
  Serial.print(" | ");
  Serial.print("CH3 : ");
  Serial.print(pulseIn(A6,HIGH));
  Serial.print(" | ");
  Serial.print("CH2 : ");
  Serial.print(pulseIn(A7,HIGH));
  Serial.print(" | ");
  Serial.print("CH1 : ");
  Serial.print(pulseIn(A4,HIGH));
  Serial.println();
  
}

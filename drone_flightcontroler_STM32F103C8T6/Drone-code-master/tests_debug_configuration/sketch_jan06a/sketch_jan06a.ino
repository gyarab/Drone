#include <Servo.h>

Servo FR;
Servo FL;
Servo RR;
Servo RL;
bool x = true;
void setup() {
  // put your setup code here, to run once:
  FR.attach(PB9);
  FL.attach(PB8);
  RR.attach(PB7);
  RL.attach(PB6);
  FR.writeMicroseconds(2000);
  FL.writeMicroseconds(2000);
  RR.writeMicroseconds(2000);
  RR.writeMicroseconds(2000);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.read() > 0 || !x) {
    FR.writeMicroseconds(1000);
    FL.writeMicroseconds(1000);
    RR.writeMicroseconds(1000);
    RL.writeMicroseconds(1000);
    Serial.print("x");
    x = false;
  } else if (x) {
    FR.writeMicroseconds(2000);
    FL.writeMicroseconds(2000);
    RR.writeMicroseconds(2000);
    RL.writeMicroseconds(2000);
  }

}

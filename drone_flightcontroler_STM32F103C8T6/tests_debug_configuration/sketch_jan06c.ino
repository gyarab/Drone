#include <Servo.h>
Servo motorFL;
Servo motorFR;
Servo motorRL;
Servo motorRR;
void setup() {
  // put your setup code here, to run once:
motorFL.attach(PB6);
motorFR.attach(PB9);
motorRL.attach(PB7);
motorRR.attach(PB8);

motorFL.writeMicroseconds(1000);
motorFR.writeMicroseconds(1000);
motorRL.writeMicroseconds(1000);
motorRR.writeMicroseconds(1000);

delay(5000);

}

void loop() {
  // put your main code here, to run repeatedly:
motorFL.writeMicroseconds(1500);
motorFR.writeMicroseconds(1500);
motorRL.writeMicroseconds(1500);
motorRR.writeMicroseconds(1500);
  

}

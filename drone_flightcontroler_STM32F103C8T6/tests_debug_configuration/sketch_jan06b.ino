#include <Servo.h>
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
int i = 1047;
void setup() {
  // put your setup code here, to run once:
ESC1.attach(6);
ESC1.writeMicroseconds(1000);
ESC2.attach(9);
ESC2.writeMicroseconds(1000);
ESC3.attach(10);
ESC3.writeMicroseconds(1000);
ESC4.attach(11);
ESC4.writeMicroseconds(1000);
delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
ESC1.writeMicroseconds(i);
ESC2.writeMicroseconds(i);
ESC3.writeMicroseconds(i);
ESC4.writeMicroseconds(i);
  
}

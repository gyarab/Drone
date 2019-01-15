#include <Servo.h>

Servo ESC;
bool x = true;
void setup() {
  // put your setup code here, to run once:
ESC.attach(9);
ESC.writeMicroseconds(2000);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.read()>0 || !x){
    ESC.writeMicroseconds(1000);
    Serial.print("x");
    x= false;
    }else if(x){
      ESC.writeMicroseconds(2000);
      }

}

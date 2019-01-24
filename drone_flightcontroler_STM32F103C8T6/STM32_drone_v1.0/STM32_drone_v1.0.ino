#include <Servo.h>
#include <Wire.h>
TwoWire Wire2(2,I2C_FAST_MODE); // druhy i2c port na STM32 s refresh ratem 400kHz


Servo S1, S2, S3, S4;
String command = "";

int16_t gyroPitch;
int16_t gyroRoll;
int16_t gyroYaw;
int16_t acX;
int16_t acY;
int16_t acZ;
int16_t temp;
int16_t rollCal;
int16_t pitchCal;
int16_t yawCal;
int16_t acXCal;
int16_t acYCal;
int16_t acZCal;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  S1.attach(PB6);
  S2.attach(PB7);
  S3.attach(PB8);
  S4.attach(PB9);


  pinMode(PA6, OUTPUT);
  pinMode(PA8, OUTPUT);
  digitalWrite(PA6, false);
  //.write("AT");
  Serial1.begin(115200);
  S1.writeMicroseconds(1000);
  S2.writeMicroseconds(1000);
  S3.writeMicroseconds(1000);
  S4.writeMicroseconds(1000);
  delay(4000);
  gyroSetup();
  calibrateGyro();



}

/*int numberCharToInt(char c) {
  for (int i = 0; i < 10; i++) {
    if (numbers[i] == c)return i;
  }
  return -1;
}*/

void loop() {
  // put your main code here, to run repeatedly:

  S1.writeMicroseconds(1000);
  S2.writeMicroseconds(1000);
  S3.writeMicroseconds(1000);
  S4.writeMicroseconds(1000);

  readGyro();
  Serial.println();
  Serial.print("Pitch : ");
  Serial.print(gyroPitch);
  Serial.print(" || ");
  Serial.print("Roll : ");
  Serial.print(gyroRoll);
  Serial.print(" || ");
  Serial.print("Yaw : ");
  Serial.print(gyroYaw);
  Serial.println();
  Serial.print("AccX : ");
  Serial.print(acX);
  Serial.print(" || ");
  Serial.print("AccY : ");
  Serial.print(acY);
  Serial.print(" || ");
  Serial.print("AccZ : ");
  Serial.print(acZ);
  
  /*EN1.writeMicroseconds(1150);
    delay(5000);
    EN1.writeMicroseconds(1000);
    if(Serial.available()){
    Serial1.write(Serial.read());
    }
    if(Serial1.available()){
    tone(PA8,2000);
    Serial.write(Serial1.read());
    noTone(PA8);
    }
  */


}

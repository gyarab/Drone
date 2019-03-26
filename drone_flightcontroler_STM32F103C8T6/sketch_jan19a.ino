#include <SoftWire.h>
#include <Wire.h>


#include <Servo.h>


const int MPU6050_addr = 0x68;


int16_t AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ;
int state = 1;
int power = 1000;
int power2 = 1000;

TwoWire WIRE2(2, I2C_FAST_MODE);
#define Wire WIRE2;
Servo FR;
Servo FL;
Servo RR;
Servo RL;


void setup() {
  // put your setup code here, to run once:
  initPins();
  initWire();
  initSerial();

}

void loop() {
  // put your main code here, to run repeatedly:
  switch (state) {
    case 0:
      for (int i = 0; i < 200; i++) {
        FR.writeMicroseconds(1000 + (i));
        FL.writeMicroseconds(1000 + i);

        RR.writeMicroseconds(1000 + (200 - i));
        RL.writeMicroseconds(1000 + (200 - i));
        delay(30);
      }

      for (int i = 0; i < 200; i++) {
        FR.writeMicroseconds(1000 + (200 - i));
        FL.writeMicroseconds(1000 + (200 - i));

        RR.writeMicroseconds(1000 + i);
        RL.writeMicroseconds(1000 + i);
        delay(30);
      }

      //playMusic();
      break;

    case 1:
      WIRE2.beginTransmission(MPU6050_addr);
      WIRE2.write(0x3B);
      WIRE2.endTransmission(false);
      WIRE2.requestFrom(MPU6050_addr, 14);
      AccX = WIRE2.read() << 8 | WIRE2.read();
      AccY = WIRE2.read() << 8 | WIRE2.read();
      AccZ = WIRE2.read() << 8 | WIRE2.read();
      Temp = WIRE2.read() << 8 | WIRE2.read();
      GyroX = WIRE2.read() << 8 | WIRE2.read();
      GyroY = WIRE2.read() << 8 | WIRE2.read();
      GyroZ = WIRE2.read() << 8 | WIRE2.read();

      Serial.print("AccX = "); Serial.print(AccX);
      Serial.print(" || AccY = "); Serial.print(AccY);
      Serial.print(" || AccZ = "); Serial.print(AccZ);
      Serial.print(" || Temp = "); Serial.print(Temp / 340.00 + 36.53);
      Serial.print(" || GyroX = "); Serial.print(GyroX);
      Serial.print(" || GyroY = "); Serial.print(GyroY);
      Serial.print(" || GyroZ = "); Serial.println(GyroZ);
      delay(100);
      break;

    case 2:
      break;

    case 3:
      break;

    case 4:
      break;
  }
}


void initPins() {
  pinMode(PB5, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PA0, OUTPUT);
  FR.attach(PB9);
  FL.attach(PB8);
  RR.attach(PB7);
  RL.attach(PB6);

  FR.writeMicroseconds(1000);
  FL.writeMicroseconds(1000);
  RR.writeMicroseconds(1000);
  RL.writeMicroseconds(1000);
  delay(5000);
}

void initWire() {
  WIRE2.begin();
  WIRE2.beginTransmission(MPU6050_addr);
  WIRE2.write(0x6B);
  WIRE2.write(0);
  WIRE2.endTransmission(true);
}

void initSerial() {
  Serial.begin(115200);
}

void playMusic() {

}

void ledLight(int pin) {
  //digitalWrite(PB5,HIGH);
  digitalWrite(PB4, HIGH);
}

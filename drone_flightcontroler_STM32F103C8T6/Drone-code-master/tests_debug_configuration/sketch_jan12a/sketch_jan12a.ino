#include<Wire.h>    
#include<SoftWire.h>    

int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
const int MPU6050_addr=0x68;
TwoWire WIRE2(2,I2C_FAST_MODE);
#define Wire WIRE2;
const int key = PA6;

void setup() {
  
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  //SoftWIRE2 WIRE2(PB11,PB10,SOFT_FAST);

  WIRE2.begin();
  WIRE2.beginTransmission(MPU6050_addr);
  WIRE2.write(0x6B);
  WIRE2.write(0);
  WIRE2.endTransmission(true);
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  WIRE2.beginTransmission(MPU6050_addr);
  WIRE2.write(0x3B);
  WIRE2.endTransmission(false);
  WIRE2.requestFrom(MPU6050_addr,14);
  AccX=WIRE2.read()<<8|WIRE2.read();
  AccY=WIRE2.read()<<8|WIRE2.read();
  AccZ=WIRE2.read()<<8|WIRE2.read();
  Temp=WIRE2.read()<<8|WIRE2.read();
  GyroX=WIRE2.read()<<8|WIRE2.read();
  GyroY=WIRE2.read()<<8|WIRE2.read();
  GyroZ=WIRE2.read()<<8|WIRE2.read();
  
  Serial.print("AccX = "); Serial.print(AccX);
  Serial.print(" || AccY = "); Serial.print(AccY);
  Serial.print(" || AccZ = "); Serial.print(AccZ);
  Serial.print(" || Temp = "); Serial.print(Temp/340.00+36.53);
  Serial.print(" || GyroX = "); Serial.print(GyroX);
  Serial.print(" || GyroY = "); Serial.print(GyroY);
  Serial.print(" || GyroZ = "); Serial.println(GyroZ);
  delay(100);
}

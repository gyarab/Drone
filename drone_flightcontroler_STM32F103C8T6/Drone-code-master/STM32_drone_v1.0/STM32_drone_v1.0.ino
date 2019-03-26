#include <Servo.h>
#include <Wire.h>
TwoWire Wire2(2, I2C_FAST_MODE); // druhy i2c port na STM32 s refresh ratem 400kHz


Servo S1, S2, S3, S4;
String command = "";

//PID
//Roll
float tError;
float pidI;
float pidP;
float pidD;
float befError;
float pid;
float pGain = 2.9;
float iGain = 0.005;
float dGain = 1.35;
int maxPitch = 500;
float gPitcCDeg;

float AccAngleX;
float AccAngleY;

float GyroAngleX;
float GyroAngleY;

float TotalAngleX;
float TotalAngleY;


//Gyro
int16_t gyroX;
int16_t gyroY;
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


int FR = 1000;
int FL = 1000;
int RR = 1000;
int RL = 1000;

int throtle = 1300;


float rad_to_deg = 180 / 3.141592654;
float elapsedTime, eTime, timePrev;

void setup() {
  // put your setup code here, to run once:
  Wire2.begin(); //begin the Wire22 comunication
  Wire2.beginTransmission(0x68);
  Wire2.write(0x6B);
  Wire2.write(0);
  Wire2.endTransmission(true);
  Serial.begin(115200);

  S1.attach(PB9); // Front - Right
  S2.attach(PB8); // Front - Left
  S3.attach(PB7); // Rear - Right
  S4.attach(PB6); // Rear - Left


  pinMode(PA6, OUTPUT);
  pinMode(PA8, OUTPUT);
  digitalWrite(PA6, false);
  //.write("AT");
  Serial1.begin(115200);

  S1.writeMicroseconds(FR);
  S2.writeMicroseconds(FL);
  S3.writeMicroseconds(RR);
  S4.writeMicroseconds(RL);
  delay(3000);
  eTime = millis();
  //gyroSetup();
  //  calibrateGyro();



}

/*int numberCharToInt(char c) {
  for (int i = 0; i < 10; i++) {
    if (numbers[i] == c)return i;
  }
  return -1;
  }*/

void loop() {
  // put your main code here, to run repeatedly:
  timePrev = eTime;  // the previous time is stored before the actual time read
  eTime = millis();  // actual time read
  elapsedTime = (eTime - timePrev) / 1000;
  if (Serial.read());
  /*  S1.writeMicroseconds(1000);
    S2.writeMicroseconds(1000);
    S3.writeMicroseconds(1000);
    S4.writeMicroseconds(1000);*/

  //readGyro();

  Wire2.beginTransmission(0x68);
  Wire2.write(0x3B); //Ask for the 0x3B register- correspond to AcX
  Wire2.endTransmission(false);
  Wire2.requestFrom(0x68, 6);

  acX = Wire2.read() << 8 | Wire2.read(); //each value needs two registres
  acY = Wire2.read() << 8 | Wire2.read();
  acZ = Wire2.read() << 8 | Wire2.read();

  AccAngleX = atan((acY / 16384.0) / sqrt(pow((acX / 16384.0), 2) + pow((acZ / 16384.0), 2))) * rad_to_deg;
  AccAngleY = atan(-1 * (acX / 16384.0) / sqrt(pow((acY / 16384.0), 2) + pow((acZ / 16384.0), 2))) * rad_to_deg;


  Wire2.beginTransmission(0x68);
  Wire2.write(0x43); //Gyro data first adress
  Wire2.endTransmission(false);
  Wire2.requestFrom(0x68, 4); //Just 4 registers

  gyroX = Wire2.read() << 8 | Wire2.read(); //Once again we shif and sum
  gyroY = Wire2.read() << 8 | Wire2.read();

  GyroAngleX = gyroX / 131.0;
  GyroAngleY = gyroY / 131.0;

  TotalAngleX = 0.98 * (TotalAngleX + GyroAngleX * elapsedTime) + 0.02 * AccAngleX;
  TotalAngleY = 0.98 * (TotalAngleY + GyroAngleY * elapsedTime) + 0.02 * AccAngleY;

  // gPitchDeg =
  tError = TotalAngleY;
  pidP = pGain * tError;

  pidI =0; //pidI +(iGain*tError);
  pidD = dGain * ((tError - befError) / elapsedTime);
  
  pid = pidP + pidI +pidD;
  if (pid < -500) {
    pid = -500;
  }
  if (pid > 500) {
    pid = 500;
  }
  FR = throtle + pid;
  FL = throtle + pid;
  RR = throtle - pid;
  RL = throtle - pid;




  if (FR > 2000)
    FR = 2000;
  if (FL > 2000)
    FL = 2000;
  if (RR > 2000)
    RR = 2000;
  if (RL > 2000)
    RL = 2000;
  if (FR < 1000)
    FR = 1000;
  if (FL < 1000)
    FL = 1000;
  if (RR < 1000)
    RR = 1000;
  if (RL < 1000)
    RL = 1000;


 /* Serial.print("FR ");
  Serial.print(FR);
  Serial.print(" FL ");
  Serial.print(FL);
  Serial.print(" RR ");
  Serial.print(RR);
  Serial.print(" RR ");
  Serial.print(RR);
  Serial.print(" PID ");*/
  Serial.println(pid);
 /* Serial.print(" AngleY ");
  Serial.print(TotalAngleY);
  Serial.print(" AngleX ");
  Serial.print(TotalAngleY);
  Serial.println();*/
  S1.writeMicroseconds(FR);
  S2.writeMicroseconds(FL);
  S3.writeMicroseconds(RR);
  S4.writeMicroseconds(RL);


  befError = tError;
}

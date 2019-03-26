#include <Servo.h>
#include <Wire.h>
TwoWire Wire2(2, I2C_FAST_MODE);

Servo FR, FL, RR, RL;


int E1 = 0;
int E2 = 0;
int E3 = 0;
int E4 = 0;
int loopTimer = 0;
int16_t accX;
int16_t accY;
int16_t accZ;
int16_t gyroRoll;
int16_t gyroPitch;
int16_t gyroYaw;

int32_t accVector;

float rollIn, pitchIn, yawIn;
float accPitch, accRoll;
float pitch, roll, yaw;

float pGain = 1.3;
float iGain = 0;
float dGain = 6 ;
float pidRoll;
float pidPitch;
float pidYaw;
int pidLimit = 300;
bool enginePower = false;
int throttle = 1300;
void setup() {
  Serial.begin(9600);
  timerSetup();
  delay(50);

  Wire2.begin();
  Wire2.beginTransmission(0x68);
  Wire2.endTransmission();
  Serial.println("Calibration");
  gyroSetup();
  gyroCalibration();
  loopTimer = micros();
  Serial.println("");
  Serial.println("");

  FR.attach(PB9);
  FL.attach(PB8);
  RR.attach(PB7);
  RL.attach(PB6);


}

void loop() {
  // put your main code here, to run repeatedly:
  readGyro();
  //   deg/sec
  rollIn = (rollIn * 0.7) + (((float)gyroRoll / 65.5) * 0.3);
  pitchIn = (pitchIn * 0.7) + (((float)gyroPitch / 65.5) * 0.3);
  yawIn = (yawIn * 0.7) + (((float)gyroYaw / 65.5) * 0.3);

  pitch += (float)gyroPitch * 0.0000611;
  roll += (float)gyroRoll * 0.0000611;
  yaw += (float)gyroYaw * 0.0000611;
  if (yaw < 0) yaw += 360;
  else if (yaw >= 360) yaw -= 360;

  pitch -= roll * sin((float)gyroYaw * 0.000001066);
  roll += pitch * sin((float)gyroYaw * 0.000001066);

  accVector = sqrt((accX * accX) + (accY * accY) + (accZ * accZ));
  if (abs(accY) < accVector) {
    accPitch = asin((float)accY / accVector) * 57.296;
  }
  if (abs(accX) < accVector) {
    accRoll = asin((float) accX / accVector) * 57.296;
  }

  pitch = pitch * 0.9996 + accPitch * 0.0004;
  roll = roll * 0.9996 + accRoll * 0.0004;

  pid();
  if (Serial.available() > 0) {
    char c = Serial.read();
    if(c == 'X')
    enginePower = !enginePower;
  }

  if (enginePower) {
    
    E1 = throttle - pidPitch;
    E2 = throttle - pidPitch;
    E3 = throttle + pidPitch;
    E4 = throttle + pidPitch;
    Serial.println(pidPitch);
    if (E1 > 2000) E1 = 2000;
    if (E1 < 1000) E1 = 1000;

    if (E2 > 2000) E2 = 2000;
    if (E2 < 1000) E2 = 1000;

    if (E3 > 2000) E3 = 2000;
    if (E3 < 1000) E3 = 1000;

    if (E4 > 2000) E4 = 2000;
    if (E4 < 1000) E4 = 1000;



    FR.writeMicroseconds(E1);
    FL.writeMicroseconds(E2);
    RR.writeMicroseconds(E3);
    RL.writeMicroseconds(E4);

  } else {
    Serial.println(pidPitch);
    Serial.print("PitchIn ");
    Serial.print(pitchIn);
    Serial.print("| RollIn ");
    Serial.print(rollIn);
    Serial.print("| YawIn ");
    Serial.print(yawIn);
    Serial.print("| Pitch ");
    Serial.print(pitch);
    Serial.print("| Roll ");
    Serial.print(roll);
    Serial.println();
    FR.writeMicroseconds(1000);
    FL.writeMicroseconds(1000);
    RR.writeMicroseconds(1000);
    RL.writeMicroseconds(1000);

  }
  /*Serial.print("PitchIn ");
    Serial.print(pitchIn);
    Serial.print("| RollIn ");
    Serial.print(rollIn);
    Serial.print("| YawIn ");
    Serial.print(yawIn);
    Serial.print("| Pitch ");
    Serial.print(pitch);
    Serial.print("| Roll ");
    Serial.print(roll);
    Serial.println();
  */





}

int16_t pitchOff = 0;
int16_t rollOff = 0;
int16_t yawOff = 0;
bool calibration = false;
void gyroSetup() {
  Wire2.beginTransmission(0x68);
  Wire2.write(0x6B);
  Wire2.write(0x00);
  Wire2.endTransmission();

  Wire2.beginTransmission(0x68);
  Wire2.write(0x1B);
  Wire2.write(0x08);
  Wire2.endTransmission();

  Wire2.beginTransmission(0x68);
  Wire2.write(0x1C);
  Wire2.write(0x10);
  Wire2.endTransmission();

  Wire2.beginTransmission(0x68);
  Wire2.write(0x1A);
  Wire2.write(0x03);
  Wire2.endTransmission();
}

void gyroCalibration() {
  calibration = false;
  for (int i = 0; i < 2000; i++) {
    if(i%100==0) Serial.print(".");
    readGyro();
    rollOff = gyroRoll;
    pitchOff = gyroPitch;
    yawOff = gyroYaw;
    
    delay(4);
  }
  rollOff /= 2000;
  pitchOff /= 2000;
  yawOff /= 2000;
  calibration = true;
}


void readGyro() {
  int16_t temp;
  Wire2.beginTransmission(0x68);
  Wire2.write(0x3B);
  Wire2.endTransmission();
  Wire2.requestFrom(0x68, 14);
  accX = Wire2.read() << 8 | Wire2.read();
  accY = Wire2.read() << 8 | Wire2.read();
  accZ = Wire2.read() << 8 | Wire2.read();
  temp = Wire2.read() << 8 | Wire2.read();
  gyroRoll = Wire2.read() << 8 | Wire2.read();
  gyroPitch = Wire2.read() << 8 | Wire2.read();
  gyroYaw = Wire2.read() << 8 | Wire2.read();

  gyroPitch *= -1;
  gyroYaw *= -1;
  if (calibration) {
    gyroRoll -= rollOff;
    gyroPitch -= pitchOff;
    gyroYaw -= yawOff;
  }
}

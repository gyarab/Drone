bool calibrationDone = false;
void gyroSetup() {
  Wire2.begin();
  Wire2.beginTransmission(0x68);
  Wire2.endTransmission();

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


void readGyro() {
  Wire2.beginTransmission(0x68);
  Wire2.write(0x3B);
  Wire2.endTransmission();
  Wire2.requestFrom(0x68, 14);

  acY = Wire2.read() << 8 | Wire2.read();
  acX = Wire2.read() << 8 | Wire2.read();
  acZ = Wire2.read() << 8 | Wire2.read();
  temp = Wire2.read() << 8 | Wire2.read();
  gyroRoll = Wire2.read() << 8 | Wire2.read();
  gyroPitch = (Wire2.read() << 8 | Wire2.read()) * (-1);
  gyroYaw = (Wire2.read() << 8 | Wire2.read()) * (-1);

  if (calibrationDone) {
    gyroRoll -= rollCal;
    gyroPitch -= pitchCal;
    gyroYaw -= yawCal;
    acX -= acXCal;
    acY -= acYCal;
    acZ -= acZCal;
  }
}

void calibrateGyro() {
  calibrationDone = false;
  for (int i = 0; i < 3000; i++) {
    readGyro();
    rollCal += gyroRoll;
    pitchCal += gyroPitch;
    yawCal += gyroYaw;
    acXCal += acX;
    acYCal += acY;
    acZCal += acZ;
    delay(5);
  }
  rollCal /= 3000;
  pitchCal /= 3000;
  yawCal /= 3000;
  
  Serial.print("CalRo : ");
  Serial.print(rollCal);
  Serial.print(" || ");
  Serial.print("CalPi : ");
  Serial.print(pitchCal);
  Serial.print(" || ");
  Serial.print("CalYa : ");
  Serial.print(yawCal);
  Serial.println();
  Serial.print("CalX : ");
  Serial.print(acXCal);
  Serial.print(" || ");
  Serial.print("CalY : ");
  Serial.print(acYCal);
  Serial.print(" || ");
  Serial.print("CalZ : ");
  Serial.print(acZCal);
  
  
  calibrationDone = true;
}

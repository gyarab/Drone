float lastRerror = 0;
float lastPerror = 0;
void pid() {
  //roll
  float error = rollIn;
  float P = pGain * error;
  float I = iGain;
  float D = dGain * (error - lastRerror);
  lastRerror = error;

  pidRoll = P + I + D;
  if (pidRoll > pidLimit) pidRoll = pidLimit;
  else if (pidRoll < pidLimit) pidRoll = -1 * pidLimit;

  //pitch
  error = pitchIn;
  P = pGain * error;
  I = iGain;
  D = dGain * (error - lastPerror);
  Serial.print("P ");
  Serial.print(P);
  Serial.print(" I ");
  Serial.print(I);
  Serial.print( " D ");
  Serial.println(D);
  lastPerror = error;

  pidPitch = P + I + D;
  if (pidPitch > pidLimit) pidPitch = pidLimit;
  else if (pidPitch < -1*pidLimit) pidPitch = -1 * pidLimit;

}

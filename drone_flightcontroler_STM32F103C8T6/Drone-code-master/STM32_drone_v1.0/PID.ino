/*void pid() {
  //Roll calculations
  pid_error_temp = gyro_roll_input - pid_roll_setpoint;
  pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
  if (pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
  else if (pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

  pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
  if (pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
  else if (pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

  pid_last_roll_d_error = pid_error_temp;
  }*/
/*

void pidP() {
  //pitch
  tError = TotalAngleY - 0;
  iPitch +=0;//iPitchGain * tError;
  pPitch = pPitchGain * tError;
  dPitch = dPitchGain * ((tError - pitchEBef)/eTime);

  if (iPitch > maxPitch) {
    iPitch = maxPitch;
  } else if (iPitch < -maxPitch) {
    iPitch = -maxPitch;
  }

  pidPitch = pPitch + iPitch + dPitch;
  
  if (pidPitch > maxPitch) {
    pidPitch = maxPitch;
  } else if (pidPitch < -maxPitch) {
    pidPitch = -maxPitch;
  }

  
  
}*/
/*
float countError(float pitchInternal){
  if (pitchInternal > 5)errorValue = -1;    //omezeni maximalniho naklonu
  else if (pitchInternal < 5)errorInternal = 1;
  else errorInternal = map(pitchInternal,-5.00,5.00,1.000,-1.000);
  return errorInternal;
}
//  uhel -5 tupnu, dron na naklopen dozadu errorValue je jedna, pokus o naklon dopredu je maximalni
//  PROPORCNI SLOZKA     pitch_p = parametr * setPoint
float pitch; //values in range from -50 to 50.....naklon dozadu kladna hodnota
float errorValue;  //jaká je požadovaná hodnota
float parametr_I = 100; //od oka tipnuto
float pitch_P = parametr*countError(); //navisime hodnotu motorů...kladne dopredu, zaporne dozadu
//  INTERGRACNI SLOZKA    pitch_i = parametr + celkova odchylka/celkovy cas
float parametr_I = 0.1;//jak jinak nez tip :)
float sumError += errorValue();
float pitch_I = sumError/millis();//potreba poresit preteceni obou hodnot
//  DERIVACNI SLOZKA      
float parametr_D = 20; //tip
float firstTime = millis()
float firstError = countError();
delay(5);//fakt hnusny reseni, ale mega kratky prozatim a nebude to tak rychly alespon
float secondError = countError();//zmeri 
float timeDifference = millis() - firstTime;
float pitch_D = parametr_D * (firstError - secondError)/timeDifference;
*/

void setup() {
  initCommunication();//zahajuje komunikaci s ovladacem
}

void loop() {
  requestForData();// DULEZITE updatuje data z ovladace
  getRotation();//vraci rotaci -100 - 100 100 ccw
  getSideMovement();//vraci pohyb dostrany -100 - 100 100 doprava
  getForwardMovement();//vraci naklon dopredu ci dozadu -100 - 100 100 =´dopredu
  getPower();//vraci výkon na motorech 0 - 100
  delay(1);

}



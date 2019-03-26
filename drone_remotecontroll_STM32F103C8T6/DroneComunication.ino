#define HC12 Serial2
const char startString[] = "START";
const char numbers[] = "0123456789";
int data[13];
uint32_t lastReqMillis = 0;
struct ControllerData {
  int rotation;
  int sideMovement;
  int forwardMovement;
  int power;
};
ControllerData actualData = {0, 0, 0, 0};


void initCommunication() {
  Serial2.begin(115200);
  bool comunicationReady = true;
  for (;;) {
    comunicationReady = true;
    for (int i = 0; HC12.available() != 0; i++) {
      if (startString[i] != HC12.read()) {
        comunicationReady = false;
      }
    }
    if (comunicationReady)break;
    delay(10);
  }
  for (int i = 0; i < 100; i++) {
    HC12.print("READY");
    delay(10);
  }
}
void requestForData() {
  if (millis() - lastReqMillis >= 20) {
    lastReqMillis = millis();
    HC12.print("REQ");
    parseData();
  }
}
void parseData() {
  
  for(int i = 0;HC12.available()!=0;i++){
    data[i] =parseIntFromChar( HC12.read());
  }
  actualData.rotation = getParsedNumber(data[0], data[1], data[2]);
  actualData.sideMovement =  getParsedNumber(data[3], data[4], data[5]);
  actualData.forwardMovement = getParsedNumber(data[6], data[7], data[8]);
  actualData.power = getParsedNumber(data[9], data[10], data[11]);

}
void sendDataToController() {//dunno co posilat z5

}







int getParsedNumber(int first, int sec, int third) {
  
  return first * 100 + sec * 10 + third-200;
}

int parseIntFromChar(char number) {
  for (int i = 0; i < 10; i++)if (number == numbers[i])return i;
}

//gettery -> nobody cares
int getRotation() {
  return actualData.rotation;
}
int getSideMovement() {
  return actualData.sideMovement;
}
int getForwardMovement() {
  return actualData.forwardMovement;
}
int getPower() {
  return actualData.power;
}


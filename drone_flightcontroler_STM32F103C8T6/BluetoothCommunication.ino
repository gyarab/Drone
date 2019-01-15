#include <SoftwareSerial.h>
#define RX 11
#define TX 12

SoftwareSerial bt(TX, RX);
char inputPassword [10] = "6:}k712m07";
char engineCommand [6];
int engineCommandCounter = 0;//max 6 1=1000
bool readyForCommunication = false;//bool urcuje, jestli je pripojen telefon
int passwordCounter = 0;
char password[10];
char numbers[10] = "0123456789";


void initBluetooth() {
  bt.begin(115200);//rychlost cteni, asi budes chtit zmenit
  Serial.begin(2000000);
}

void checkBluetooth() { //kontroluje zda-li neni na bt nova informace
  if (bt.available() == 0)return; //konci pokud je bt prazdny
  
  if (readyForCommunication) { //pokud je pripojen telefon
    while (bt.available() != 0) {
      engineCommand[engineCommandCounter++] = (char)bt.read();//cteni z bt
      if (engineCommandCounter == 6) { //precteni cele zpravy
        int power = 1;
        for (int i = 2; i < 6; i++) { //vyparsovani poslaneho cisla
          power += numberCharToInt(engineCommand[i]) * pow(10, 5 - i);
        }
        setPowerValue(engineCommand[0], power);
        engineCommandCounter = 0;
      }
    }

  } else { //pokud neni -> otestuje se
    for(;passwordCounter<10&&bt.available()!=0;passwordCounter++)password[passwordCounter] = (char)bt.read(); 
    if(passwordCounter==10)readyForCommunication = isPhoneConnected();
    Serial.println(readyForCommunication);
    
    
  }
  
}

bool isPhoneConnected() {
  for(int i = 0;i<10;i++){
    Serial.print(password[i]);
    Serial.print(" = ");
    Serial.println(inputPassword[i]);
  }
  for (int i = 0; i < 10; i++) {
    if (password[i] != inputPassword[i]) return false;
  }
  
  bt.print("+9/On}2pao");
  return true;
}
int numberCharToInt(char c) {
  for (int i = 0; i < 10; i++) {
    if (numbers[i] == c)return i;
  }
  return -1;
}
void setPowerValue(char motorNumber, int power) {
  switch (motorNumber) {
    case '1':
      firstEngine = power;
      break;
    case '2':
      secondEngine = power;
      break;
    case '3':
      thirdEngine = power;
      break;
    case '4':
      fourthEngine = power;
      break;
    case '5':
      firstEngine = power;
      secondEngine = power;
      thirdEngine = power;
      fourthEngine = power;
      break;
    default:
      break;
  }
}





int firstEngine = 1000;
int secondEngine = 1000;
int thirdEngine = 1000;
int fourthEngine = 1000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("START");
initBluetooth();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkBluetooth();//aktualizuje každý cyklus čísla na motorech
  Serial.println("EN1 " + firstEngine);
  Serial.println("EN2 " + secondEngine);
  Serial.println("EN3 " + thirdEngine);
  Serial.println("EN4 " + fourthEngine);
  delay(1);
}



//#include <SoftwareSerial.h>
//#define RX 11
//#define TX 12
//SoftwareSerial Serial1(TX, RX);




char inputPassword [11] = "6:}k712m07";
char engineCommand [6];
int engineCommandCounter = 0;//max 6 1=1000
bool readyForCommunication = false;//bool urcuje, jestli je pripojen telefon
int passwordCounter = -1;
int password[11];
char numbers[11] = "0123456789";


void initBluetooth() {
  Serial1.begin(115200);//rychlost cteni, asi budes chtit zmenit
  
}

void checkBluetooth() { //kontroluje zda-li neni na Serial1 nova informace
  if (Serial1.available() == 0) return; //konci pokud je Serial1 prazdny
  if (readyForCommunication) { //pokud je pripojen telefon
    while (Serial1.available() != 0) {
      engineCommand[engineCommandCounter++] = (char)Serial1.read();//cteni z Serial1
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
    readyForCommunication = isPhoneConnected();
  }
}

bool isPhoneConnected() {

  while (Serial1.available() != 0) {
    int readed = Serial1.read();
    if (readed == 11) {
      passwordCounter = 0;
    } else if (passwordCounter >= 0) {
      password[passwordCounter++] = readed;
      if (passwordCounter == 11) {
        bool isCorrect = true;
        for (int i = 0; i < 11; i++) {
          if (password[i] != inputPassword[i]) {
            isCorrect = false;
            break;
          }
        }
        if (isCorrect)Serial1.print("+9/On}2pao");
        return isCorrect;
      }
    }
  }

  return false;
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

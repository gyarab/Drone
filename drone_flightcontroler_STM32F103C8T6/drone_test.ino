#include <Servo.h>

// Knihovna I2Cdev využívá knihovnu Wire
#include "Wire.h"
#include "I2Cdev.h"
#include <MPU6050_6Axis_MotionApps20.h>

// Výchozí I2C adresa MPU-6050 je 0x68, ale pokud máte jinou, zde ji můžete uvést
//AD0 low = 0x68;
// AD0 high = 0x69
MPU6050 mpu;
TwoWire WIRE2(2, I2C_FAST_MODE);
#define Wire WIRE2
#define LED_PIN 13 // číslo pinu s integrovanou LED diodou

bool dmpReady = false;  // když je DPM připraveno, obsahuje true
uint8_t mpuIntStatus;   // stav externího přerušení z DPM
uint8_t devStatus;      // stav poslední operace
uint16_t packetSize;    // velikost paketu z DPM
uint16_t fifoCount;     // počet bytů ve FIFO zásobníku
uint8_t fifoBuffer[64]; // FIFO zásobník

// proměnné důležité pro výpočet
Quaternion q;           // [w, x, y, z]         kvaternion
VectorFloat gravity;    // [x, y, z]            vektor setrvačnosti
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll úhly

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = true;     // true pokud DMP vyvolalo přerušení
void dmpDataReady() {
  mpuInterrupt = true;
}

//MOTORS
Servo S1, S2, S3, S4;
unsigned int FR = 1000;
unsigned int FL = 1000;
unsigned int RR = 1000;
unsigned int RL = 1000;

//PID
float pidP = 0;
float pidI = 0;
float pidD = 0;
float tError = 0;
float befError = 0;
float pid = 0;
float pGain = 2.65;
float iGain = 0.006;
float dGain = 0.90;
int maxPid = 400;

float  eTime, timePrev;
double elapsedTime;
double runtime;
String mess = "";
int throtle = 1300;
bool on = false;
bool pidset = false;
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  //pinMode(LED_PIN, OUTPUT);


  S1.attach(PB9); // Front - Right
  S2.attach(PB8); // Front - Left
  S3.attach(PB7); // Rear - Right
  S4.attach(PB6); // Rear - Left


  S1.writeMicroseconds(FR);
  S2.writeMicroseconds(FL);
  S3.writeMicroseconds(RR);
  S4.writeMicroseconds(RL);
  // připojíme se na I2C sbCěrnici
  Wire.begin();

  // inicializujeme UART
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately

  // inicializujeme MPU-6050
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // ověříme připojení k MPU-6050
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // incializujeme DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // ujistíme se, že funguje
  if (devStatus == 0) {
    // zapneme DMP
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // externí přerušení Arduina nabindujeme na funkci dmpDataReady
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // zjistíme si, jak velké pakety DMP vrací
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // Když dojde k chybě, tak:
    // 1 = selhala úvodní komunikace s DMP
    // 2 = selhala aktualizace nastavení DMP
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
  while(Serial.available()==0){}
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '#') {
      beg =  true;
    }
    if (((c != '|' && c != 'X')) && beg ) {
      mess += c;
    } else if (c == '|' && beg) {
      d = (float)(strtol(&mess[0], NULL, 16));
      d /= (float)1000.0;
      mess = "";

    } else if (c == 'X' && beg) {
      d = (float)(strtol(&mess[0], NULL, 16));
      d /= (float)1000.0;
      beg = false;
      Serial.println(d, 4);
      mess = "";
    }
  }


  digitalWrite(LED_PIN, LOW);
  delay(5000);
  eTime = millis();
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {


  // pokud není DMP připravené, nebudeme dělat nic
  if (!dmpReady) return;

  // zde provádíme náš kód, cyklus ověřuje, zda nemá DMP připravena nějaká data
  while (!mpuInterrupt && fifoCount < packetSize) {
    Serial.print(".");
    //
  }

  // resetujeme proměnnou informující o přerušení vyvolané z DMP a získáme INT_STATUS byte
  //  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // získáme velikost FIFO zásobníku
  fifoCount = mpu.getFIFOCount();

  // zjistíme, zda nedošlo k přetečené zásobníku
  // pokud k němu dojde, je třeba optimalizovat kód v cyklu výše,
  // případně přerušit provádění mezi delšími výpočty, je-li to třeba
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // vyčistíme zásobník
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // pokud je vše v pořádku, zpracujeme data z DMP
  } else if (mpuIntStatus & 0x02) {
    timePrev = eTime;
    eTime = (float)millis();
    elapsedTime = (float)((float)((float)eTime - (float)timePrev) / 1000.0);
    /* Serial.print(timePrev);
      Serial.print("   ");
      Serial.print(eTime);
      Serial.println("   ");*/
    // čekání na správnou délku dat
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // přečteme paket ze zásobníku
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // pokud je v zásobníku víc než jeden paket, tímto zajistíme, že bude přečten v dalším cyklu
    fifoCount -= packetSize;

    // naplnění proměnných s vypočítanými hodnotami
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    //Výpis YAW/PITCH/ROLL
    /*Serial.print("ypr\t");
      Serial.print(ypr[0] * 180 / M_PI);
      Serial.print("\t");
      Serial.print(ypr[1] * 180 / M_PI);
      Serial.print("\t");*/
    // Serial.println(ypr[2] * 180 / M_PI);

    tError = ((ypr[1] * 180) / M_PI);
    if (on) {
      pidP = pGain * tError;

      if (-3.0 < tError < 3.0) {
        pidI = (pidI + (iGain * tError));
      }

      pidD = dGain * ((tError - befError) / elapsedTime);
      /*Serial.print(pidD);
        Serial.print(" ");
        Serial.print(tError);
        Serial.print(" ");
        Serial.print(befError);
        Serial.print(" ");
        Serial.print(elapsedTime);*/



      pid = pidP + pidI + pidD;

      if (pid > maxPid) {
        pid = maxPid;
      } else if (pid < -maxPid) {
        pid = -maxPid;
      }
      if (on) {
        Serial.print(tError);
        Serial.print("|");
        Serial.print(pidP);
        Serial.print("|");
        Serial.print(pidD);
        Serial.print("|");
        Serial.print(pidI);
        Serial.print("|");
        Serial.print((millis() - runtime));
        Serial.println();
      }
      FR = throtle - pid;
      FL = throtle - pid;
      RR = throtle + pid;
      RL = throtle + pid;
    }
    /* Serial.print(FR);
      Serial.print(" ");
      Serial.print(FL);
      Serial.print(" ");
      Serial.print(RR);
      Serial.print(" ");
      Serial.println(RL);
      Serial.print("pid ");
      Serial.println(pid);
      Serial.print("Error ");
      Serial.println(tError);*/

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

    if (Serial.read() == 'X') {

      on = !on;
      runtime = millis();
    }
    if (!on) {
      on = false;
      S1.writeMicroseconds(1000);
      S2.writeMicroseconds(1000);
      S3.writeMicroseconds(1000);
      S4.writeMicroseconds(1000);
    } else {
      S1.writeMicroseconds(FR);
      S2.writeMicroseconds(FL);
      S3.writeMicroseconds(RR);
      S4.writeMicroseconds(RL);
    }

    befError = tError;

  }
}

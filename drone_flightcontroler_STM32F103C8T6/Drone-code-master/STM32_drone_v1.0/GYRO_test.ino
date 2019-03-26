// Knihovna I2Cdev využívá knihovnu Wire
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Výchozí I2C adresa MPU-6050 je 0x68, ale pokud máte jinou, zde ji můžete uvést
// AD0 low = 0x68
// AD0 high = 0x69
MPU6050 mpu;

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

volatile bool mpuInterrupt = false;     // true pokud DMP vyvolalo přerušení
void dmpDataReady() {
mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
pinMode(LED_PIN, OUTPUT);

// připojíme se na I2C sběrnici
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

digitalWrite(LED_PIN, LOW);
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
// pokud není DMP připravené, nebudeme dělat nic
if (!dmpReady) return;

// zde provádíme náš kód, cyklus ověřuje, zda nemá DMP připravena nějaká data
while (!mpuInterrupt && fifoCount < packetSize) {
    //
}

// resetujeme proměnnou informující o přerušení vyvolané z DMP a získáme INT_STATUS byte
mpuInterrupt = false;
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
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180/M_PI);
    Serial.print("\t");
    Serial.println(ypr[2] * 180/M_PI);
}
} 

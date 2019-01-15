/*#include <SoftSerialSTM32.h>





  SoftSerialSTM32 bt(PA3,PA2);*/
String command = ""; // Stores response from HC-06
void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);       //monitor
  Serial1.begin(115200);      //bluetooth

  Serial.print("AT      ");
  for (int i = 0; i < 5; i++) {
    Serial1.print("AT");
    delay(1000);
  }  //PING
  if (Serial1.available()) {
    while (Serial1.available()) { // While there is more to be read, keep reading.
      delay(3);
      char c = Serial1.read();
      command += c;
    }
  }
  delay(2000);
  Serial.println(command);
  command = ""; // No repeats

  Serial.print("AT+NAMEDrone      ");
  Serial1.print("AT+NAMEDrone");        //CHANGE NAME
  if (Serial1.available()) {
    while (Serial1.available()) { // While there is more to be read, keep reading.
      delay(3);
      command += (char)Serial1.read();
    }
  }
  delay(2000);
  Serial.println(command);
  command = ""; // No repeats

  Serial.println("AT+PIN1234");
  Serial1.print("AT+PIN1234");        //CHANGE PASSWORD
  if (Serial1.available()) {
    while (Serial1.available()) { // While there is more to be read, keep reading.
      delay(3);
      command += (char)Serial1.read();
    }
  }
  delay(2000);
  Serial.println(command);
  command = ""; // No repeats

  Serial.print("AT+BAUD8      ");
  Serial1.print("AT+BAUD8");               //CHANGE SPEED TO 115K
  if (Serial1.available()) {
    while (Serial1.available()) { // While there is more to be read, keep reading.
      command += (char)Serial1.read();
    }
  }
  delay(2000);
  Serial.println(command);
}

void loop()

{

  if (Serial1.available()) {

    Serial.write(Serial1.read());
  }
  if (Serial.available()) {

    Serial1.write(Serial.read());
  }
}

int firstEngine = 1000;
int secondEngine = 1000;
int thirdEngine = 1000;
int fourthEngine = 1000;


void setup() {
  // put your setup code here, to run once:
initBluetooth();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkBluetooth();//aktualizuje každý cyklus čísla na motorech
}

#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Wire.h>
#include <SoftWire.h>
#include <Wire_slave.h>

#include <U8glib.h>


#define RX_bt PB6
#define TX_bt PB7
#define SCL_led PB8
#define SDA_led PB9
#define B1_TurnRight PA15
#define B2_TurnLeft PB3
#define B3 PA11
#define B4 PA10
#define B5 PA9
#define MOSI_mk1 PB15   //spi slave data input
#define CSN_mk1 PB11    //spi chip select
#define CE_mk1 PB10     //rx or tx mode
#define SCK_mk1 PB13    //spi clock
#define MISO_mk1 PB14   //spi slave data output
#define Y_power PA4
#define Y_move PA6
#define X_move PA7


U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);
RF24 communication(CSN_mk1,CE_mk1);
const byte adress[][6] =("00001","00002");  //const byte adress[][6] = ("00001","00002"); //drone

boolean turnRight = false;
boolean turnLeft = false;
uint8_t power;
uint8_t x_fly; //-512 až 512
uint8_t y_fly; //-512 až 512
uint8_t drone_bat;//map values from 0 to 99
uint8_t controller_bat;//map values from 0 to 99

void setup() {
  // put your setup code here, to run once:
  pinMode(Y_power, INPUT);
  pinMode(Y_move, INPUT);
  pinMode(X_move, INPUT);
  communication.begin();                      //communication.begin();
  communication.openWritingPipe(adress[1]);   //communication.openReadingPipe(1,adress[0]);
  communication.openReadingPipe(1,adress[0]); //communication.openWritingPipe(adress[1]);
  communication.setPALevel(RF_PA_MED);        //communication.setPALevel(RF_PA_MAX); //better distance than LOW, dont work for close distance??
}

void loop() {
  // put your main code here, to run repeatedly:
  DisplayPrint();
  DroneCommunication();
}

void DisplayWrite(){
  display.setFont(u8g_font_6x13);
  display.setPrintPos(0,20);
  display.print("Drone bat: " + drone_bat + "%");
  display.setPrintPos(0,40);
  display.print("Controller bat: " + controller_bat + "%");
}

void DisplayPrint(){
  display.firstPage();
  do{
    DisplayWrite();
  }while(display.nextPage());
}

void MoveInput(){
  power = map(analogRead(Y_power),0,1024,0,255);
  x_fly = map(analogRead(X_move),0,1024,0,255);
  y_fly = map(analogRead(Y_move),0,1024,0,255);
  if(digitalRead(B1_TurnRight) == HIGH){
    turnRight = true;
  }else{
    turnRight = false;
  }
  if(digitalRead(B2_TurnLeft) == HIGH){
    turnLeft = true;
  }else{
    turnLeft = false;  
  }
}

void DroneCommunication(){
  MoveInput();
  communication.stopListening();
  communication.write(x_fly,sizeof(x_fly));
  communication.write(y_fly,sizeof(y_fly));
  communication.write(power,sizeof(power));
  communication.write(turnRight,sizeof(turnRight));
  communication.write(turnLeft,sizeof(turnLeft));
  communication.startListening();
  /*
   * Getting data from drone
   */
}

#include <Servo.h>



Servo esc; //Creating a servo class with name as esc

void setup()

{

esc.attach(9); //Specify the esc signal pin,Here as D8

esc.writeMicroseconds(1000); //initialize the signal to 1000
delay(10000);
Serial.begin(9600);

}

void loop()

{

int val; //Creating a variable val

//val= ; //Read input from analog pin a0 and store in val

//val= map(val, 0, 1023,1000,2000); //mapping val to minimum and maximum(Change if needed)
esc.writeMicroseconds(2000); //using val as the signal to esc
/*for(int i =0;i <10;i++){
esc.writeMicroseconds(1000+(i*100));
delay(200);
}
for(int i =10;i >0;i--){
esc.writeMicroseconds(1000+(i*100));
delay(200);
}
delay(10000);*/
}

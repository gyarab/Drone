String mess = "";
float d[3];
bool beg = false;
float p = 0.0;
float i = 0.0;

int count = 0;
double c = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 
  //beg=true;
  while(true){
    if(Serial.available()>0){
    char c = Serial.read();
    if (c == '#') {
      beg =  true;
    }
    if (((c != '|' && c != 'X' && c!='#')) && beg ) {
      mess += c;
    } else if (c == '|' && beg) {
      d[count] = (float)(strtol(&mess[0], NULL, 16));
      d[count] /= (float)1000.0;
      count++;
      mess = "";

    } else if (c == 'X' && beg) {
      d[count] = (float)(strtol(&mess[0], NULL, 16));
      d[count] /= (float)1000.0;
      beg = false;
      //Serial.println(d, 4);
      mess = "";
      break;
    }
  }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  
    String s= "";
    s+= sin(c);
    c+=0.1;
   
   
    Serial.println(s);
    

    
  /*for(float f:d){
    Serial.println(f,3);
    }

    delay(500);
 /* Serial.println(p,3);
  Serial.println(d,3);*/

  



}

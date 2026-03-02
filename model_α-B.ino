int LB = 8;
int counter = 0;
char val = '0';

void setup(){
  Serial.begin(9600);
  
  pinMode(LB,OUTPUT);
}

void loop(){
  while(Serial.available() == 0){}
  val = Serial.read();
  if (val == '1'){
    counter = 2;
    while (counter > 0){
      digitalWrite(LB,HIGH);
      delay (1000);
      digitalWrite(LB,LOW);
      delay (1000);
      counter -= 1;
    }
  }
  else if (val == '2'){
    counter = 3;
    while (counter > 0){
      digitalWrite(LB,HIGH);
      delay (1000);
      digitalWrite(LB,LOW);
      delay (1000);
      counter -= 1;
    }
  }
  else{//エラー時
    digitalWrite(LB,HIGH);
    delay (2000);
    digitalWrite(LB,LOW);
  }
  while(Serial.available() != 0){Serial.read();}
  Serial.print('F');
  digitalWrite(LB,LOW);
}

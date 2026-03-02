int b1 = 3;
int b2 = 2;
int LA = 8;

char SA = 'W';//待機中かどうかの状態を表す

void setup(){
  Serial.begin(9600);//シリアル通信開始
  
  pinMode(b1,INPUT_PULLUP);//各ピンの役割決め
  pinMode(b2,INPUT_PULLUP);
  pinMode(LA,OUTPUT);
}

void loop(){
  if (SA == 'W'){//待機中なら
    digitalWrite(LA,HIGH);
    if (digitalRead(b1) == LOW){//b1が押された
      digitalWrite(LA,LOW);
      SA = '1';
    }
    else if (digitalRead(b2) == LOW){//b2が押された
      digitalWrite(LA,LOW);
      SA = '2';
    }
  }
  else{
    Serial.print(SA);
    while(Serial.available() == 0){}
    while(Serial.available() != 0){Serial.read();}
    SA = 'W';
  }
}

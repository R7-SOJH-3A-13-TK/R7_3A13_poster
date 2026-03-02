#include <Servo.h>
Servo myservo;

int LED = 8;
int moto = 2;
char val = 'N';

void setup(){
  Serial.begin(9600);
  myservo.attach(moto);//2pinをサーボ用に使用
  myservo.write(0);
  pinMode(LED,OUTPUT);
}
void loop(){
  while(Serial.available() == 0){}
  val = Serial.read();
  if (val != 'R'){
    digitalWrite(LED,HIGH);
    if (val != 'L'){
      if (val == '1'){
        myservo.write(120);//正三角形
        delay(1500);
        myservo.write(0);
        delay(1500);
      }else if (val == '2'){
        myservo.write(60);//正六角形
        delay(1500);
        myservo.write(0);
        delay(1500);
      }else if (val == '3'){
        myservo.write(144);//星型
        delay(1500);
        myservo.write(0);
        delay(1500);
      }
      while (Serial.available() != 0){ Serial.read(); }
      Serial.print('F');//処理終了を伝える。Finishの意
    }
  }
  else{//リセット
    digitalWrite(LED,LOW);
  }
}

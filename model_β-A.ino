#include <Stepper.h>
const int STEPS_PER_REV = 2048;//一回転あたりのステップ数
Stepper motor(STEPS_PER_REV, 10, 12, 11, 13);//10~13DPをIN1~4に対応させる

int b1 = 2;//DPの番号をわかりやすくするため。
int b2 = 3;//b1→正三角形、b2→正六角形、b3→星型
int b3 = 4;
int LED = 8;//通信確認用LED
int num = 0;
char val ='N';//nullの意

void setup(){
  Serial.begin(9600);//シリアル通信開始

  motor.setSpeed(10);  // 回転速度（rpm）　15以下に抑えたほうがよい
  
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  pinMode(b3,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
}
void loop(){
  if (val == 'N'){//待機中なら
    digitalWrite(LED,HIGH);
    if (digitalRead(b1) == LOW){//b1が押された
      digitalWrite(LED,LOW);
      val = '1';
      num = 3;//3回伸ばす
    }
    else  if (digitalRead(b2) == LOW){//b2が...以下略
      digitalWrite(LED,LOW);
      val = '2';
      num = 6;//6回伸ばす
    }
    else  if (digitalRead(b3) == LOW){
      digitalWrite(LED,LOW);
      val = '3';
      num = 5;//5回伸ばす
    }
  }
  else{//ボタンが押された
    digitalWrite(LED,LOW);
    Serial.print('L');//通信が成功していれば、相手側のArdのLEDが点灯するはず
    while(num >= 1){
      motor.step(6144);//3回転(1回転2048*3)
      delay(1000);//1秒間待機。SMが回り切るのを待つ
      Serial.print(val);
      while (Serial.available() == 0){}//待機
      if (Serial.available() != 0){ Serial.read(); }//availableの後処理
      num -= 1;
    }
    val = 'N';
  }
}

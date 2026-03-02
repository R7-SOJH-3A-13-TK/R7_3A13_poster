#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);//液晶関数追加

const char words[] = "abcdefghijklmnopqrstuvwxyz ";//文字表
const int L = 8;
const int C = 9;
const int R = 10;
const int D = 11;
const int S = 12;
//ボタン入力のピンを決める。Rはright、Cはcenter、Lはleft、Dはdelate、Sはsendの略.
const int CT1 = 25;//左右長押し移動開始まで
const int CT2 = 10;//左右長押し移動の間隔
const int CT3 = 30;//入力中の文字を表すアニメーションの間隔
const int CT4 = 250;//文字送りアニメーションの間隔(ms)
const int CT5 = 50;//次のメッセージを送ることができるまでの間隔
const int CT6 = 40;//早すぎる送信ペナルティ

int Pushed = CT1;//長押しで、移動できるようにします。そのクールタイムです
int button = 0;//左ボタンは-1、右ボタンは+1になる
int select = 0;//何を選択しているか
int cursorAnime = CT3 * 2;//入力文字アニメーション管理
int CD = CT5;//この状態だと次のメッセージが送れる

char buffer1[32] = "";//選んだものを一時的に保存しする
char buffer2[32] = "";//受信用
char DW;//DsiplayingWordsの略。

void setup(){

  Serial.begin(9600);//シリアル通信開始
  
  lcd.init();
  lcd.backlight();//液晶初期設定
  
  pinMode(R,INPUT_PULLUP);
  pinMode(C,INPUT_PULLUP);
  pinMode(L,INPUT_PULLUP);
  pinMode(D,INPUT_PULLUP);
  pinMode(S,INPUT_PULLUP);//ボタン設定
  
}

void loop(){

  if(digitalRead(S) == LOW){//sentボタンが押されたなら
    if(CD == CT5){//送れる状態
      CD = 0;//次の送信のクールタイム
      int len = strlen(buffer1);
      if (len < 1){//文字数が0だった場合、不具合が生じる可能性があるため、スペースを入れる。
        buffer1[len] = ' ';//バグ防止
        buffer1[len + 1] = '\0';
        len += 1;
      }
      Serial.print(buffer1);
      //for(最初に実行;条件;ループ終わりに毎回実行)
      for(int i = 0; i < 16; i++){//アニメーション。16はlcdの横幅
        lcd.setCursor(0,0);
        for(int I = i; I > 0; I--){//iの数だけ、空白を設置
          lcd.print(" ");
        }
        lcd.print(buffer1);
        for(int I = 16-i; I > 0; I--){//iの数だけ、空白を設置
          lcd.print(" ");
        }
        delay(CT4);
      }
      while(len >= 1){//buffer1リセット
        buffer1[len - 1] = '\0';//終端文字で上書き
        len -= 1;
      }
      cursorAnime = CT3 * 2;
    }
    else{//送れない状態
      int wait = CT6;
      while(wait > 0 && digitalRead(S) == LOW){
        wait -= 1;
        if(CD < CT5){
          CD  += 1;
        }
        lcd.setCursor(0,0);
        lcd.print("Stop press send");
        lcd.setCursor(0,1);
        lcd.print("button ! ");
        lcd.print(CT5-CD);
        delay(50);
      }
    }
  }
  else if (Serial.available() != 0){//受信したら
    int len = strlen(buffer2);
    lcd.setCursor(0, 1);//左がx、右がy
    lcd.print("RECEIVED!");
    while(len >= 1){//buffer2リセット
      buffer2[len - 1] = '\0';
      len -= 1;
    }
    len = 0;
    while(Serial.available() != 0 && len < 17){
      buffer2[len] = Serial.read();//1文字追加
      buffer2[len + 1] = '\0';//終端文字設定
      len += 1;
    }
    while(Serial.available() != 0){
      Serial.read();
    }
    delay(1000);//1秒間、通信成功を伝える
    for(int i = 16; i > 0; i--){//アニメーション。16はlcdの横幅
      lcd.setCursor(i-1,1);
      lcd.print(buffer2);
      for(int I = 16-i; I > 0; I--){//iの数だけ、空白を設置
        lcd.print(" ");
      }
      delay(CT4);
    }
  }
  else{
    if(digitalRead(R) == LOW){//rightボタン
      if(button != 1 ){
        button = 1;
        Pushed = CT1;
      }
    }
    else{
      if(button == 1){
        button = 0;//右ボタンが押されていたかつ右ボタンが押されていない→リセット
      }
    }
    
    if(digitalRead(L) == LOW){//leftボタン
      if(button != -1 ){
        button -= 1;//左だけなら、-1。左も右も押しているなら0になる
        Pushed = CT1;
      }
    }
    else{
      if(button == -1){
        button = 0;
        Pushed = CT1;
      }
    }
    
    if(button != 0){//左右どちらかのボタンのみが有効なら
      if(Pushed == CT1 || Pushed == CT2){
        select += button;
      }
      cursorAnime = CT3 * 2;
      Pushed -= 1;
      if(Pushed <= 0){
        Pushed = CT2;
      }
    }

    int bLen = strlen(buffer1);
    if (digitalRead(C) == LOW && bLen < 16) {//追加処理
      buffer1[bLen] = words[select];
      buffer1[bLen + 1] = '\0';
      cursorAnime = CT3 * 2;
      while (digitalRead(C) == LOW) {}//ボタンを離すまで待つ
    }
    
    if (digitalRead(D) == LOW && bLen > 0) {//削除処理
      buffer1[bLen - 1] = '\0';
      cursorAnime = CT3;
      while (digitalRead(D) == LOW) {}
    }
    
    if(CD < CT5){
      CD  += 1;
    }
    
    cursorAnime -= 1;
    cursorAnime = (cursorAnime + CT3 * 2) % (CT3 * 2);
    int wLen = strlen(words);
    select = (select + wLen) % wLen;//0~27までの数に収める
    DW = words[select];
    lcd.setCursor(0, 0);
    lcd.print(buffer1);
    if(cursorAnime >= CT3){
      lcd.print(DW);
    }
    else{
      lcd.print("I");
    }
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(buffer2);
    lcd.print("                ");
  }

}



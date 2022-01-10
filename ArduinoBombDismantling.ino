int tDelay = 1;   // 1ms遅延

// ピン設定
int rclkPin = 11;   // (11) ST_CP [RCLK] on 74HC595
int srclkPin = 9;   // (9)  SH_CP [SRCLK] on 74HC595
int dsPin = 12;     // (12) DS [SER] on 74HC595
int dig1 = 1;       // DIG1を1番ピンに割り当て
int dig2 = 2;       // DIG2を2番ピンに割り当て
int dig3 = 3;       // DIG3を3番ピンに割り当て
int dig4 = 4;       // DIG4を4番ピンに割り当て
int buzzer = 6;     // ブザーを6番ピンに割り当て
int red = 7;        // 赤い線を7番ピンに割り当て
int blue = 8;       // 青い線を8番ピンに割り当て

// 時間
long timeNow;           //現在時刻
long timeStart;         //タイマー開始時刻
int timeDisp = 9999; //表示時刻
int numDisp[4];        //LEDに表示する数字 
int setTime = 5000;
int stopFlag = 0;   // タイマーストップフラグ

// 青赤正解フラグ
int answer;
int redLineValue; // 赤線の状態
int blueLineValue; // 青線の状態

// seven_ledsをbyte型として定義
// 配列にDisplay表示用のデータ0~9と全消灯を設定
// 1 = LED on, 0 = LED off
byte seven_leds[14] =       { B11111100,  // 0
                              B01100000,  // 1
                              B11011010,  // 2
                              B11110010,  // 3
                              B01100110,  // 4
                              B10110110,  // 5
                              B10111110,  // 6
                              B11100000,  // 7
                              B11111110,  // 8
                              B11100110,  // 9
                              B00000001,  // 10 D.P 
                              B00000000,  // 11 OFF
                              B10111100,  // 12 G
                              B01111010,  // 13 D
                             };

// ゲームクリア関数
void funcGameClear(){
  stopFlag = 2;
}

// ゲームオーバー関数
void funcGameOver(){
  timeDisp = 0;
  tone(buzzer,262,10000);
  stopFlag = 1;
}

//シフトレジスタ
void funcShiftReg(int x)
{
  digitalWrite(rclkPin, LOW);                         //送信中のRCLKをLowにする
  shiftOut(dsPin, srclkPin, LSBFIRST, seven_leds[x]); //シフト演算を使って点灯するLEDを選択
  digitalWrite(rclkPin, HIGH);                        //送信終了後RCLKをHighにする
}
  
void setup() 
{
  pinMode(red, INPUT);        //0番ピンをINPUTとして定義
  pinMode(blue, INPUT);       //1番ピンをINPUTとして定義
  pinMode(rclkPin, OUTPUT);   //11番ピンをOUTPUTとして定義
  pinMode(dsPin, OUTPUT);     //12番ピンをOUTPUTとして定義
  pinMode(srclkPin, OUTPUT);  //9番ピンをOUTPUTとして定義
  pinMode(dig1, OUTPUT);      //7番ピンをOUTPUTとして定義
  pinMode(dig2, OUTPUT);      //2番ピンをOUTPUTとして定義
  pinMode(dig3, OUTPUT);      //3番ピンをOUTPUTとして定義
  pinMode(dig4, OUTPUT);      //4番ピンをOUTPUTとして定義
  digitalWrite(dig1, HIGH);    //1番ピンをHIGH DI1 OFF
  digitalWrite(dig2, HIGH);    //2番ピンをHIGH DI2 OFF
  digitalWrite(dig3, HIGH);    //3番ピンをHIGH DI3 OFF
  digitalWrite(dig4, HIGH);    //4番ピンをHIGH DI4 OFF
  funcShiftReg(11);           //信号初期化x
}

void loop() 
{
  switch (stopFlag) {
  case 2: 
    //7セグLEDの制御
    //DIG1(上1桁)の表示 
     delay(tDelay);
     digitalWrite(dig1, LOW); 
     digitalWrite(dig2, HIGH);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, HIGH);
     //DIG1に1を表示
     funcShiftReg(12);
     delay(tDelay);
     funcShiftReg(11);
     
     //DIG2(上2桁)の表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, LOW);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, HIGH);
     //DIG2に2を表示
     funcShiftReg(0);
     delay(tDelay);
     funcShiftReg(11);
  
     //DIG3(上3桁)の表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, HIGH);
     digitalWrite(dig3, LOW);
     digitalWrite(dig4, HIGH);
     //DIG3に3を表示
     funcShiftReg(0);
     delay(tDelay);
     funcShiftReg(11);
     
     //DIG4(上4桁)の表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, HIGH);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, LOW);
     //DIG4に4を表示
     funcShiftReg(13);
     delay(tDelay);
     funcShiftReg(11);
    break;
  case 1:
    break;
  default : 
    //現在時刻の取得 
    timeNow = millis(); 
    if (timeDisp == 9999) {
      randomSeed(timeNow);
      timeStart = timeNow;
    }
    timeDisp = setTime - (timeNow - timeStart) / 10;

    redLineValue = digitalRead(red);
    blueLineValue = digitalRead(blue);

    if (timeDisp < 0) {
      funcGameOver();
    }
    else if (redLineValue == 0 || blueLineValue == 0){
      answer = timeNow % 2;
      if ((answer == 0 && redLineValue == 0) || (answer == 1 && blueLineValue == 0)){
        funcGameClear();
      }
      else {
        funcGameOver();
      }
    }

    //表示時刻をLEDの数字へ変更 
    
    numDisp[0] = (timeDisp / 1000)%10;  
    numDisp[1] = (timeDisp / 100)%10; 
    numDisp[2] = (timeDisp / 10)%10; 
    numDisp[3] = timeDisp % 10; 
    //7セグLEDの制御
    //DIG1(上1桁)の表示 
     delay(tDelay);
     digitalWrite(dig1, LOW); 
     digitalWrite(dig2, HIGH);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, HIGH);
     //DIG1に1を表示
     funcShiftReg(numDisp[0]);
     delay(tDelay);
     funcShiftReg(11);
     
     //DIG2(上2桁)の表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, LOW);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, HIGH);
     //DIG2に2を表示
     funcShiftReg(numDisp[1]);
     delay(tDelay);
     funcShiftReg(11);
  
     //DIG2のドット表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, LOW);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, HIGH);
     //DIG2に2を表示
     funcShiftReg(10);
     delay(tDelay);
     funcShiftReg(11);
     
     //DIG3(上3桁)の表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, HIGH);
     digitalWrite(dig3, LOW);
     digitalWrite(dig4, HIGH);
     //DIG3に3を表示
     funcShiftReg(numDisp[2]);
     delay(tDelay);
     funcShiftReg(11);
     
     //DIG4(上4桁)の表示
     delay(tDelay);
     digitalWrite(dig1, HIGH); 
     digitalWrite(dig2, HIGH);
     digitalWrite(dig3, HIGH);
     digitalWrite(dig4, LOW);
     //DIG4に4を表示
     funcShiftReg(numDisp[3]);
     delay(tDelay);
     funcShiftReg(11);
  }
}

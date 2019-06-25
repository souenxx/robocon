/*
 * Rcourse.c
 *
 *  Created on: 2017/07/06
 *  Author: Hiroki
 */

 #include "Rcourse.h"

//初期化
 void Start_Sumou_init(Rcourse* this, BasicRun* basicrun, ColorSensor* colorsensor, SonarSensor* sonarsensor, Motor* arm_motor, Sumo_Action* sumo_action, Linetracer* linetracer, PID* pid, Driver* driver, Light_Lightdetector* lightdetector, GyroSensor* gyrosensor, Gray_detection* gray_detection, Motor* left_motor, Motor* right_motor){
 	this -> state = 0;
  this -> small_state = 0;
  this -> counter = 0;
  this -> line_state = 0;
  this -> timer = 0;
  this -> Dohyo_Color = COLOR_NONE;
 	this -> Block_Color = COLOR_NONE;
  this -> last_action = 0;
  this -> place = 1;
  this -> big_timer = 0;
  this -> distance = 255;

 	this -> basicrun = basicrun;
 	this -> colorsensor = colorsensor;
  this -> sonarsensor = sonarsensor;
  this -> arm_motor = arm_motor;
  this -> left_motor = left_motor;
  this -> right_motor = right_motor;
  this -> sumo_action = sumo_action;
  this -> linetracer = linetracer;
  this->pid = pid;
  this->driver = driver;
  this->lightdetector = lightdetector;
  this->gyrosensor = gyrosensor;
  this -> gray_detection = gray_detection;
 }

//Rコースの行動を行う
int Start_Sumou(Rcourse* this){
  this -> big_timer++;
  /*
  lcdfont_t font = EV3_FONT_MEDIUM;
  ev3_lcd_set_font(font);
  int32_t fontw, fonth;
  ev3_font_get_size(font, &fontw, &fonth);

  ev3_lcd_draw_string("Color is BLUE",0,0);   //EV3の画面に表示
  ev3_lcd_draw_string("Yorikiri",0,fonth*1);
  ev3_speaker_play_tone (NOTE_C4,500);  //音を出す
  */

  //灰色検知をするまで韋駄天を行う
  if(this -> state == 0){
    if(this -> small_state == 0){
      Linetracer_do(this -> linetracer, 0, 0);

      this -> timer++;
      if(this -> timer >= 5800){
        PID_gray_detection(this->pid);
        Motor_stop(this -> left_motor,true);
        Motor_stop(this -> right_motor,true);
        this -> small_state = 1;
      }
    }
    else if(this -> small_state == 1){
      Linetracer_do(this -> linetracer, 0, 0);
      if(Gray_detection_do(this -> gray_detection) == 1){
        this -> timer = 0;
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> small_state = 0;
        this -> state = 1;
      }
    }
  }

  //灰色の部分からバトンタッチされたところからスタートする時
  //モータストップ
  //（停止時に右に少し曲がるので）25度旋回する
  //アームの角度を下げて地面に着ける
  //安全走行でライントレースをする（大体、線路の前）
  //モータをストップし、アームを元の位置に戻す
  else if(this -> state == 1){
    if(this -> small_state == 0){
      Motor_stop(this -> left_motor,true);
      Motor_stop(this -> right_motor,true);
      this -> small_state = 1;
    }
    else if(this -> small_state == 1){
      if(BasicRun_Curve(this -> basicrun,-25) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 3;
      }
    }
    //大会直前での変更
    /*
    else if(this -> small_state == 2){
      if(BasicRun_GoStraight(this -> basicrun,6) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 3;
      }
    }
    */
    else if(this -> small_state == 3){
      Motor_setRotatedegree(this->arm_motor, -20 ,20,true);
      Motor_stop(this->arm_motor, true);
      this -> small_state = 4;
    }
    else if(this -> small_state == 4){
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);
      this -> timer++;
      if(this -> timer >= 1400){
        this -> timer = 0;
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> small_state = 6;
      }
    }
    //大会直前での変更
    /*
    else if(this -> small_state == 4){
      Motor_stop(this -> left_motor,true);
      Motor_stop(this -> right_motor,true);
      this -> small_state = 5;
    }
    else if(this -> small_state == 5){
      Linetracer_do(this -> linetracer, 0, 0);
      this -> timer++;
      if(this -> timer >= 830){
        this -> timer = 0;
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> small_state = 6;
      }
    }
    */
    else if(this -> small_state == 6){
      Motor_stop(this -> left_motor,true);
      Motor_stop(this -> right_motor,true);
      this -> small_state = 7;
    }
    else if(this -> small_state == 7){
      Motor_setRotatedegree(this->arm_motor, 20 ,20,true);
      Motor_stop(this->arm_motor, true);
      this -> small_state = 0;
      this -> state = 2;
    }
  }
  //線路を越えるためにBasicRunの速度を変更する
  else if(this -> state == 2){
    BasicRun_speedchange(this -> basicrun, 55);
    this -> state = 3;
  }

  //線路前に設置->新幹線が通るまで待機
  //1秒待機する（timer = 1の時、約0.04秒）（最初は2秒停止する）
  //最初のラインの交差点まで直進
  //右に90度旋回
  else if(this -> state == 3){
    /*
    if(this -> small_state == 0){
      //新幹線の場所を確認して通過可能か判断する
      //place=「1」から「3」を指定
      int train_state = Train_Timer(this -> place, this -> big_timer);
      this -> place++;
      if(train_state == 1){
        this -> small_state = 1;
      }
      else if(train_state == 2){
        this -> small_state = 2;
      }
      else if(train_state == 3){
        this -> small_state = 3;
      }
      this -> small_state = 1;
      this -> place++;
    }
    */
    if(this -> small_state == 0){
      this -> distance = SonarSensor_getDistance(this -> sonarsensor);
      if(this -> distance <= 20){
        this -> place++;
        this -> small_state = 2;
        this -> distance = 255;
      }
    }
    else if(this -> small_state == 2){
      if(this -> place == 2){
        this -> timer++;
        if(this -> timer >= 500){
          this -> timer = 0;
          this -> small_state = 3;
        }
      }
      else{
        this -> timer++;
        if(this -> timer >= 250){
          this -> timer = 0;
          this -> small_state = 3;
        }
      }
    }
    else if(this -> small_state == 3){
      if(this -> counter == 4){
        if(BasicRun_GoStraight(this -> basicrun,36) == 1){
          BasicRun_reset(this -> basicrun);
          this -> small_state = 4;
        }
      }
      else{
        if(BasicRun_GoStraight(this -> basicrun,33) == 1){
          BasicRun_reset(this -> basicrun);
          this -> small_state = 4;
        }
      }
    }
    else if(this -> small_state == 4){
      if(BasicRun_Curve(this -> basicrun,90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 0;
        this -> state = 4;
      }
    }
  }
  //土俵の色を検知するまでライントレースする（青色は例外）
  else if(this -> state == 4){
    //土俵の色が黄色の時
    if(this -> counter == 0 || this -> counter == 4){
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);

      if(ColorSensor_getColor(this -> colorsensor) == COLOR_YELLOW){
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> Dohyo_Color = COLOR_YELLOW;
        BasicRun_reset(this -> basicrun);
        this -> state = 5;
        this -> counter++;
      }
    }
    //土俵の色が緑色の時
    else if(this -> counter == 1 || this -> counter == 5){
      //安全走行
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);

      if(ColorSensor_getColor(this -> colorsensor) == COLOR_GREEN){
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> Dohyo_Color = COLOR_GREEN;
        BasicRun_reset(this -> basicrun);
        this -> state = 5;
        this -> counter++;
      }
    }
    //土俵の色が青色の時（現状青色の最適な取り方がわからない・・・）
    else if(this -> counter == 2 || this -> counter == 6){
      //安全走行
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);

      if(ColorSensor_getColor(this -> colorsensor) == COLOR_BLUE && ColorSensor_getReflectBrightness(this -> colorsensor) < 20){
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> Dohyo_Color = COLOR_BLUE;
        BasicRun_reset(this -> basicrun);
        this -> state = 5;
        this -> counter++;
      }
    }
    //土俵の色が赤色の時
    else if(this -> counter == 3 || this -> counter == 7){
      //安全走行
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);

      if(ColorSensor_getColor(this -> colorsensor) == COLOR_RED){
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> Dohyo_Color = COLOR_RED;
        BasicRun_reset(this -> basicrun);
        this -> state = 5;
        this -> counter++;
      }
    }
  }
  //次の動作を行う
  //ちょっとバックする
  //アームを上げる
  //ブロックの色を検知する
  //アームを下げる
  else if(this -> state == 5){
    if(this -> small_state == 0){
      if(BasicRun_GoStraight(this -> basicrun,-1) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 1;
      }
    }
    else if(this -> small_state == 1){
      Motor_setRotatedegree(this->arm_motor, 60 ,20,true);
      this -> small_state = 2;
    }
    else if(this -> small_state == 2){
      this -> Block_Color = ColorSensor_getColor(this -> colorsensor);
      this -> small_state = 3;
    }
    else if(this -> small_state == 3){
      Motor_setRotatedegree(this->arm_motor, -60 ,20,true);
      this -> small_state = 0;
      this -> state = 6;
    }
  }
  //土俵の色とブロックの色を比較して同色なら「寄り切り」、異色なら「押し出し」を行う
  //そのあと、ブロックを何個動かしたかでstateが変わる
  else if(this -> state == 6){
    int ACTION = Color_Comparison(this->Dohyo_Color, this->Block_Color);
    if(Start_Sumo(this -> sumo_action,ACTION,this -> counter) == 1){
      //2個、または6個ブロックを動かした時
      if(this -> counter == 2 || this -> counter == 6){
        this -> state = 7;
      }
      //4個ブロックを動かした時
      else if(this -> counter == 4){
        this -> state = 8;
      }
      //8個ブロックを動かした時
      else if(this -> counter == 8){
        this -> last_action = ACTION;
        this -> state = 9;
      }
      //それ以外の個数の時
      else{
        this -> state = 4;
      }
    }
  }
  //（2個、または6個ブロックを動かした後に）90度右旋回してバックする
  //その後、90度右旋回してブロックのある方向に向く（新幹線接触を避けるため）
  else if(this -> state == 7){
    if(this -> small_state == 0){
      if(BasicRun_Curve(this -> basicrun,90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 1;
      }
    }
    else if(this -> small_state == 1){
      if(BasicRun_GoStraight(this -> basicrun,-15) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 2;
      }
    }
    else if(this -> small_state == 2){
      if(BasicRun_Curve(this -> basicrun,180) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 3;
      }
    }
    else if(this -> small_state == 3){
      if(BasicRun_GoStraight(this -> basicrun,15) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 4;
      }
    }
    else if(this -> small_state == 4){
      if(BasicRun_Curve(this -> basicrun,-90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 0;
        this -> state = 4;
      }
    }
  }
  //4個ブロックを動かした後に真ん中に戻って90度右に旋回
  //stateを3に戻す
  else if(this -> state == 8){
    if(this -> small_state == 0){
      if(BasicRun_GoStraight(this -> basicrun,3) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 1;
      }
    }
    else if(this -> small_state == 1){
      if(BasicRun_Curve(this -> basicrun,90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 0;
        this -> state = 3;
      }
    }
  }
  //次の動作を行う
  //ブロックを8個動かした後の動作（state=13から続き）
  //左に旋回（76度）
  //15cm直進
  //懸賞のある方向を向く
  //アームを下に目一杯下げてその状態をロックする
  //懸賞方向に直進
  //アームをゆっくり上げて懸賞獲得

  //新幹線が通るまで待機
  //線路を通過
  else if(this -> state == 9){
    if(this -> small_state == 0){
      if(this -> last_action == 0){
        if(BasicRun_Curve(this -> basicrun,88) == 1){
          BasicRun_reset(this -> basicrun);
          this -> small_state = 1;
        }
      }
      else{
        if(BasicRun_Curve(this -> basicrun,94) == 1){
          BasicRun_reset(this -> basicrun);
          this -> small_state = 1;
        }
      }
    }
    else if(this -> small_state == 1){
      if(BasicRun_GoStraight(this -> basicrun,-16) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 2;
      }
    }
    else if(this -> small_state == 2){
      if(BasicRun_Curve(this -> basicrun,90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 3;
      }
    }
    else if(this -> small_state == 3){
      Motor_setRotatedegree(this->arm_motor, -20 ,20,true);
      Motor_stop(this->arm_motor, true);
      this -> small_state = 4;
    }
    else if(this -> small_state == 4){
      if(BasicRun_GoStraight(this -> basicrun,16) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 5;
      }
    }
    else if(this -> small_state == 5){
      Motor_setRotatedegree(this->arm_motor, 85 , 10, true);
      this -> small_state = 6;
    }
    else if(this -> small_state == 6){
      if(BasicRun_GoStraight(this -> basicrun,-19) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 7;
      }
    }
    else if(this -> small_state == 7){
      if(BasicRun_Curve(this -> basicrun,-90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 9;
      }
    }
    else if(this -> small_state == 8){
      //新幹線の場所を確認して通過可能か判断する
      //place=「1」から「3」を指定
      /*
      int train_state = Train_Timer(this -> place, this -> big_timer);
      if(train_state == 1){
        this -> small_state = 9;
      }
      else if(train_state == 2){
        this -> small_state = 10;
      }
      else if(train_state == 3){
        this -> small_state = 11;
      }
      */
      this -> small_state = 9;
    }
    else if(this -> small_state == 9){
      if(SonarSensor_getDistance(this -> sonarsensor) <= 30){
        this -> small_state = 10;
      }
    }
    else if(this -> small_state == 10){
      this -> timer++;
      if(this -> timer >= 250){
        this -> timer = 0;
        this -> small_state = 11;
      }
    }
    else if(this -> small_state == 11){
      if(BasicRun_GoStraight(this -> basicrun,53) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 0;
        this -> state = 10;
      }
    }
  }
  //state = 32まで懸賞をおくプログラム
  else if(this -> state == 10){
    if(this -> small_state == 0){
      if(BasicRun_Curve(this -> basicrun,90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 1;
      }
    }
    else if(this -> small_state == 1){
      if(BasicRun_GoStraight(this -> basicrun,10) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 2;
      }
    }
    else if(this -> small_state == 2){
      Motor_setRotatedegree(this->arm_motor, -65 ,20,true);
      this -> small_state = 4;
    }
    else if(this -> small_state == 3){
      if(BasicRun_Curve(this -> basicrun,-30) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 4;
      }
    }
    else if(this -> small_state == 4){
      if(BasicRun_GoStraight(this -> basicrun,-17) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 5;
      }
    }
    else if(this -> small_state == 5){
      if(BasicRun_Curve(this -> basicrun,-90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 0;
        this -> state = 11;
        //this -> state = 100;
      }
    }
  }
  //ここから駐車を行う部分（懸賞置き場から垂直の位置にあるラインからスタート）
  //ライントレースを行う（数秒間）
  //少し前進する（直角を曲がる対策）
  //90度右に旋回
  //ライントレースして指定秒数後停止（駐車完了）
  else if(this -> state == 11){
    if(this -> small_state == 0){
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);

      //timer = 1の時、約0.004秒
      this -> timer++;
      if(this -> timer >= 800){
        this -> timer = 0;
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> small_state = 1;
      }
    }
    else if(this -> small_state == 1){
      Motor_stop(this -> left_motor,true);
      Motor_stop(this -> right_motor,true);
      this -> small_state = 2;
    }
    else if(this -> small_state == 2){
      if(BasicRun_GoStraight(this -> basicrun,4) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 3;
      }
    }
    else if(this -> small_state == 3){
      if(BasicRun_Curve(this -> basicrun,90) == 1){
        BasicRun_reset(this -> basicrun);
        this -> small_state = 4;
      }
    }
    else if(this -> small_state == 4){
      if(this -> line_state == 0){
        PID_saveParam(this->pid);
        this -> line_state++;
      }
      Linetracer_do(this -> linetracer, 0, 1);

      //timer = 1の時、約0.004秒
      //4秒で多分大丈夫
      this -> timer++;
      if(this -> timer >= 950){
        this -> timer = 0;
        this -> line_state = 0;
        PID_reset(this->pid);
        this -> small_state = 0;
        this -> state = 100;
      }
    }
  }
  else if(this -> state == 100){
    return 1;
  }
  return 0;
}

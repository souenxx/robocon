/*
 * BasicRun_L.c
 *
 *  Created on: 2016/08/24
 *      Author: Fukunaga
 */

#include "BasicRun_L.h"
/*
 *注意:すべてのmethodは終わったら1サイクルだけ1を返します.
 *    次のサイクルでまた呼ぶと同じ処理をやり直します.
 *    このクラスのmethodで自分のステートを動かしたい場合
 *    ifでステートを動かすのではなくswitch文での運用をおすすめします.
 */


/*
 * 初期化
 */
void BasicRun_L_init(BasicRun_L* this,TouchSensor* touchsensor, Motor* leftMotor, Motor* rightMotor, ColorSensor* colorsensor, Light_Lightdetector* lightdetector, Calibrator* calibrator){
  this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
  this->lightdetector = lightdetector;
  this->touchsensor = touchsensor;
  this->state = 0;
  this->state_count = 0;
  this->initangle = 0;
  this->colorsensor = colorsensor;
  this->detect_color = 0;
  this->count_vert = 0;
  this->state_vert = 0;
  this->calibrator = calibrator;
  this->count = 0;
  this->bound_count = 0;

  this->count_tama_state = 0;
  this->tama_count =0;
  this->curve_tama_state = 0;
  this->roll_tama_state =0;
  this->tamangle = 0;
  this->white_tama = 0;
  this->fukki_state=0;
  BasicRun_L_reset(this);

}
//速度変更
void BasicRun_L_speedchange(BasicRun_L* this, int speed){
  this->Speed = speed;
}
//各変数の変更  運搬時用
void BasicRun_L_paramchange(BasicRun_L* this){
  this->delta = del*0.95;//小さくするほど遠くまで行く
  this->pivot90 = basepivot_L - 10;//小さくするほど回らない
  this->angle90 = baseangle_L - 5.5;//小さくするほど回らない
  BasicRun_L_speedchange(this,10);
}
//再初期化
//1が返ってくる前に他のmethodを使うことをやめたら必ず呼ぶ
void BasicRun_L_reset(BasicRun_L* this){
  this->state = 0;
  this->Speed = CurveSpeed_L;
  this->delta = del;
  this->pivot90 = basepivot_L;
  this->angle90 = baseangle_L;
}

//軸に従って適した軸のポインタを返す
Motor *BasicRun_L_getMotor(BasicRun_L* this,int rightPivot){
  //右車輪軸回転 左を動かす
  if(rightPivot==0){
    return this->rightMotor;
  }//浸り車輪軸回転
  else{
    return this->leftMotor;
  }
}
//片足を軸に旋回する
//rightPivotが1で右軸で左車輪が動く
//終わったら1を返却する
int BasicRun_L_pivot(BasicRun_L* this, int angle, int rightPivot){

  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(BasicRun_L_getMotor(this,rightPivot));
      this->state = 1;
      break;
    //pivot
    case 1:
      if(angle > 0){
        Motor_setPower(BasicRun_L_getMotor(this,rightPivot), this->Speed);
      }else{
        Motor_setPower(BasicRun_L_getMotor(this,rightPivot), -1*this->Speed);
      }
      //指定角度まで進んだら
      float tmp = abs(Motor_getAngle(BasicRun_L_getMotor(this,rightPivot))-this->initangle);
      if( tmp >= abs(this->pivot90/90.0*angle)  ){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
    case 3:
      BasicRun_L_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;
}

//終わったら1を返却する
//その場旋回
//angleがプラスで右曲がり　マイナスで左曲がり
int BasicRun_L_Curve(BasicRun_L* this, int angle){

  switch(this->state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->state = 1;
      break;
    //旋回
    case 1:
      if(angle > 0){//右曲がり
        Motor_setPower(this->rightMotor, -1*this->Speed);
        Motor_setPower(this->leftMotor, this->Speed);
      }else{//左曲がり
        Motor_setPower(this->rightMotor, this->Speed);
        Motor_setPower(this->leftMotor, -1*this->Speed);
      }

      //指定角度まで進んだら
      float tmp = abs(Motor_getAngle(this->rightMotor) - this->initangle);
      if( tmp >= abs(this->angle90/90.0*angle)){
        this->state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->state = 3;
      }
      break;
      // this->state = 3;
      // break;
    case 3:
      BasicRun_L_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;
}


//指定距離　直進する
//---return について--
//終わてないなら-1
//終わている　かつ　黒があったなら１(detect_color)
//終わている　かつ　黒がないなら0
//
//---colorStop---
//colorStop==1のとき、colorをみつけたらとまる

int BasicRun_L_GoStraight(BasicRun_L* this, double cm, int color, int colorStop){
  switch(this->state){
    //今の回転角度を取得
    case 0:
      // Motor_stop(this->leftMotor,true);
      // Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->state = 1;
      break;
    //直進
    case 1:
      if(cm > 0){
        Motor_setPower(this->rightMotor, (this->Speed));
        Motor_setPower(this->leftMotor, (this->Speed));
      }else{
        Motor_setPower(this->rightMotor, -1*(this->Speed));
        Motor_setPower(this->leftMotor, -1*(this->Speed));
      }

      // ここがbasicrunとの違い！
      // if(ColorSensor_getColor(this->colorsensor) == color) {
      //   this->detect_color = 1;
      // }
      int normal_light;
      int lpf_light;
      Lightdetector_get(this->lightdetector, &normal_light, &lpf_light);
      if(normal_light >= 30) {
        if(color == 2){
          this->detect_color = 1;
        }
      } else if(normal_light < 10) {
        if(color == 3){
          this->detect_color = 1;
        }
      } else {
        if(color == 1){
          this->detect_color = 1;
        }
      }


      if(colorStop == 1 && this->detect_color == 1){
        this -> state = 2;
      }

      //指定距離まで進んだら
      float tmp = abs(Motor_getAngle(this->rightMotor) - this->initangle);
      if( tmp*this->delta >= abs(cm)  ){
        this->state = 2;
      }
      break;
    case 2://停止
      // Motor_stop(this->leftMotor,true);
      // Motor_stop(this->rightMotor,true);
      // if(Motor_getPower(this->rightMotor) == 0){
      //   this->state = 3;
      // }
      this->state = 3;
      break;
    case 3:
      BasicRun_L_reset(this);
      if(this->detect_color == 1){
        this->detect_color=0;
        return 1;
      } else {
        return 0;
      }
      break;
    default:
      break;
  }
  return -1;

}


int vertCurve(BasicRun_L* this, int leftflag){
  if(this->state_vert == 0){
    if(countup(this) != -1) {
      this->state_vert = 1;
    }
  } else if(this->state_vert == 1) {
    // int ang = (int)(90.0 - acos(this->bound_count/this->count)*(180.0/M_PI));
    double a = (double)this->bound_count / (double)this->count;
    double rad = acos(a);
    double doo = rad * (180.0/3.14159);
    double ang_dou = 90. - doo;
    this->ang = (int)ang_dou;
    char lcdstr[100];
    sprintf(lcdstr, "t1=%d,t2=%d",this->bound_count, this->count);
    ev3_lcd_draw_string(lcdstr,0,0);
    sprintf(lcdstr, "ang=%d, t/t2=%f",this->ang, a);
    ev3_lcd_draw_string(lcdstr,0,20);
    if(a > 1){
      this->ang = 90.0;
    }
    this->state_vert = 15;

  } else if(this->state_vert == 15){
    if(BasicRun_L_GoStraight(this, 3.*(this->ang/90.), 0, 0) != -1){
      this->state_vert = 2;
    }
  } else if(this->state_vert == 2){
    if(BasicRun_L_Curve(this,pow(-1, leftflag)*this->ang) == 1){
      this->state_vert = 3;
    }
  } else if(this->state_vert == 3){
    this->state_vert = 0;
    return 1;
  }
  return 0;
}

int countup(BasicRun_L* this) {
  //	まずは前進しつづける
  if(this->state_count == 0){
  	Motor_setPower(this->leftMotor,SET);
  	Motor_setPower(this->rightMotor,SET);

    this->state_count = 1;
  } else if(this->state_count == 1) {

   	int light = ColorSensor_getReflectBrightness(this->colorsensor);
    if (light == this->bound){
      this->state_count = 2;
    }
  } else if(this->state_count == 2) {
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    this->count_vert++;
    if (light != this->bound){
      this->state_count = 3;
    }
  } else if(this->state_count == 3) {

    int light = ColorSensor_getReflectBrightness(this->colorsensor);

    this->count_vert++;
    if (light == this->bound){
      this->state_count = 4;
    }
  } else if(this->state_count == 4){
    Motor_stop(this->leftMotor,true);
    Motor_stop(this->rightMotor,true);
    this->state_count = 0;
    this->count = this->count_vert;
    this->count_vert = 0;
    return 1;
  }
  return -1;
}

void set_bound_count(BasicRun_L* this, int count){
  this->bound_count = count;
}

int get_bound_count(BasicRun_L* this){
  if(this->state == 0){
    if (TouchSensor_getState(this->touchsensor)== true){
      double a = Calibrator_get_boundary(this->calibrator);
      this->bound = (int)a;
      ev3_led_set_color(LED_ORANGE);
      this->state = 1;
    }
  } else if(this->state == 1){
    Motor_setPower(this->leftMotor,SET);
    Motor_setPower(this->rightMotor,SET);

    this->state = 2;
  } else if(this->state == 2){
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    if (light == this->bound){
      this->state = 3;
    }
  } else if(this->state == 3){
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    this->bound_count++;
    if (light != this->bound){
      this->state = 4;
    }
  } else if(this->state == 4){
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    this->bound_count++;
    if (light == this->bound){
      this->state = 5;
    }
  } else if(this->state == 5) {
    Motor_stop(this->leftMotor,true);
    Motor_stop(this->rightMotor,true);
    this->state = 0;
    return 1;
  }
  return 0;
}



//----------------たまプロ---------------------------------------------------
//-------------------------------------------------------------------

void reset_tama(BasicRun_L* this){
  this->tama_count = 0;
}

int get_tama(BasicRun_L* this){
  return this->tama_count;
}

//境界値から境界値の幅をカウント
//target_count,base_countのカウント用
//this->tama_countがカウント数になる,<⚠️>リセットして使い回し<⚠️>
int count_tama(BasicRun_L* this){
  //境界値と白の値を保持する,初回のみ実行
  if(this->count_tama_state == 0){
      double a = Calibrator_get_boundary(this->calibrator);
      int b = Calibrator_get_white(this->calibrator);
      this->bound = (int)a;
      this->white_tama = b;
      this->count_tama_state = 1;
  //一定速度で走らせる
} else if(this->count_tama_state == 1){
    Motor_setPower(this->leftMotor,SET);
    Motor_setPower(this->rightMotor,SET);
    this->count_tama_state = 2;
  //白スタートが前提
  //白→境界値(灰色)
  } else if(this->count_tama_state == 2){
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    if (light == this->bound){
      this->count_tama_state = 3;
    }
  //境界値から黒
  //カウントを始める
  } else if(this->count_tama_state == 3){
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    this->tama_count++;
    if (light != this->bound){
      this->count_tama_state = 4;
    }
  //黒から境界値
  //この間もカウントアップ
  } else if(this->count_tama_state == 4){
    int light = ColorSensor_getReflectBrightness(this->colorsensor);
    this->tama_count++;
    if (light == this->bound){
      this->count_tama_state = 5;
    }
  //境界値に到達,モーターをストップ
  //state=1はstate0を初回だけにするため
  } else if(this->count_tama_state == 5) {
    Motor_stop(this->leftMotor,true);
    Motor_stop(this->rightMotor,true);
    this->count_tama_state = 1;
    return 1;
  }
  return 0;
}

//ラインに平行になるまで回転する(全体編)
int tamaCurve(BasicRun_L* this,int base_count,int tama){
  if(this->curve_tama_state == 0) {
    //底辺/斜辺,三角関数
    double a = (double)base_count / (double)this->tama_count;
    //acos()はオーバーフローは引数0になるので
    if(a>1){
      a=1;
    }
    //入射角算出
    double rad = acos(a);
    //構造体に保持,あとで使用
    this->tamangle = rad;
    this->curve_tama_state = 1;
    //平行になるためには適度に直進が必要
  } else if(this->curve_tama_state == 1){
    if(BasicRun_L_GoStraight(this, 4.5*cos(this->tamangle), 0, 0) != -1){//4.5は工藤の中心とセンサーまでのおよその距離
      this->curve_tama_state = 2;
    }
    //境界値まで回転
  } else if(this->curve_tama_state == 2){
    if(Tama_special(this,tama)==1){
      this->curve_tama_state = 3;
    }
    //リターン
  } else if(this->curve_tama_state == 3){
    this->curve_tama_state = 0;
    return 1;
  }
  return 0;
}

//平行になるまで回転プログラム(動作編)
//境界値を見つけるまで回転
int Tama_special(BasicRun_L* this,int tama){
//tama 1 右回転 0 左回転
  switch(this->roll_tama_state){
    //今の回転角度を取得
    case 0:
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      this->initangle = Motor_getAngle(this->rightMotor);
      this->roll_tama_state = 1;
      break;
    //旋回
    case 1:
      if(tama==1){//右曲がり
        Motor_setPower(this->rightMotor, -1*5); //回転の速さ,なんとなく5
        Motor_setPower(this->leftMotor, 5);
      }else{//左曲がり
        Motor_setPower(this->rightMotor, 5);
        Motor_setPower(this->leftMotor, -1*5);
      }

      //指定角度まで進んだら
      int gray_light = Calibrator_get_boundary(this->calibrator);
      int light = ColorSensor_getReflectBrightness(this->colorsensor);
      if( light <= gray_light-6){        //っここここここここここここここここここここ
        this->roll_tama_state = 2;
      }
      break;
    case 2://停止
      Motor_stop(this->leftMotor,true);
      Motor_stop(this->rightMotor,true);
      if(Motor_getPower(this->rightMotor) == 0){
        this->roll_tama_state = 3;
      }
      break;
      // this->state = 3;
      // break;
    case 3:
      BasicRun_L_reset(this);
      return 1;
      break;
    default:
      break;
  }
  return 0;
}

//ライン復帰プログラム
//引数(this,base_count,入射方向)
//入射方向,0左入射→右エッジ,1右入射→左エッジ
int Line_fukki(BasicRun_L* this,int base_count,int tama){
  if(this->fukki_state==0){
    //
    if (count_tama(this) == 1){
      this->fukki_state=1;
    }
  }else if(this->fukki_state==1){
    if (tamaCurve(this,base_count,tama) == 1){
      reset_tama(this);
      this->fukki_state=0;
      return 1;
    }
  }
  return 0;
  }


// char lcdstr[100];
// sprintf(lcdstr, "%d",light);
// ev3_lcd_draw_string(lcdstr,0,0);

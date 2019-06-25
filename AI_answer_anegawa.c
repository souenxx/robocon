#include "AI_answer.h"

void AI_answer_init(AI_answer* this, BasicRun* basicrun, BasicRun_L* basicrun_L, Motor* left_motor, Motor* right_motor){
 this -> state = 1;
 this -> state2 = 0;
 this -> basicrun = basicrun;
 this -> basicrun_L = basicrun_L;
 this -> left_motor = left_motor;
 this -> right_motor = right_motor;
}

//デジタル数字読み取り関数
int degital_recog(AI_answer* this) {
  int hoge;
  int result_score = 0;
  char lcdstr[100];
  if (this -> state == 1) {
    sprintf(lcdstr, "AI_state 1");
    ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,15);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 2;
      result_score += hoge*1;
    } //1
  } else if(this -> state == 2){
    if(BasicRun_L_Curve(this -> basicrun_L,90) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 3;
    }
  } else if(this -> state == 3){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,15);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 4;
      result_score += hoge * 2;
    } //2
  } else if(this -> state == 4){
    if(BasicRun_L_Curve(this -> basicrun_L,180) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 5;
    }
  } else if(this ->state == 5){
    if(BasicRun_L_GoStraight(this -> basicrun_L,15) != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 55;
    } // 3
  } else if(this ->state == 55){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,15);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 6;
      result_score+=hoge*4;
    } // 4
  } else if(this -> state == 6){
    if(BasicRun_L_Curve(this -> basicrun_L,180) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 7;
    }
  } else if(this ->state == 7){
    if(BasicRun_L_GoStraight(this -> basicrun_L,15) != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 8;
    } //5
  } else if(this -> state == 8){
    if(BasicRun_L_Curve(this -> basicrun_L,-90) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 9;
    }
  } else if(this -> state == 9){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,30);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 10;
      result_score+=hoge*8;
    } //6
  } else if(this -> state == 10){
    if(BasicRun_L_Curve(this -> basicrun_L,-90) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 11;
    }
  } else if(this -> state == 11){
    if(BasicRun_L_GoStraight(this -> basicrun_L,30) != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 12;
    } //7
  } else if(this -> state == 12){
    if(BasicRun_L_Curve(this -> basicrun_L,-90) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 13;
    }
  } else if(this -> state == 13){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,30);
      if(hoge != -1){
        BasicRun_L_reset(this -> basicrun_L);
        this->state = 14;
        result_score += hoge*16;
      } //8
  } else if(this -> state == 14){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,30);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 100;
      result_score += hoge*32;
    } //9
  } else if(this -> state == 100) {
    Motor_stop(this -> left_motor,true);
    Motor_stop(this -> right_motor,true);
    state_reset(this);
    return 0;
    int answer; //答え
    switch(result_score){
      case 33:
        answer = 1;
        break;
      case 23:
        answer = 2;
        break;
      case 39:
        answer = 3;
        break;
      case 45:
        answer = 4;
        break;
      case 35:
        answer = 7;
        break;
      case 59:
        answer = 0;
        break;
      case 63:
        answer = 8;
        break;
      default:
        answer = -1; //えらーだお
    }
    sprintf(lcdstr, "%d",answer);
    ev3_lcd_draw_string(lcdstr,0,0);
  }

  return -1;
}


//アナログ数字読み取り関数
int analog_recog(){

}

int brack_detection(){

}

//相撲を始める関数（駐車は別）
//初期位置を一番上の真ん中だと仮定
int sumou_do(AI_answer* this, int analog, int degital){
  // char lcdstr[100];
  if(this -> state2 == 0){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state2 = 1;
    }
  }
  else if(this -> state2 == 1){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 2;
    }
  }
  else if(this -> state2 == 2) {
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 3;
    }
  }
  else if(this -> state2 == 3){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 4;
    }
  }
  else if(this -> state2 == 4) {
    if(BasicRun_GoStraight(this -> basicrun,30) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 5;
    }
  }
  else if(this -> state2 == 5){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 6;
    }
  }
  else if(this -> state2 == 6) {
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 7;
    }
  }
  else if(this -> state2 == 7){
    if(BasicRun_Curve(this -> basicrun,90) == 1){
      BasicRun_reset(this -> basicrun);
      this->state2 = 100;
    }
  }
  else if(this->state2 == 100){
    Motor_stop(this -> left_motor,true);
    Motor_stop(this -> right_motor,true);
    state2_reset(this);
    return 0;
  }
  return -1;
}

void state_reset(AI_answer* this) {
  this -> state = 1;
}

void state2_reset(AI_answer* this) {
  this -> state2 = 1;
}

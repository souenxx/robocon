#include "AI_answer.h"

void AI_answer_init(AI_answer* this, BasicRun* basicrun, BasicRun_L* basicrun_L, Motor* left_motor, Motor* right_motor){
 this -> state = 1;
 this -> state = 1;
 this -> basicrun = basicrun;
 this -> left_motor = left_motor;
 this -> right_motor = right_motor;
}

//デジタル数字読み取り関数
int degital_recog(AI_answer* this) {
  char lcdstr[100];
  if (this -> state == 1) {
    sprintf(lcdstr, "AI_state 1");
    ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 2;
    }
  } else if(this -> state == 2){
    if(BasicRun_Curve(this -> basicrun,90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 3;
    }
  } else if(this -> state == 3){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 4;
    }
  } else if(this -> state == 4){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 5;
    }
  } else if(this ->state == 5){
    if(BasicRun_GoStraight(this -> basicrun,30) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 6;
    }
  } else if(this -> state == 6){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 7;
    }
  } else if(this ->state == 7){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 8;
    }
  } else if(this -> state == 8){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 9;
    }
  } else if(this -> state == 9){
    if(BasicRun_GoStraight(this -> basicrun,30) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 10;
    }
  } else if(this -> state == 10){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 11;
    }
  } else if(this -> state == 11){
    if(BasicRun_GoStraight(this -> basicrun,30) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 12;
    }
  } else if(this -> state == 12){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 13;
    }
  } else if(this -> state == 13){
      if(BasicRun_GoStraight(this -> basicrun,60) == 1){
        BasicRun_reset(this -> basicrun);
        this->state = 100;
      }


  } else if(this -> state == 100) {
    Motor_stop(this -> left_motor,true);
    Motor_stop(this -> right_motor,true);
    this -> state = 21;
    return 0;
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
int sumou_do(AI_answer* this, int analog, int degital) {
  if(this -> state == 21){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
        BasicRun_reset(this -> basicrun);
        this -> state = 22;
    }
  } else if(this -> state == 22){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 23;
    }
  } else if(this -> state == 23){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
    	// if(degital/4 == 1){
    	// }
    	// else if{
    	// }
    	degital=degital%4;
      this->state = 24;
    }
  } else if(this -> state == 24){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 25;
    }
  } else if(this -> state == 25){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 26;
    }
  } else if(this -> state == 26){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 27;
    }
  } else if(this -> state == 27){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 28;
    }
  } else if(this -> state == 28){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 29;
    }
  } else if(this -> state == 29){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
    	// if(degital/2 == 1){
    	// }
    	// else if{
    	// }
    	degital=degital%2;
      this->state = 30;
    }
  } else if(this -> state == 30){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 31;
    }
  } else if(this -> state == 32){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 33;
    }
  } else if(this -> state == 33){
    if(BasicRun_Curve(this -> basicrun,-90) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 34;
    }
  } else if(this -> state == 35){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
    	// if(degital == 1){
    	// }
    	// else if{
    	// }
    	degital=degital%4;
      this->state = 36;
    }
  } else if(this -> state == 36){
    if(BasicRun_Curve(this -> basicrun,180) == 1){
      BasicRun_reset(this -> basicrun);
      this -> state = 37;
    }
  } else if(this -> state == 37){
    if(BasicRun_GoStraight(this -> basicrun,15) == 1){
      BasicRun_reset(this -> basicrun);
      this->state = 100;
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

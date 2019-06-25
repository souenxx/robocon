#include "AI_answer.h"

void AI_answer_init(AI_answer* this, ColorSensor* colorsensor, BasicRun* basicrun, BasicRun_L* basicrun_L, Motor* left_motor, Motor* right_motor,Linetracer* linetracer){
 this -> state = 1;
 this -> state2 = 1;
 this -> sumou_state = 1;
 this -> ans_num_deg = 0;
 this -> ans_num_ana = 0;
 this -> basicrun = basicrun;
 this -> basicrun_L = basicrun_L;
 this -> left_motor = left_motor;
 this -> right_motor = right_motor;
 this -> linetracer = linetracer;
 this->colorsensor = colorsensor;
 this -> flag = 0;
 this -> minus1_zero = 0;
}

//デジタル数字読み取り関数
int degital_recog(AI_answer* this) {
  char lcdstr[100];
  if (this -> state == 1) {
    // sprintf(lcdstr, "AI_state 1");
    // ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,num_edge*1.5, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      if(hoge == 1){
        result = 1;
        this -> state = 2;
        this -> flag = 1;
      } else {
        result = 0;
        this -> state = 9;
        this -> flag = 0;  //ここのflagはstate = 9 のときバックか前進かを分けるぞ！
      }
    }
  } else if(this -> state == 2){
    BasicRun_L_speedchange(this->basicrun_L,10);
    if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 3;
    }
  } else if(this -> state == 3){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,num_edge, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      result += hoge*2;
      this -> state = 5;
    }
  // } else if(this -> state == 4){
  //   BasicRun_L_speedchange(this->basicrun_L,10);
  //   if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE*2) == 1){
  //     BasicRun_L_reset(this -> basicrun_L);
  //     this -> state = 5;
  //   }
  } else if(this ->state == 5){
    if(BasicRun_L_GoStraight(this -> basicrun_L,-num_edge, BRACK, 0) != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 55;
    }
  } else if(this ->state == 55){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,-num_edge-5, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      result += hoge*4;
      this->state = 7;
    }
  // } else if(this -> state == 6){
  //   BasicRun_L_speedchange(this->basicrun_L,10);
  //   if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE*2) == 1){
  //     BasicRun_L_reset(this -> basicrun_L);
  //     this -> state = 7;
  //   }
  } else if(this ->state == 7){
    if(BasicRun_L_GoStraight(this -> basicrun_L,num_edge+5, BRACK, 0) != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 8;
    }
  } else if(this -> state == 8){
    BasicRun_L_speedchange(this->basicrun_L,10);
    if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE+1) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 9;
    }
  } else if(this -> state == 9){
    this->minus1_zero = pow(-1, this->flag);
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,this->minus1_zero*num_edge*2, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      result += hoge*8;
      this->state = 10;
      this->flag = 0;
    }
  } else if(this -> state == 10){
    BasicRun_L_speedchange(this->basicrun_L,10);
    if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE+1) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 11;
    }
  } else if(this -> state == 11){
    if(BasicRun_L_GoStraight(this -> basicrun_L,(-1)*this->minus1_zero*num_edge*1.5, BRACK, 0) != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 12;
    }
  } else if(this -> state == 12){
    BasicRun_L_speedchange(this->basicrun_L,10);
    if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE+1) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 13;
    }
  } else if(this -> state == 13){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,this->minus1_zero*num_edge*2, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      result += hoge*16;
      this->state = 14;
    }
  } else if(this -> state == 14){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L,this->minus1_zero*num_edge*1.5, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      result += hoge*32;
      this->state = 100;
    }

  } else if(this -> state == 100) {
    Motor_stop(this -> left_motor,true);
    Motor_stop(this -> right_motor,true);
    state_reset(this);
    switch (result) {
      case 59:
      result_num = 0;
      break;
      case 33:
      result_num = 1;
      break;
      case 23:
      result_num = 2;
      break;
      case 39:
      result_num = 3;
      break;
      case 45:
      result_num = 4;
      break;
      case 40:
      result_num = 5;
      break;
      case 56:
      result_num = 6;
      break;
      case 35:
      result_num = 7;
      break;
      case 63:
      result_num = 8;
      break;
      default:
      result_num = 999;
      break;

    }
    sprintf(lcdstr, "%d",result);
    ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
    this->ans_num_deg = result_num;
    return result_num;
  }

  return -1;
}


//アナログ数字読み取り関数
int analog_recog(){
  return 0;//
}

int brack_detection(){
  return 0;//
}


// //相撲を始める関数（駐車は別）
// //初期位置を一番上の真ん中だと仮定
// int sumou_do(AI_answer* this, int analog, int degital) {
//   char lcdstr[100];
//   sprintf(lcdstr, "sumou do");
//   ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
//   if(this -> state == 1){//ブロックの間1に行く
//     if(BasicRun_GoStraight(this -> basicrun,tate+1) == 1){
//         BasicRun_reset(this -> basicrun);
//         this -> state = 2;
//     }
//   } else if(this -> state == 2){ //1bit目
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 3;
//     }
//   } else if(this -> state == 3){
//     if(BasicRun_GoStraight(this -> basicrun,yoko) == 1){
//       BasicRun_reset(this -> basicrun);
//     	if(this->ans_num_deg/4 == 1){
//         this->flag = 1;
//     	}	else {
//         this->flag = 0;
//     	}
//     	this->ans_num_deg=this->ans_num_deg%4;
//       this->state = 35;
//     }
//   } else if(this -> state == 35) {
//       if(blockmove_deg(this,this->flag) == 1) {
//         this -> state = 4;
//         state2_reset(this);
//       }
//       // this->state = 4;
//   } else if(this -> state == 4){
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE*2) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 5;
//     }
//   } else if(this -> state == 5){//ブロックの間に行く1
//     if(BasicRun_GoStraight(this -> basicrun,yoko*2) == 1){
//       BasicRun_reset(this -> basicrun);
//       this->state = 54;
//       if(this->ans_num_ana/4 == 1){
//         this->flag = 1;
//     	}	else{
//         this->flag = 0;
//     	}
//     	this->ans_num_ana=this->ans_num_ana%4;
//     }
//   } else if(this -> state == 54) {
//       if(blockmove_ana(this,this->flag) == 1) {
//         this -> state = 55;
//         state2_reset(this);
//       }
//       // this->state = 55;
//   } else if(this -> state == 55){//ブロックの間に行く1
//     if(BasicRun_GoStraight(this -> basicrun,-yoko) == 1){
//       BasicRun_reset(this -> basicrun);
//       this->state = 6;
//     }
//   } else if(this -> state == 6){
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 7;
//     }
//   } else if(this -> state == 7){
//     if(BasicRun_GoStraight(this -> basicrun,tate) == 1){
//       BasicRun_reset(this -> basicrun);
//       this->state = 8;
//     }
//   } else if(this -> state == 8){//2bit目
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 83;
//     }
//   } else if(this -> state == 83){
//     if(BasicRun_GoStraight(this -> basicrun,yoko) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 84;
//       if(this->ans_num_deg/2 == 1){
//         this->flag = 1;
//     	}	else{
//         this->flag = 0;
//     	}
//     	this->ans_num_ana=this->ans_num_ana%2;
//     }
//   } else if(this -> state == 84) {
//       if(blockmove_deg(this,this->flag) == 1) {
//         this -> state = 85;
//         state2_reset(this);
//       }
//       // this->state = 85;
//   } else if(this -> state == 85){
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE*2) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 9;
//     }
//   } else if(this -> state == 9){
//     if(BasicRun_GoStraight(this -> basicrun,yoko*2) == 1){
//       BasicRun_reset(this -> basicrun);
//       if(this->ans_num_ana/2 == 1){
//         this->flag = 1;
//     	}	else{
//         this->flag = 0;
//     	}
//     	this->ans_num_ana=this->ans_num_ana%2;
//       this->state = 94;
//     }
//   } else if(this -> state == 94) {
//       if(blockmove_ana(this,this->flag) == 1) {
//         this -> state = 95;
//         state2_reset(this);
//       }
//       // this->state = 95;
//   } else if(this -> state == 95){
//     if(BasicRun_GoStraight(this -> basicrun,-yoko) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 10;
//     }
//   } else if(this -> state == 10){
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 12;
//     }
//   } else if(this -> state == 12){
//     if(BasicRun_GoStraight(this -> basicrun,tate) == 1){
//       BasicRun_reset(this -> basicrun);
//       this->state = 13;
//     }
//   } else if(this -> state == 13){//3bit目
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 14;
//     }
//   } else if(this -> state == 14){
//     if(BasicRun_GoStraight(this -> basicrun,yoko) == 1){
//       BasicRun_reset(this -> basicrun);
//       if(this->ans_num_deg == 1){
//         this->flag = 1;
//     	}	else{
//         this->flag = 0;
//     	}
//       this->state = 145;
//     }
//   } else if(this -> state == 145) {
//       if(blockmove_deg(this,this->flag) == 1) {
//         this -> state = 15;
//         state2_reset(this);
//       }
//       // this->state = 15;
//   } else if(this -> state == 15){
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE*2) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 16;
//     }
//   } else if(this -> state == 16){
//     if(BasicRun_GoStraight(this -> basicrun,yoko*2) == 1){
//       BasicRun_reset(this -> basicrun);
//       if(this->ans_num_ana == 1){
//         this->flag = 1;
//     	}	else{
//         this->flag = 0;
//     	}
//       this->state = 164;
//     }
//   } else if(this -> state == 164) {
//       if(blockmove_ana(this,this->flag) == 1) {
//         this -> state = 165;
//         state2_reset(this);
//       }
//       // this->state = 165;
//   } else if(this -> state == 165){
//     if(BasicRun_GoStraight(this -> basicrun,-yoko) == 1){
//       BasicRun_reset(this -> basicrun);
//       this->state = 17;
//     }
//   } else if(this -> state == 17){
//     if(BasicRun_Curve(this -> basicrun,RIGHTANGLE) == 1){
//       BasicRun_reset(this -> basicrun);
//       this -> state = 100;
//     }
//   // } else if(this -> state == 18){
//   //   if(BasicRun_GoStraight(this -> basicrun,yoko) == 1){
//   //     BasicRun_reset(this -> basicrun);
//   //     this->state = 19;
//   //   }
//   // } else if(this -> state == 19){
//   //   if(BasicRun_Curve(this -> basicrun,-RIGHTANGLE) == 1){
//   //     BasicRun_reset(this -> basicrun);
//   //     this -> state = 20;
//   //   }
//   // } else if(this -> state == 20){
//   //   if(BasicRun_GoStraight(this -> basicrun,tate) == 1){
//   //     BasicRun_reset(this -> basicrun);
//   //     this->state = 100;
//   //   }
//   } else if(this->state == 100){
//     Motor_stop(this -> left_motor,true);
//     Motor_stop(this -> right_motor,true);
//     state_reset(this);
//     return 0;
//   }
//   return -1;
// }

//相撲を始める関数（駐車は別）
//初期位置を一番上の真ん中だと仮定
int sumou_do(AI_answer* this, int analog, int degital) {
  char lcdstr[100];
  sprintf(lcdstr, "sumou do");
  ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
  if(this -> state == 1){
    Linetracer_do(this->linetracer, 0 ,1);
    if(ColorSensor_getReflectBrightness(this->colorsensor) == COLOR_RED){
        this -> state = 15;
        blockLR(this, 4, 0);
    }
  } else if(this -> state == 15){ //1bit目
    if(blockmove_ana(this,this->flag) == 1) {
      this -> state = 2;
      flag_reset(this);
    }
  } else if(this -> state == 2){ //1bit目
    if(BasicRun_Curve(this -> basicrun,RIGHTANGLE*2) == 1){
      this -> state = 3;
    }
  } else if(this -> state == 3){
    Linetracer_do(this->linetracer, 0 ,0);
    if(ColorSensor_getReflectBrightness(this->colorsensor) == COLOR_BLUE){
        this -> state = 35;
        blockLR(this, 4,1);
    }
  } else if(this -> state == 35) {
      if(blockmove_deg(this,this->flag) == 1) {
        this -> state = 4;
        flag_reset(this);
      }
      // this->state = 4;
  } else if(this -> state == 4){
    if(next_sumou(this) == 1){
      this -> state = 5;
    }
  } else if(this -> state == 5){//ブロックの間に行く1
    Linetracer_do(this->linetracer, 0 ,1);
    if(ColorSensor_getReflectBrightness(this->colorsensor) == COLOR_GREEN){
        this -> state = 54;
        blockLR(this,2,0);
    }
  } else if(this -> state == 54) {
      if(blockmove_ana(this,this->flag) == 1) {
        this -> state = 55;
        flag_reset(this);
      }
    } else if(this -> state == 55){
      if(BasicRun_Curve(this -> basicrun,RIGHTANGLE*2) == 1){
        this -> state = 6;
      }
    } else if(this -> state == 6){
      Linetracer_do(this->linetracer, 0 ,0);
      if(ColorSensor_getReflectBrightness(this->colorsensor) == COLOR_YELLOW){
          this -> state = 7;
          blockLR(this,2,1);
      }
    } else if(this -> state == 7) {
      if(blockmove_deg(this,this->flag) == 1) {
        this -> state = 8;
        flag_reset(this);
      }
    } else if(this -> state == 8){
      if(next_sumou(this) == 1){
        this -> state = 9;
      }
    } else if(this -> state == 9){
        Linetracer_do(this->linetracer, 0 ,1);
        if(ColorSensor_getReflectBrightness(this->colorsensor) == COLOR_RED){
            this -> state = 10;
            blockLR(this,1, 0);
        }
    } else if(this -> state == 10){
        if(blockmove_ana(this,this->flag) == 1) {
          this -> state = 11;
          flag_reset(this);
        }
    } else if(this -> state == 11){
      if(BasicRun_Curve(this -> basicrun,RIGHTANGLE*2) == 1){
        this -> state = 12;
      }
    } else if(this -> state == 12){
      Linetracer_do(this->linetracer, 0 ,0);
      if(ColorSensor_getReflectBrightness(this->colorsensor) == COLOR_RED){
          this -> state = 13;
          blockLR(this,1,1);
      }
    } else if(this -> state == 13) {
        if(blockmove_deg(this,this->flag) == 1) {
          this -> state = 14;
          flag_reset(this);
        }
    } else if(this->state == 13){
      if(next_sumou1(this, 0) == 1){
        this->state = 100;
      }
  } else if(this->state == 100){
    Motor_stop(this -> left_motor,true);
    Motor_stop(this -> right_motor,true);
    state_reset(this);
    return 0;
  }
  return -1;
}

int next_sumou(AI_answer* this){
  if(this->sumou_state == 1){
    if(next_sumou1(this,90) == 1){
      this->sumou_state = 2;
    }
  } else if(this->sumou_state == 2){
    if(next_sumou2(this) == 1){
      this->sumou_state = 4;
    }
  } else if(this->sumou_state == 4){
    this->sumou_state = 1;
    return 1;
  }
  return 0;
}

int next_sumou2(AI_answer* this){
  if(this->state2 == 1){
    if(BasicRun_L_Curve(this -> basicrun_L,30) == 1){
      this -> state2 = 2;
    }
  } else if(this->state2 == 2){
    if(vertCurve(this->basicrun_L, 0) == 1){
      this->state2 = 3;
    }
  } else if(this->state2 == 3){
    this->state2 = 0;
    return 1;
  }
  return 0;
}

int next_sumou1(AI_answer* this, int addang){
  if(this->state2 == 1){
    if(BasicRun_L_Curve(this -> basicrun_L,45+addang) == 1){
      this -> state2 = 2;
    }
  } else if(this->state2 == 2){
    if(vertCurve(this->basicrun_L, 1) == 1){
      this->state2 = 3;
    }
  } else if(this->state2 == 3){
    this->state2 = 1;
    return 1;
  }
  return 0;
}


void blockLR(AI_answer* this, int bit,int deg_flag){
  if(deg_flag == 1){
    if(this->ans_num_deg/bit == 1){
      this->flag = 1;
    }	else {
      this->flag = 0;
    }
    this->ans_num_deg=this->ans_num_deg%bit;

  } else {
    if(this->ans_num_ana/bit == 1){
      this->flag = 1;
    }	else {
      this->flag = 0;
    }
    this->ans_num_ana=this->ans_num_ana%bit;
  }
}


int degiToAns(AI_answer* this) {
  if(this->state == 1){
    // if(BasicRun_L_Curve(this->basicrun_L,RIGHTANGLE*2) == 1){
    //   BasicRun_L_reset(this -> basicrun_L);
    //   this->state = 11;
    // }
    this -> state = 11;
  } else if(this->state == 11){
    if(BasicRun_L_GoStraight(this -> basicrun_L,(-1)*this->minus1_zero*MUGEN,GREEN, 1) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 12;
    }
  }else if(this->state == 12){
    if(BasicRun_L_GoStraight(this->basicrun_L,(-1)*this->minus1_zero*5,WHITE,0) != -1){
      BasicRun_reset(this->basicrun);
      this->state = 2;
    }
  } else if(this->state == 2) {
    if(BasicRun_L_Curve(this -> basicrun_L,this->minus1_zero*RIGHTANGLE) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 3;
    }
  } else if(this -> state == 3) {
    if(BasicRun_L_GoStraight(this -> basicrun_L, MUGEN, WHITE,1) == 1){ //ここは黒を見つけるまで
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 31;
    }
  } else if(this->state == 31){
    if(BasicRun_L_GoStraight(this -> basicrun_L, MUGEN, BRACK,1) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 32;
    }
  } else if(this->state == 32){
    if(BasicRun_L_GoStraight(this -> basicrun_L, 10, BRACK,0) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 4;
    }

  //ここから変更の余地あり
}else if(this -> state == 4) {
  // if(BasicRun_L_Curve(this -> basicrun_L,45) == 1){
  //   BasicRun_L_reset(this -> basicrun_L);
  //   this -> state = 5;
  // }
  if(next_sumou1(this, 0) == 1){
    this->state = 5;
  }
} else if(this -> state == 5) {
  if(next_sumou2(this) == 1){
    this->state = 100;
  }
} else if(this->state == 100){
  Motor_stop(this -> left_motor,true);
  Motor_stop(this -> right_motor,true);
  state_reset(this);
  return 0;
}
return -1;
}
//   }else if(this -> state == 4) {
//     if(BasicRun_L_GoStraight(this -> basicrun_L, 10, BRACK,0) != -1){ //ここは黒を見つけるまで
//       BasicRun_L_reset(this -> basicrun_L);
//       this->state = 5;
//     }
//   } else if(this->state == 5) {
//     if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE+1) == 1){
//       BasicRun_L_reset(this -> basicrun_L);
//       this -> state = 6;
//     }
//   } else if(this -> state == 6) {
//     if(BasicRun_L_GoStraight(this -> basicrun_L, MUGEN, BRACK,1) == 1){ //ここは黒を見つけるまで
//       BasicRun_L_reset(this -> basicrun_L);
//       this->state = 7;
//     }
//   } else if(this->state == 7) {
//     if(BasicRun_L_Curve(this -> basicrun_L,-RIGHTANGLE-1) == 1){
//       BasicRun_L_reset(this -> basicrun_L);
//       this -> state = 100;
//     }
//   } else if(this->state == 100){
//     Motor_stop(this -> left_motor,true);
//     Motor_stop(this -> right_motor,true);
//     state_reset(this);
//     return 0;
//   }
//   return -1;
// }

int LineToDegi(AI_answer* this){
  if(this->state == 1){
    if(BasicRun_L_GoStraight(this -> basicrun_L, MUGEN, GREEN,1) == 1){ //緑を見つけるまで
        this->state = 2;
    }
  } else if (this->state == 2) {
    if(BasicRun_L_Curve(this -> basicrun_L,-RIGHTANGLE+5) == 1){
      this->state = 3;
    }
  } else if(this->state == 3){
    if(BasicRun_L_GoStraight(this -> basicrun_L, 5, GREEN,0) == 1){ //緑を見つけるまで
        this->state = 4;
    }
  } else if(this->state == 4) {
    if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE) == 1){
      this->state = 5;
    }
  } else if(this->state == 5){
    if(BasicRun_L_GoStraight(this -> basicrun_L, 2, WHITE,0) == 1){ //緑を見つけるまで
        this->state = 6;
    }
  } else if(this->state == 6){
    if(BasicRun_L_GoStraight(this -> basicrun_L, MUGEN, WHITE,1) == 1){ //緑を見つけるまで
        this->state = 100;
    }
  } else if(this->state == 100){
    Motor_stop(this -> left_motor,true);
    Motor_stop(this -> right_motor,true);
    state_reset(this);
    return 1;
  }
  return 0;
}



void state_reset(AI_answer* this) {
  this -> state = 1;
}

void state2_reset(AI_answer* this) {
  this -> state2 = 1;
}

void flag_reset(AI_answer* this) {
  this -> flag = 0;
}

//ブロックを移動させる関数(analog),正常終了なら返り値1 otherwise 返り値0
int blockmove_ana(AI_answer* this,int direction){
    BasicRun_speedchange(this->basicrun,10);
    if (direction == 1) {
        if (this -> state2 == 1){
            if (BasicRun_Curve(this -> basicrun,RIGHT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 2;
            }
        }
        else if (this -> state2 == 2){
            if (BasicRun_GoStraight(this -> basicrun,5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 3;
            }
        }
        else if (this -> state2 == 3){
            if (BasicRun_GoStraight(this -> basicrun,-5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 4;
            }
        }
        else if (this -> state2 == 4){
            if (BasicRun_Curve(this -> basicrun,LEFT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 100;
                state2_reset(this);
                return 1;
            }
        }
    }
    else{
        if (this -> state2 == 1){
            if (BasicRun_Curve(this -> basicrun,LEFT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 2;
            }
        }
        else if (this -> state2 == 2){
            if (BasicRun_GoStraight(this -> basicrun,5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 3;
            }
        }
        else if (this -> state2 == 3){
            if (BasicRun_GoStraight(this -> basicrun,-5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 4;
            }
        }
        else if (this -> state2 == 4){
            if (BasicRun_Curve(this -> basicrun,RIGHT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 100;
                state2_reset(this);
                return 1;
            }
        }
    }
    return 0;
}

//ブロックを移動させる関数(degital),正常終了なら返り値1 otherwise 返り値0
int blockmove_deg(AI_answer* this,int direction){
    BasicRun_speedchange(this->basicrun,10);
    if (direction == 1) {
        if (this -> state2 == 1){
            if (BasicRun_Curve(this -> basicrun,LEFT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 2;
            }
        }
        else if (this -> state2 == 2){
            if (BasicRun_GoStraight(this -> basicrun,5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 3;
            }
        }else if (this -> state2 == 3){
            if (BasicRun_GoStraight(this -> basicrun,-5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 4;
            }
        }else if (this -> state2 == 4){
            if (BasicRun_Curve(this -> basicrun,RIGHT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 100;
                state2_reset(this);
                return 1;
            }
        }
    }else{
        if (this -> state2 == 1){
            if (BasicRun_Curve(this -> basicrun,RIGHT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 2;
            }
        }
        else if (this -> state2 == 2){
            if (BasicRun_GoStraight(this -> basicrun,5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 3;
            }
        }
        else if (this -> state2 == 3){
            if (BasicRun_GoStraight(this -> basicrun,-5) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 4;
            }
        }
        else if (this -> state2 == 4){
            if (BasicRun_Curve(this -> basicrun,LEFT) == 1) {
                BasicRun_reset(this -> basicrun);
                this -> state2 = 100;
                state2_reset(this);
                return 1;
            }
        }
    }
    return 0;
}



int basicrunL_test(AI_answer* this){
  if (this -> state == 1) {
    // sprintf(lcdstr, "AI_state 1");
    // ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
    hoge = BasicRun_L_GoStraight(this -> basicrun_L, tate+1, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 2;
    }
  } else if(this -> state == 2){
    BasicRun_L_speedchange(this->basicrun_L,10);
    if(BasicRun_L_Curve(this -> basicrun_L,RIGHTANGLE*4) == 1){
      BasicRun_L_reset(this -> basicrun_L);
      this -> state = 3;
    }
  } else if(this -> state == 3){
    hoge = BasicRun_L_GoStraight(this -> basicrun_L, tate, BRACK, 0);
    if(hoge != -1){
      BasicRun_L_reset(this -> basicrun_L);
      this->state = 4;
    }
  } else if (this->state ==4){
      Motor_stop(this -> left_motor,true);
      Motor_stop(this -> right_motor,true);
      return 1;

  }
    // BasicRun_L_speedchange(this->basicrun_L,10);
    // if(BasicRun_L_Curve(this -> basicrun_L,-RIGHTANGLE*2) == 1){
    //   BasicRun_L_reset(this -> basicrun_L);
    //   this -> state = 1;
    // }
    return 0;
}

int vertCurve_test(AI_answer* this){
  if(this->state == 1) {
    if(vertCurve(this->basicrun_L, 1) == 1){
      this->state = 2;
    }
  }else if(this->state == 2){
    return 1;

  }
  return 0;
}


int go_park(AI_answer* this){
  if(this->state == 1){
    if(BasicRun_L_GoStraight(this -> basicrun_L, MUGEN, WHITE, 1)==1){
        this->state++;
    }
  }else if(this->state==2){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, GREEN, 1)==1){
      this->state=25;
    }
  }else if(this->state==25){
    if(BasicRun_L_GoStraight(this->basicrun_L, 8, GREEN, 0)==1){
      this->state=3;
    }
  }else if(this->state==3){
    if(BasicRun_L_Curve(this->basicrun_L, -82)==1){
      this->state=35;
    }
  }else if(this->state==35){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, WHITE, 1)==1){
      this->state=4;
    }
  }
  else if(this->state==4){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, BRACK, 1)==1){
      this->state++;
    }
  }else if(this->state==5){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, WHITE, 1)==1){
      this->state++;
    }
  }else if(this->state==6){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, BRACK, 1)==1){
      this->state++;
    }
  }else if(this->state==7){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, WHITE, 1)==1){
      this->state++;
    }
  }else if(this->state==8){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, BRACK, 1)==1){
      this->state++;
    }
  }else if(this->state==9){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, WHITE, 1)==1){
      this->state++;
    }
  }else if(this->state==10){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, BRACK, 1)==1){
      this->state++;
    }
  }else if(this->state==11){
    if(BasicRun_L_GoStraight(this->basicrun_L, MUGEN, WHITE, 1)==1){
      this->state++;
      Motor_stop(this -> left_motor,true);
      Motor_stop(this -> right_motor,true);
    }
  }else if(this->state==12){
    if(BasicRun_L_Curve(this->basicrun_L, 45)==1){
      this->state = 0;
      return 1;
    }
  }
  return 0;

}

/**
 ******************************************************************************
 ** ファイル名 : app.c
 **
 ** 概要 : 2輪倒立振子ライントレースロボットのTOPPERS/HRP2用Cサンプルプログラム
 **
 ** 注記 : sample_c4 (sample_c3にBluetooth通信リモートスタート機能を追加)
 ******************************************************************************
 **/

//使うソースコードをすべてインクルードする
//ファイルを増やした場合は、app.cfgとMakefile.incを前の例にしたがって書き換える
#include "D_BluetoothSender.h"
#include "D_ColorSensor.h"
#include "D_GyroSensor.h"
#include "D_Motor.h"
#include "D_SonarSensor.h"
#include "D_TouchSensor.h"
#include "ev3api.h"
#include "app.h"
#include "Calibrator.h"
#include "Prep_Arm.h"
#include "PID_PID.h"
#include "Linetracer.h"

#include "Rcourse.h"
#include "Train_Timer.h"
#include "Gray_detection.h"
#include "BasicRun_L.h"
#include "AI_answer.h"

#include "Lcourse.h"
#include "LgameCalc.h"
#include "LgameMove.h"
#include "Dijk.h"
#include "EdgeChanger.h"


//おまじない感あふれる箇所
#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/**
 * センサー、モーターの接続を定義する
 */
static const sensor_port_t
    touchsensor_p    = EV3_PORT_1,
    colorsensor_p    = EV3_PORT_2,
	  sonarsensor_p    = EV3_PORT_3,
    gyrosensor_p     = EV3_PORT_4;

static const motor_port_t
    left_motor_p      = EV3_PORT_C,
    right_motor_p     = EV3_PORT_B,
    arm_motor_p      = EV3_PORT_A,
	  tail_motor_p		= EV3_PORT_D;
static int      bt_cmd = 0;     /* Bluetoothコマンド 1:リモートスタート */
static FILE     *bt = NULL;     /* Bluetoothファイルハンドル */
FILE 	*testfile = NULL;
FILE 	*logging = NULL;

char lcdstr[100];

int count_t = 0;
int l_state =1;
int logcount =0;
int flag =true;
int loggingflag=true;
int state = 0;
int time1 =0;
int base_count =103; //環境次第,事前に調べる
int target_count = 0;
int R_course = 1;  //1:Rコース0:Lコース

int analog_num;
int degital_num;
int line_count=0;
int green_count = 0;

//デバイスクラスのインスタンス作成
TouchSensor touchsensor;
ColorSensor colorsensor;
SonarSensor sonarsensor;
GyroSensor gyrosensor;

Motor left_motor;
Motor right_motor;
Motor arm_motor;
Motor tail_motor;


//その他クラスのインスタンス作成
Calibrator calibrator;
Prep_Arm prep_arm;
PID pid;
Linetracer linetracer;
BasicRun basicRun;
BasicRun_L basicRun_L;

Rcourse rcourse;
Sumo_Action sumo_action;

Light_Lightdetector lightdetector;
Driver driver;
LPF_LPF lpf;
Gray_detection gray_detection;

AI_answer ai_answer;

Lcourse   lcourse;
LgameCalc lgamecalc;
LgameMove lgamemove;
Dijk      dijk;
EdgeChanger edgechanger;


/* 下記のマクロは個体/環境に合わせて変更する必要があります */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* LCDフォントサイズ */
//#define CALIB_FONT (EV3_FONT_SMALL)
//#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
//#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)
#define CALIB_FONT (EV3_FONT_MEDIUM)
#define CALIB_FONT_WIDTH (12/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (18/*TODO: magic number*/)

/* 関数プロトタイプ宣言 */


//初期化関数
void ev3_init(){
	//デバイスの初期化
	TouchSensor_init(&touchsensor,touchsensor_p);
	ColorSensor_init(&colorsensor,colorsensor_p);
	SonarSensor_init(&sonarsensor,sonarsensor_p);
	GyroSensor_init(&gyrosensor,gyrosensor_p);

	//モータの初期化
	Motor_init(&left_motor,left_motor_p,LARGE_MOTOR);
	Motor_init(&right_motor,right_motor_p,LARGE_MOTOR);
	Motor_init(&arm_motor,arm_motor_p,LARGE_MOTOR);
	Motor_init(&tail_motor,tail_motor_p,MEDIUM_MOTOR);

	//その他クラスの初期化
  PID_init(&pid,&calibrator);
	Calibrator_init(&calibrator,&colorsensor,&left_motor,&right_motor);
	arm_init(&prep_arm,&arm_motor);
	Linetracer_init(&linetracer, &pid, &lightdetector, &driver);
	BasicRun_init(&basicRun,&left_motor,&right_motor);
  BasicRun_L_init(&basicRun_L,&touchsensor, &left_motor,&right_motor,&colorsensor, &lightdetector, &calibrator);

  Start_Sumou_init(&rcourse, &basicRun, &colorsensor, &sonarsensor, &arm_motor, &sumo_action, &linetracer, &pid, &driver, &lightdetector, &gyrosensor, &gray_detection, &left_motor, &right_motor);
  Sumo_Action_init(&sumo_action, &basicRun, &arm_motor, &tail_motor);
  Driver_init(&driver, &left_motor, &right_motor);
  Lightdetector_init(&lightdetector, &colorsensor, &lpf);
  Gray_detection_init(&gray_detection, &colorsensor);


  AI_answer_init(&ai_answer, &colorsensor, &basicRun, &basicRun_L, &left_motor, &right_motor, &linetracer);
  Lcourse_init(&lcourse, &linetracer, &edgechanger, &lgamecalc, &lgamemove, &dijk, &basicRun, &pid, &arm_motor, &left_motor, &right_motor);
  LgameMove_init(&lgamemove, &colorsensor, &basicRun, &arm_motor);
  Dijk_init(&dijk);
  LgameCalc_init(&lgamecalc, &dijk);


}


/* 初期化タスク*/
void init_task(intptr_t unused)
{	//char filename[30];
	if(flag==true){
	ev3_init();
	 //bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	//char timestr[30];

//	int filenumber=1;
  /**
	while(1){
		sprintf(filename,"Linetrace_%d.csv",filenumber);
		if ((testfile = fopen(filename, "r")) == NULL) {
				fclose(testfile);
				break;
			}
		filenumber++;
		}

	if ((logging = fopen(filename, "w")) == NULL) {
			printf("file open error!!\n");
			ext_tsk();
	}
  **/
	//fprintf(logging,"Time,Battery,LMoter,RMoter,AMoter,TMoter,Gyro,Touch,Reflect,Red,Green,Blue,Sonar,Data1,Data2,tiradius,light_level,distance\n");
	flag=false;
	}
	//ulong_t correnttime;

    /* LCD画面表示 */
	  //ev3_lcd_set_font(CALIB_FONT);
    //ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    //ev3_lcd_draw_string(filename, 0, CALIB_FONT_HEIGHT*1);
    /* Open Bluetooth file */
//ここが実行されない限りメインタスクはストップする
    set_flg(FLG1,0x01);
    set_flg(FLG2,0x01);
    ext_tsk();
}


    /**
    * メインループ countが1000になるまで繰り返される
    */
 void main_task(intptr_t unused){
	 FLGPTN flag;

   //白と黒の閾値（0と255は適当）
   //int threshold_white = 0;
   //int threshold_black = 255;

   //ここでストップする
	 wai_flg(FLG1, 0x01, TWF_ANDW, &flag);
	 //アームを上げる

   if(state==0){
		 if(arm_rize(&prep_arm) == true){
		 	state=1;
		 }
	 }
	 //タッチセンサーが押されるまで待機
	 else if(state==1){
    		ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
    		if(TouchSensor_getState(&touchsensor)== true){
    			state=2;//state=2;
    		}
    	}






   else if(state==100){
        if(TouchSensor_getState(&touchsensor)==true){
          pid.kp = Motor_getAngle(&left_motor) * 0.01;
          pid.kd = Motor_getAngle(&right_motor) * 0.001;
          pid.ki = Motor_getAngle(&tail_motor) * 0.1;
          state=2;
        }
      }
	 //キャリブレーションを行う
	 else if(state==2){
        sprintf(lcdstr, "state 2");
        ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示

    		ev3_led_set_color(LED_GREEN); /* スタート通知 */
    		if(Calibrator_calibrate(&calibrator)==true){
    			//Linetracer_set_boundly(&linetracer,Calibrator_get_boundary(&calibrator));
    			state = 4;//通常state=3, 便利ツールstate=24(base_count チェッカーくん)
    		}
    	}

    //Rコース用??
    else if(state == 3){
        sprintf(lcdstr, "state 4");
        ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
        ev3_led_set_color(LED_GREEN); /* 受信待ち状態 */
        bt = ev3_serial_open_file(EV3_SERIAL_BT);
        //fprintf(bt, "Bluetooth SPP ID: %d\n", EV3_SERIAL_BT);
        int c = fgetc(bt);
        switch(c){
          case '1':
            bt_cmd = 1;
            state = 5;
          break;
          default:
          break;
        }
        fputc(c, bt); /* エコーバック */
      }

	 //タッチセンサーが押されるまで待機
   	else if(state ==4){
       sprintf(lcdstr, "state 3");
       ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
  		 ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
       PID_init(&pid,&calibrator);

       lcdfont_t font = EV3_FONT_MEDIUM;
       ev3_lcd_set_font(font);
       int32_t fontw, fonth;
       ev3_font_get_size(font, &fontw, &fonth);

       if(ev3_bluetooth_is_connected() == 1){sprintf(lcdstr, "true");}
       else{sprintf(lcdstr, "false");}

       ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示

      //試験用に分岐可能,通常はstate=5
  		if (TouchSensor_getState(&touchsensor)== true){
  			state = 5;//ライントレース
        // state = 1ß111; //便利ツール
        // state = 11;//駐車
        //state = 25;//ライン復帰テスト用
        //state = 125;//色チェックプログラム
        //state = 1212;//シャレオツフェードイン
  		}
  	}

    else if(state==1212){
      if(Line_fukki(&basicRun_L,base_count,1)==1){
        count_t=0;
        state=5;
      }


    }


	 else if(state==5){
     //左側ライントレース
     sprintf(lcdstr, "state 5");
     ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
     //R_cource=1で固定
     if(R_course){
       //Rコース
       //count_t=9000;
       count_t += 1;
       if(count_t<15000){
         Linetracer_do(&linetracer, 1, 1);
       //}else if(count_t<8000){
         //Linetracer_do(&linetracer, 0, 1);
         //Linetracer_do(&linetracer, 1, 1);
       //}else if(count_t<14000){
         //Linetracer_do(&linetracer, 1, 0);
       }else{
         Linetracer_do(&linetracer, 1, 2);//通常速度
         if(ColorSensor_getColor(&colorsensor)==4){
           state=333;
         }
         //Linetracer_do(&linetracer, 0, 1);//低速、カーブが無理ならこっちで
         //int f=ColorSensor_getColor(&colorsensor);
         //if(Gray_detection_do(&gray_detection)){//1回目の青検知
          //count_t=0;
          //state=998;//1回目の青を読み込み、とりあえず停止

       }
      }else{
        count_t += 1;
        if(count_t<4000){
          Linetracer_do(&linetracer, 0, 1);
          //if(ColorSensor_getColor(&colorsensor)==4){
            //state=333;

        }else if(count_t<8000){
          Linetracer_do(&linetracer, 0, 1);
        }else if(count_t<14000){
          Linetracer_do(&linetracer, 0, 0);

        }else{
          Linetracer_do(&linetracer, 0, 2);//通常速度
          if(ColorSensor_getColor(&colorsensor)==4){
            state=333;
          }

          //Linetracer_do(&linetracer, 0, 1);//低速、カーブが無理ならこっちで
             //if(ColorSensor_getColor(&colorsensor)==2){//1回目の青検知
               //count_t=0;
               //state=1002;//1回目の青を読み込み、とりあえず停止
             //}
          //if(Gray_detection_do(&gray_detection)){//1回目の青検知
          //if(ColorSensor_getColor(&colorsensor)==4){
            //count_t=0;
            //state=998;//1回目の青を読み込み、とりあえず停止

         }
       }

   }else if(state==333){
     if(BasicRun_GoStraight(&basicRun,110)==1){
       state=334;
     }
   }else if(state==334){
     if(BasicRun_Curve(&basicRun , 90)==1){
     //if(BasicRun_GoStraight(&basicRun , 100)==1){
       state = 335;
     }
   }else if(state==335){
     if(BasicRun_GoStraight(&basicRun , 20)==1){
       state=336;
     }
   }else if(state==336){
     if(BasicRun_GoStraight(&basicRun , -20)==1){
       state=337;
     }
   }else if(state==337){
     if(BasicRun_Curve(&basicRun , -90)==1){
       state=13;
     }




        //昨年のLコース
        //通常運転のみでok
   }else if(state==998){
          //左側ライントレース
        sprintf(lcdstr, "state 998");
        ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
        //R_cource=1で固定
        if(R_course){
          //Rコース
          //count_t=9000;
          count_t += 1;
          if(count_t<200){
            Linetracer_do(&linetracer, 1, 0);
          }else{
            Linetracer_do(&linetracer, 1, 0);//通常速度
            if(Gray_detection_do(&gray_detection)){//2回目の青検知
              state=999;//2回目の青を読み込み、とりあえず停止
            }
          }
        }else{
             count_t += 1;
             if(count_t<200){
               Linetracer_do(&linetracer, 0, 0);
             }else{
               Linetracer_do(&linetracer, 0, 0);//通常速度
               //Linetracer_do(&linetracer, 0, 1);//低速、カーブが無理ならこっちで
               if(Gray_detection_do(&gray_detection)){
                 state=999;//2回目の青を読み込み、とりあえず停止
               }
              }
        }

   }else if(state==999){
            //左側ライントレース
            sprintf(lcdstr, "state 999");
            ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
            if(R_course){
            //Rコース
            //count_t += 1;
                Linetracer_do(&linetracer, 1, 1);
                if(ColorSensor_getColor(&colorsensor)==4){
                  state=1002;//黄色を読み込み、ビンゴプログラムに切り替え
                  //state=13;
                  //駐車プログラム
                }
            }else{
            //Lコース
                Linetracer_do(&linetracer, 0, 1);
                if(ColorSensor_getColor(&colorsensor)==4){
                  state=1002;
                }
              }

        /*
        Linetracer_do(&linetracer, 0, 0);
        // if(ColorSensor_getColor(&colorsensor)==3){//緑を検知したら
        int normal_light;
        int lpf_light;
        Lightdetector_get(&lightdetector, &normal_light, &lpf_light);
        rgb_raw_t rgb_score;
        ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb_score);
        if (rgb_score.r < 30 && rgb_score.g >= 46 && rgb_score.b >= 46) {
          state = 6;
        }
        */
        //if(normal_light <= 8 && normal_light >= 5) {
          // green_count++;
          // if(green_count >= 40) {
          //   state = 6;
          // }
        // if(ColorSensor_getColor(&colorsensor)==5){//簡易R用,赤までトレース
          // state = 6;//Lコース競技へ
        // } else {
        //   green_count = 0;
        // }

      // 27 48 52


   }else if(state==1000){
     if(Lcourse_task(&lcourse) == 1){
       state = 1001;
     }
  }

  else if(state==1001){
    if(BasicRun_GoStraight(&basicRun,70)==1){
      state = 1002;
    }
  }

  else if(state==1002){
    Motor_stop(&left_motor,true);
    Motor_stop(&right_motor,true);
  }

   else if(state==6){
     sprintf(lcdstr, "state 6");
     ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示

     //とりま緑と（数字の周りの）白の間まで行く
     // if(LineToDegi(&ai_answer) == 1){
     //   state = 7;
     //   // state = 11;
     // }
     if(go_park(&ai_answer) == 1) {
       state = 11;
     }
     // state = 7;
   }

   else if(state==7){
     //デジタル数字読み取り
     // sprintf(lcdstr, "state 7");
     // ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示

     // degital_num = 1;
     // state = 8;
     // if(basicrunL_test(&ai_answer) == 1){
     //   state = 11;
     // }
     // if(vertCurve_test(&ai_answer) == 1){
     //   ev3_led_set_color(LED_GREEN);
     //   state = 11;
     // }
     degital_num =  degital_recog(&ai_answer);
     if(degital_num != -1) {
       state = 8;
     }
   }

   else if(state==8){
     // sprintf(lcdstr, "state 8");
     // ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示
     //ふにゃふにゃ数字読み取り
     // int analog_num = analog_recog(); //まだない
     analog_num = 1;
     state = 9;
   }

   // degi -> ans までの動き
   else if(state == 9) {
     int flag = degiToAns(&ai_answer);
     if(flag != -1){
       state = 10;
     }
     // state = 10;
   }

   else if(state==10){
     //すもう
     if(sumou_do(&ai_answer, analog_num, degital_num) != -1){
       state = 11;
     }
     // if(blockmove_deg(&ai_answer, 0)==1){
     //  return;
     // }
   }

   else if(state==11){
     //駐車part1
     //ライン復帰
     if(Line_fukki(&basicRun_L,base_count,0)==1){
       count_t=0;
       state=12;
     }
   }

   else if(state==12){
     //駐車part2
     //ちょこっとライントレース,超低速版
     Linetracer_do(&linetracer, 0, 0);
     count_t+=1;
     if(count_t>200){     //ここここここここおこここここここここここ
       state=13;
     }
   }

   else if(state==13){
     //駐車Part3
     //各コース毎の駐車ムーブ
     //Motor_setPower(&left_motor,10);
     //Motor_setPower(&right_motor,10);
    if(R_course){
         //Rコース駐車
      state =14;
    }else{
         //Lコース駐車
      state = 15;//L駐車
    }
  }

   //Rコース駐車
   else if(state ==14){
     //まず直進
     if(l_state==0){
       if(BasicRun_GoStraight(&basicRun , 10)==1){
       //if(BasicRun_GoStraight(&basicRun , 100)==1){
         l_state = 1;
       }
     //弧を描いて駐車スペースへ
   }else if(l_state==1){
       if(BasicRun_Curve(&basicRun,90)==1){
         l_state=2;
       }
   }else if(l_state==2){
       if(BasicRun_Kido(&basicRun,95)==1){
         l_state=3;
       }
   }else if(l_state==3){
       if(BasicRun_GoStraight(&basicRun,15)==1){
         l_state=4;
       }

     //最後に角度の調整
     //モーターストップ
   }else if(l_state==4){
       state=17;
     }
   }

   //Lコース駐車
   else if(state == 15){
     //駐車スペースに向かって直進
     if(BasicRun_GoStraight(&basicRun , 50)==1){
       state = 16;
     }
   }
   else if(state==16){
     if(BasicRun_Curve(&basicRun,-85)==1){
       state=17;
     }
   }
   else if(state==17){
     Motor_stop(&left_motor,true);
     Motor_stop(&right_motor,true);
   }


//---------------------------試験用プログラム-------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//ライン復帰トレース試験用25-27
else if(state==25){
  if (count_tama(&basicRun_L) == 1){
    target_count = get_tama(&basicRun_L);
    reset_tama(&basicRun_L);
    state = 26;
  }
}
else if(state==26){
  if (tamaCurve(&basicRun_L,base_count,target_count) == 1){
    target_count = 0;
    state = 27;
  }
}







//----------------------------以下便利ツール--------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//base_countを表示する,競技前に繰り返して平均値を使う
else if(state==1112) {
  ev3_lcd_set_font (EV3_FONT_MEDIUM);
  if (count_tama(&basicRun_L) == 1){
    sprintf(lcdstr, "base_count(%d)=%d",line_count,get_tama(&basicRun_L));
    ev3_lcd_draw_string(lcdstr,0,line_count*20);
    reset_tama(&basicRun_L);
    line_count = line_count + 1;
    state = 1111;
  }
} else if(state == 1111){
  if(TouchSensor_getState(&touchsensor)== true){
    state=1112;//state=2;
    line_count = 0;
  }
}




//base_countを表示する,競技前に繰り返して平均値を使う
else if(state==24) {
  if (count_tama(&basicRun_L) == 1){
    sprintf(lcdstr, "base_count=%d",get_tama(&basicRun_L));
    reset_tama(&basicRun_L);
    state = 2;
  }
}


//check color and brightness
  else if(state==125){
    lcdfont_t font = EV3_FONT_MEDIUM;
    ev3_lcd_set_font(font);
    int32_t fontw, fonth;
    ev3_font_get_size(font, &fontw, &fonth);
  sprintf(lcdstr, "brightness=%d",  ColorSensor_getReflectBrightness(&colorsensor));
  ev3_lcd_draw_string(lcdstr,0,0);
  int k = ColorSensor_getColor(&colorsensor);
  if(k==0){
    sprintf(lcdstr, "color=ERROR");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==1){
    sprintf(lcdstr, "color=BLACK");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==2){
    sprintf(lcdstr, "color=BLUE");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==3){
    sprintf(lcdstr, "color=GREEN");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==4){
    sprintf(lcdstr, "color=YELLOW");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==5){
    sprintf(lcdstr, "color=RED");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==6){
    sprintf(lcdstr, "color=WHITE");
    ev3_lcd_draw_string(lcdstr,0,30);
  }else if(k==7){
    sprintf(lcdstr, "color=BROWN");
    ev3_lcd_draw_string(lcdstr,0,30);
  }
}
    ext_tsk();
}


//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
void bt_task(intptr_t unused)
{
    while(1)
    {
        uint8_t c = fgetc(bt); /* 受信 */
        switch(c)
        {
        case '1':
            bt_cmd = 1;
            break;
        default:
            break;
        }
        fputc(c, bt); /* エコーバック */
    }
}
void bt_logging_task(intptr_t unused)
{
	ulong_t correnttime;
    while(1)
    { 	get_tim(&correnttime);
    	printf("%ld",correnttime);
        //fprintf("",); /* エコーバック */
    }
}
//*****************************************************************************
// 関数名 : sd_logging_task
// 引数 : unused
// 返り値 : なし
// 概要 : 200ms毎に起動し様々なセンサーの値を取得、SDカードに書き込む
// ふたつ目のfprintfに見たい値を入れれば出力される
//*****************************************************************************
void sd_logging_task(intptr_t unused)
{	 FLGPTN flag;
//ここでストップする
	wai_flg(FLG2, 0x01, TWF_ANDW, &flag);
	ulong_t correnttime = getTime();
if(loggingflag==false){
    int battery = ev3_battery_voltage_mV();
    int lpower = Motor_getPower(&left_motor);
    int rpower = Motor_getPower(&right_motor);
    int apower = Motor_getPower(&arm_motor);
    int tpower = Motor_getPower(&tail_motor);
    int gyro =	 GyroSensor_getAngle(&gyrosensor);
    int touch = TouchSensor_getState(&touchsensor);
    int reflect = ColorSensor_getReflectBrightness(&colorsensor);
	//int ambient = ColorSensor_getAmbientBrightness(&colorsensor);

	//rgb_raw_t rgb;
	//ColorSensor_getRawColor(&colorsensor,&rgb);
//	int r = rgb.r;
//	int g = rgb.g;
//	int b = rgb.b;
	//
	int r = 0;
	int g = 0;
	int b = 0;

	int sonar =SonarSensor_getDistance(&sonarsensor);
	if(logcount%5==0){
	char comment[50];
	sprintf(comment,"%d",reflect);
	//ev3_lcd_draw_string(comment, 0, CALIB_FONT_HEIGHT*2);
	}
	logcount++;
   	fprintf(logging,"%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,",correnttime,battery,lpower,rpower,apower,tpower,gyro,touch,reflect,r,g,b,sonar);
   	fprintf(logging,"%f,%f,%f,%d,%d\n",linetracer.pid->MVn_1,linetracer.pid->MVn,driver.radius,pid.light_level,rcourse.distance);
//    fprintf("",); /* エコーバック */
}
else if(loggingflag==false){
	fclose(logging);
}
	ext_tsk();
}

ulong_t getTime()
{
  static ulong_t start = -1;
  ulong_t time;

  get_tim(&time);

  if(start < 0){
    start = time;
  }

  return time - start;
}

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
#include "AI_answer.h"


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

int count = 0;
int logcount =0;
int flag =true;
int loggingflag=true;
int state = 0;
int time1 =0;

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

Rcourse rcourse;
Sumo_Action sumo_action;

Light_Lightdetector lightdetector;
Driver driver;
LPF_LPF lpf;
Gray_detection gray_detection;


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
  PID_init(&pid);
	Calibrator_init(&calibrator,&colorsensor,&left_motor,&right_motor);
	arm_init(&prep_arm,&arm_motor);
	Linetracer_init(&linetracer, &pid, &lightdetector, &driver);
	BasicRun_init(&basicRun,&left_motor,&right_motor);

  Start_Sumou_init(&rcourse, &basicRun, &colorsensor, &sonarsensor, &arm_motor, &sumo_action, &linetracer, &pid, &driver, &lightdetector, &gyrosensor, &gray_detection, &left_motor, &right_motor);
  Sumo_Action_init(&sumo_action, &basicRun, &arm_motor, &tail_motor);
  Driver_init(&driver, &left_motor, &right_motor);
  Lightdetector_init(&lightdetector, &colorsensor, &lpf);
  Gray_detection_init(&gray_detection, &colorsensor);
}


/* 初期化タスク*/
void init_task(intptr_t unused)
{	char filename[30];
	if(flag==true){
	ev3_init();
	 //bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);
	//char timestr[30];
	int filenumber=1;
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
	fprintf(logging,"Time,Battery,LMoter,RMoter,AMoter,TMoter,Gyro,Touch,Reflect,Red,Green,Blue,Sonar,Data1,Data2,tiradius,light_level,distance\n");
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
   treshold_white = 0;
   treshold_black = 255;

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
    		 ev3_led_set_color(LED_GREEN); /* スタート通知 */
    		if(Calibrator_calibrate(&calibrator)==true){
    			//Linetracer_set_boundly(&linetracer,Calibrator_get_boundary(&calibrator));
    			state = 3;
    		}
    	}
	 //タッチセンサーが押されるまで待機
   	else if(state ==3){
  		 ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */

       lcdfont_t font = EV3_FONT_MEDIUM;
       ev3_lcd_set_font(font);
       int32_t fontw, fonth;
       ev3_font_get_size(font, &fontw, &fonth);

       if(ev3_bluetooth_is_connected() == 1){sprintf(lcdstr, "true");}
       else{sprintf(lcdstr, "false");}

       ev3_lcd_draw_string(lcdstr,0,0);   //EV3の画面に表示

  		if (TouchSensor_getState(&touchsensor)== true){
  			state = 5;
  		}
  	}


    else if(state == 4){
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

	 else if(state==5){
     //灰色検知するまで左側ライントレース
     degital_recog();
   }

   else if(state==6){
     //とりま緑と（数字の周りの）白の間まで行く
   }

   else if(state==7){
     //デジタル数字読み取り
     int degital_num =  degital_recog(); //まだない
   }

   else if(state==8){
     //ふにゃふにゃ数字読み取り
     int analog_num = analog_recog(); //まだない
   }

   else if(state==9}{
     //すもう
     sumou_do(analog_num, degital_num); //まだない
   }

   else if(state==10){
     //駐車
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
if(loggingflag==true){
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

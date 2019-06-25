#include "LgameMove.h"
#include "ev3api.h"

void RGB_task(rgb_raw_t *rgb_val)
{
	// RGB RAW値を取得
	ev3_color_sensor_get_rgb_raw(EV3_PORT_2, rgb_val);
}

//Lコースのゲームパートの計算の機能の初期化
void LgameMove_init(LgameMove* this, ColorSensor* colorsensor, BasicRun* basicrun, Motor* arm_motor){
  this->mColorSensor = colorsensor;
  this->mBasicRun = basicrun;
  this->small_state = 0;
  this->arm_motor = arm_motor;
}

//次のラインに移る．
int LgameMove_nextline(LgameMove* this, int n1, int n2, int n3){

  //  ライン参照マップ（17ｘ16）
  //					   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15
int map_node[17][16] = { { M, 0, M, M, 3, M, M, M, M, M, M, M, M, M, M, M}, //0
						 { 0, M, 1, M, M, 4, M, M, M, M, M, M, M, M, M, M}, //1
						 { M, 1, M, 2, M, M, 5, M, M, M, M, M, M, M, M, M}, //2
						 { M, M, 2, M, M, M, M, 6, M, M, M, M, M, M, M, M}, //3
						 { 3, M, M, M, M, 7, M, M,10, M, M, M, M, M, M, M}, //4
						 { M, 4, M, M, 7, M, 8, M, M,11, M, M, M, M, M, M}, //5
						 { M, M, 5, M, M, 8, M, 9, M, M,12, M, M, M, M, M}, //6
						 { M, M, M, 6, M, M, 9, M, M, M, M,13, M, M, M, M}, //7
						 { M, M, M, M,10, M, M, M, M,14, M, M,17, M, M, M}, //8
						 { M, M, M, M, M,11, M, M,14, M,15, M, M,18, M, M}, //9
						 { M, M, M, M, M, M,12, M, M,15, M,16, M, M,19, M}, //10
						 { M, M, M, M, M, M, M,13, M, M,16, M, M, M, M,20}, //11
						 { M, M, M, M, M, M, M, M,17, M, M, M, M,21, M, M}, //12
						 { M, M, M, M, M, M, M, M, M,18, M, M,21, M,22, M}, //13
						 { M, M, M, M, M, M, M, M, M, M,19, M, M,22, M,23}, //14
						 { M, M, M, M, M, M, M, M, M, M, M,20, M, M,23, M}, //15
						 { M, M, M, M, M, M, M, M,24, M, M, M, M, M, M, M}  //16(入口前)
						 };

                      //左曲がりが正、右曲がりが負
//角度参照マップ(25✕24) {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}
int map_line[25][24] = { {180,  0,  M, 90,-90,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //0
						 {  0,180,  0,  M, 90,-90,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //1
						 {  M,  0,180,  M,  M, 90,-90,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //2
						 {-90,  M,  M,180,  M,  M,  M, 90,  M,  M,  0,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //3
						 { 90,-90,  M,  M,180,  M,  M,-90, 90,  M,  M,  0,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //4
						 {  M, 90,-90,  M,  M,180,  M,  M,-90, 90,  M,  M,  0,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //5
						 {  M,  M, 90,  M,  M,  M,180,  M,  M,-90,  M,  M,  M,  0,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //6
						 {  M,  M,  M,-90, 90,  M,  M,180,  0,  M, 90,-90,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //7
						 {  M,  M,  M,  M,-90, 90,  M,  0,180,  0,  M, 90,-90,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //8
						 {  M,  M,  M,  M,  M,-90, 90,  M,  0,180,  M,  M, 90,-90,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M}, //9
						 {  M,  M,  M,  0,  M,  M,  M,-90,  M,  M,  M,  M,  M,  M, 90,  M,  M,  0,  M,  M,  M,  M,  M,  M}, //10
						 {  M,  M,  M,  M,  0,  M,  M, 90,-90,  M,  M,180,  M,  M,-90, 90,  M,  M,  0,  M,  M,  M,  M,  M}, //11
						 {  M,  M,  M,  M,  M,  0,  M,  M, 90,-90,  M,  M,180,  M,  M,-90, 90,  M,  M,  0,  M,  M,  M,  M}, //12
						 {  M,  M,  M,  M,  M,  M,  0,  M,  M, 90,  M,  M,  M,180,  M,-90,  M,  M,  M,  M,  0,  M,  M,  M}, //13
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,-90, 90,  M,  M,180,  0,  M, 90,-90,  M,  M,  M,  M,  M}, //14
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,-90, 90,  M,  0,180,  0,  M, 90,-90,  M,  M,  M,  M}, //15
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,-90, 90,  M,  0,180,  M,  M, 90,-90,  M,  M,  M}, //16
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  0,  M,  M,  M,-90,  M,  M,180,  M,  M,  M, 90,  M,  M}, //17
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  0,  M,  M, 90,-90,  M,  M,180,  M,  M,-90, 90,  M}, //18
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  0,  M,  M, 90,-90,  M,  M,180,  M,  M,-90, 90}, //19
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  0,  M,  M, 90,  M,  M,  M,180,  M,  M,  M}, //20
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,-90, 90,  M,  M,180,  0,  M}, //21
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,-90, 90,  M,  0,180,  0}, //22
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,  M,-90, 90,  M,  0,180}, //23
						 {  M,  M,  M,  M,  M,  M,  M,  M,  M,  M, 90,  M,  M,  M,  0,  M,  M,-90,  M,  M,  M,  M,  M,  M}  //24(入口部分のライン)
						 };

  int Forward_line = map_node[n1][n2], Next_line = map_node[n2][n3];
  int Curve_angle = map_line[Forward_line][Next_line]; //曲がる角度を行列から取得(左曲がりなら正、右曲がりなら負)
  if(Curve_angle == 180) {
    return 1; //旋回はせずに呼び出し元に返す(ブロックを持っている場合があるため)
  }
  BasicRun_Curve(this->mBasicRun , -Curve_angle); //basicRunのインスタンス（オブジェクト）はあるつもりで書いてます．
  return 0;
}

//ノードかどうかを判断する
//同色をc回連続で検知したらノード到着とする．
//青の場合のみbrightnessで灰色と区別する
int LgameMove_findcolor(LgameMove* this) {
  colorid_t colorId = ColorSensor_getColor(this->mColorSensor);
  int colorflag;
  rgb_raw_t rgb_score;
  RGB_task(&rgb_score);

  //getcolorの青色検知を拒絶する
  if (colorId == COLOR_BLUE) {
	  colorId = COLOR_WHITE;
  }

  if (rgb_score.r * 3.5 < rgb_score.b && rgb_score.g * 2 < rgb_score.b) {
	  colorId = COLOR_BLUE;
  }

  switch(colorId) {
    case COLOR_BLACK:
 	  //色が取れているかの確認
	  ev3_lcd_draw_string("black", 30, 30);

      colorflag = 96;
      break;

	case COLOR_BLUE:
		ev3_lcd_draw_string("blue", 30, 30);
		colorflag = 7;
		break;

    case COLOR_YELLOW:
		ev3_lcd_draw_string("yerrow", 30, 30);
		colorflag = 4;
		break;

    case COLOR_GREEN:
		ev3_lcd_draw_string("green", 30, 30);
		colorflag = 3;
		break;

    case COLOR_RED:
		ev3_lcd_draw_string("red", 30, 30);
		colorflag = 5;
		break;

    case COLOR_NONE:
    case COLOR_WHITE:
      colorflag = 0;

    default:
      colorflag = 0;
      break; //COLOR_NONEとは…無色（検知できなかったりとかも）
    }

  return colorflag;
}

int LgameMove_checkcolor(LgameMove* this, int* color) {
	if (this->small_state == 0) {
		if (BasicRun_GoStraight(this->mBasicRun, -1) == 1) {
			BasicRun_reset(this->mBasicRun);
			this->small_state = 1;
		}
	}
	else if (this->small_state == 1) {
		while(Motor_setRotatedegree(this->arm_motor, 40, 20, true)!=1){}
		this->small_state = 2;
	}
	else if (this->small_state == 2) {
		//while (*color = LgameMove_findcolor(this))!=4&&!=5&&!=3&&!=7)  {}
		this->small_state = 3;
	}
	else if (this->small_state == 3) {
		while (Motor_setRotatedegree(this->arm_motor, -40, 20, true)!=1){}
		this->small_state = 0;
		return 1;
	}
	return 0;
}

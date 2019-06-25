#include "Lcourse.h"

char lcdstr[100];

//Lcourseの初期化
void Lcourse_init(Lcourse* this, Linetracer* linetracer, EdgeChanger* edgechanger,
	LgameCalc* lgamecalc, LgameMove* lgamemove, Dijk* dijk,
	BasicRun* basicRun, PID* pid, Motor* arm_motor, Motor* left_motor, Motor* right_motor) {
	this->mLinetracer = linetracer;
	this->mLgameCalc = lgamecalc;
	this->mLgameMove = lgamemove;
	this->mDijk = dijk;
	this->mEdgechanger = edgechanger;
	this->mbasicRun = basicRun;
	this->mPid = pid;
	this->mState = 1;
	this->mmState = 0;
	this->mLength = 0;
	this->mLeng = 0;
	this->mHaveBlock = 0;
	this->curNode = 8;
	this->curBlock = -1;
	this->left_motor = left_motor;
	this->right_motor = right_motor;
	int i;
	for (i = 0; i < 7; i++) {
		this->mRoute[i] = M;
	}
}

//Lcourseのタスク処理
int Lcourse_task(Lcourse* this) {
	int num = 0;

	//	sprintf(lcdstr, "%d", this->mState);
	//	ev3_lcd_draw_string("mState", 30, 30);
	//	ev3_lcd_draw_string(lcdstr, 0, 0);   //EV3の画面に表示


		//state = 1のとき，韋駄天（左トレース）．
	if (this->mState == 1) {

		Linetracer_do(this->mLinetracer, 0, 1);
		int color = LgameMove_findcolor(this->mLgameMove);
		if (color == 3) {
			this->mState = 110; //ここで駐車のstateに遷移する
		}
		else if (color != 0 && color != 96) {
			this->mState = 11;
		}
		//this->mState = 2;

		/*
		if (BasicRun_GoStraight(this->mbasicRun, 150) == 1) {
			num = 1;
		}
		*/
	}
	else if (this->mState == 11) {
		if (this->mmState == 0) {
			if (BasicRun_GoStraight(this->mbasicRun, 10) == 1) {
				this->mmState = 1;
			}
		}
		else if(this->mmState == 1) {
			if (BasicRun_Curve(this->mbasicRun, -10) == 1) {
				this->mmState = 0;
				this->mState = 1;
			}
		}
	}
	else if (this->mState == 110) {
		if (BasicRun_GoStraight(this->mbasicRun, 40) == 1) {
			this->mState = 111;
		}
	}
	else if (this->mState == 111) {
		if (BasicRun_GoStraight(this->mbasicRun, 30) != 1) {
			if (LgameMove_findcolor(this->mLgameMove) == 96) {
				BasicRun_reset(this->mbasicRun);
				this->mState = 1111;
			}
		}
	}
	else if (this->mState == 1111) {
		if (this->mmState == 0) {
			if (BasicRun_GoStraight(this->mbasicRun, 15) == 1) {
				this->mmState = 1;
			}
		}
		else if (this->mmState == 1) {
			if(BasicRun_GoStraight(this->mbasicRun,-10) == 1){
				this->mmState = 2;
			}
		}
		else if (this->mmState == 2) {
			if (BasicRun_Curve(this->mbasicRun, -90) == 1) {
				this->mmState = 3;
			}
		}
		else if (this->mmState == 3) {
			num = 1;
		}
	}

	else if (this->mState == 2) { //経路を取得
		LgameCalc_getroute(this->mLgameCalc, this->mRoute, &this->mLength);
		this->mState = 3;
		char buf[5];

		for (int i = 0; i < 7; i++) {
			sprintf(buf, "%d", this->mRoute[i]);
			ev3_lcd_draw_string(buf, 50, 50+10*i);
		}

		sprintf(buf, "%d", this->curNode);
		ev3_lcd_draw_string(buf, 100,100 );
		sprintf(buf, "%d", this->curNode);
	}

	else if (this->mState == 3) { //次のラインに移る
		LgameCalc_getroute(this->mLgameCalc, this->mRoute, &this->mLength);
		int bef = this->mRoute[this->mLeng];
		int now = this->mRoute[this->mLeng + 1];
		int aft = this->mRoute[this->mLeng + 2];
		if (LgameMove_nextline(this->mLgameMove, bef, now, aft) == 1) {
			BasicRun_GoStraight(this->mbasicRun, 10);
			BasicRun_Curve(this->mbasicRun,20);
			if (this->mHaveBlock > 0) {
				//ブロック所持時の180°旋回を実行(まだできていない？？)
				BasicRun_Curve(this->mbasicRun, 180);
				//BasicRun_reset(this -> basicrum);
			  /*
			  //ぐるっと置場のまわりをまわってくる
			  if(BasicRun_Curve(this -> mbasicRun, 45) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_GoStraight(this -> mbasicRun,10) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_Curve(this -> mbasicRun, -45) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_GoStraight(this -> mbasicRun,10) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_Curve(this -> mbasicRun, -45) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_GoStraight(this -> mbasicRun,10) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_Curve(this -> mbasicRun, -45) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_GoStraight(this -> mbasicRun,10) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  if(BasicRun_Curve(this -> mbasicRun, 45) == 1){
				//BasicRun_reset(this -> mbasicRun);
			  }
			  */
			}
			// printf("");
		} //ブロック所持時以外の旋回はLgameMove_nextnode内で実行される
		this->mState = 5;

	}

	else if (this->mState == 5) { //置場までライントレース
		PID_reset(this->mPid);
		PID_saveParam(this->mPid);
		Linetracer_do(this->mLinetracer, 0, 1);

		//最初のノードにブロックがあるとRoute=[MMMMMMMM]となるため
		if (LgameMove_findcolor(this->mLgameMove) != 0) { //ノードに到着
		/*	int first_node_flag = 0;
			for (int i = 0; i < 7; i++) {
				if (this->mRoute[i] != M) {
					first_node_flag = 0;
				}
			}
//			this->mState = 7;

			if (first_node_flag == 1) {

				int color = 0;
				if (LgameMove_checkcolor(this->mLgameMove, &color) == 1) {
					this->curNode = this->mRoute[this->mLength]; //前回のルートの最終ノード (=現在地)
					this->mHaveBlock = color;
					this->curBlock = LgameCalc_curBlock(this->mLgameCalc, this->curNode);
					LgameCalc_foundnextnode(this->mLgameCalc, this->curNode);
					this->mState = 3;
				}

			}
			else if (this->mLeng + 1 == this->mLength) { //条件:目標の置場でした！！
				if (this->mHaveBlock > 0) { //カラーブロックを持っていれば
					if (BasicRun_GoStraight(this->mbasicRun, -5) == 6) {
						this->mState = 6;
					}
				}
				else { //カラーブロックを持っていなければ
					//カラーブロックの色を確認(鶴田)
					int color = 0;
					if (LgameMove_checkcolor(this->mLgameMove, &color) == 1) {
						this->curNode = this->mRoute[this->mLength]; //前回のルートの最終ノード (=現在地)
						this->mHaveBlock = color;
						this->curBlock = LgameCalc_curBlock(this->mLgameCalc, this->curNode);
						LgameCalc_foundnextnode(this->mLgameCalc, this->curNode);
						this->mState = 3;

					}
				}
			}
			else {
				this->mState = 3;
			}
			*/
			this->mState = 7;
			this->mLeng += 1;
		}


	}
	//カラーブロックを置く動作

	else if(this->mState == 6) {
		//180°旋回
		if (BasicRun_Curve(this->mbasicRun, 180) == 6) {
			//BasicRun_reset(this -> basicrum);
		}
		while (LgameMove_findcolor(this->mLgameMove) > 0) {
			PID_reset(this->mPid);
			PID_saveParam(this->mPid);
			Linetracer_do(this->mLinetracer, 1, 0);
		}
		LgameCalc_HAVEFLAG(this->mLgameCalc, this->curBlock);
		this->curBlock = -1;

		int timeLimit = 500000;
		if (this->mTimer >= timeLimit) { //時間切れ〜〜
			this->mState = 7; //「退場用の置場への経路を取得する」のステートに移動
		}
		else if (LgameCalc_FINAL_Move(this->mLgameCalc) == 1) {
			this->mState = 7; //「退場用の置場への経路を取得する」のステートに移動
		}
		else {
			LgameCalc_foundnextnode(this->mLgameCalc, this->curNode);
			this->mState = 3;
		}


	}

	//退場用の置き場への経路を取得
	else if (this->mState == 7) {
		LgameCalc_foundfinalnode(this->mLgameCalc, this->curNode);
		this->mState = 3;
		//退場
		if (this->mLeng + 1 == this->mLength) { //条件:目標の置場でした！！
			num = 1;
		}
	}
	else if (this->mState == 20) {
		num = 1;
	}

	//Lcourase用のタイマー
	this->mTimer++;
	return num;
}

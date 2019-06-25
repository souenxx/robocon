#include "LgameCalc.h"

//Lコースのゲームパートの計算の機能の初期化
void LgameCalc_init(LgameCalc* this, Dijk* dijk) {
	this->mDijk = dijk;

	int i;
	for (i = 0; i < 7; i++) {
		this->mRoute[i] = M;
	}
	for (i = 0; i < 4; i++) { //変更　たぶんi<4
		this->mCarry[i] = F;
	}
	this->mLength = 0;

}

int LgameCalc_getcode(LgameCalc* this,int init_code) {
	//Bluetoothを用いた初期コードの受信();
	//今は決め打ち
	this->code = init_code; //→計算後は 1,6,8,14,8 となるはず
	calc_code(this); //コードを渡すこと
	//calc_ChangeBlack(this); //calc_codeをした後にしか使えない //たぶんいらない(鶴田)
	LgameCalc_foundnextnode(this, 8);//この段階で16から次のノードに行く経路を探してる．
	return 1;
}

//初期化で使う関数
//初期コードの計算
void calc_code(LgameCalc* this) {
	int i, Init_Block[4], power;
	int Init_code = this->code;

	//初期コード->初期位置番号の計算
	for (i = 3; i >= 0; i--) {                 //カラーブロック４回分
		Init_Block[3 - i] = Init_code / (int)pow(16.0, (double)i + 1);
		Init_code -= (Init_Block[3 - i] * (int)pow(16.0, (double)i + 1));
	}
	power = Init_code; //黒(パワー)ブロックのパターン

	for (i = 0; i < 4; i++) {
		this->mBlock[i] = Init_Block[i];
	}
	this->mPower = power;
}
/* たぶんいらない　（鶴田）
//黒ブロックと入れ替えるブロックを探す．
void calc_ChangeBlack(LgameCalc *this) {
	//ブロックが置いてあるノードをマップに記録する．
	//Dijk_routeの&Routeと&new_lengthは不要なのでHoge&hogeで捨てる．
  //  int Node_Flag[15] = { F, F, F, F, F, F, F, F, F, F, F, F, F, F, F};
	Dijk_map(this->mDijk, this->mBlock);

	//各ブロックの目標ノードまでのコストを計算する．

	//各ブロックのコストを保管するための変数
	int ColorB_Cost[4];
	//値を受け取る必要のない時に使うもの
	int Hoge[20], hoge;

	//赤
	Dijk_algo(this->mDijk, this->mBlock[0]);
	Dijk_route(this->mDijk, this->mBlock[0], 0, Hoge, &ColorB_Cost[0], &hoge);
	//黄
	Dijk_algo(this->mDijk, this->mBlock[1]);
	Dijk_route(this->mDijk, this->mBlock[1], 4, Hoge, &ColorB_Cost[1], &hoge);
	//青
	Dijk_algo(this->mDijk, this->mBlock[2]);
	Dijk_route(this->mDijk, this->mBlock[2], 1, Hoge, &ColorB_Cost[2], &hoge);
	//緑
	Dijk_algo(this->mDijk, this->mBlock[3]);
	Dijk_route(this->mDijk, this->mBlock[3], 9, Hoge, &ColorB_Cost[3], &hoge);
	// ColorB_Cost[3] = 1000;
	//黒ー各色
	Dijk_algo(this->mDijk, this->mBlock[0]);
	Dijk_route(this->mDijk, this->mBlock[0], 0, Hoge, &BlackB_Cost[0], &hoge);
	Dijk_route(this->mDijk, this->mBlock[0], 4, Hoge, &BlackB_Cost[1], &hoge);
	Dijk_route(this->mDijk, this->mBlock[0], 1, Hoge, &BlackB_Cost[2], &hoge);
	Dijk_route(this->mDijk, this->mBlock[0], 9, Hoge, &BlackB_Cost[3], &hoge);


	//カラーブロックと黒ブロックのコスト差を保管するための配列
	int Dis_Cost[4];
	int i, max_cost = -99;
	//黒と入れ替えるブロックの色（0：赤，1：黄，2：青，3:緑）
	int Change_black = 0;

	//上で計算したコスト差が一番大きなカラーブロックを探す．
	for (i = 0; i < 4; i++) {
		Dis_Cost[i] = ColorB_Cost[i] - BlackB_Cost[i];  //カラーブロックと黒ブロックのコスト差を計算する．
		if (max_cost < Dis_Cost[i]) {     //max_Costよりも大きいなら
			max_cost = Dis_Cost[i];     //max_Costを更新して，
			Change_black = i;           //入れ替え番号を更新する．
		}
	}
	//入れ替えを行う．
	int tmp;
	if (Change_black == 0) {
		tmp = this->mBlock[0];
		this->mBlock[0] = this->mBlock[1];
		this->mBlock[1] = tmp;
	}
	else if (Change_black == 1) {
		tmp = this->mBlock[0];
		this->mBlock[0] = this->mBlock[2];
		this->mBlock[2] = tmp;
	}
	else if (Change_black == 2) {
		tmp = this->mBlock[0];
		this->mBlock[0] = this->mBlock[3];
		this->mBlock[3] = tmp;
	}
	else if (Change_black == 3) {
		tmp = this->mBlock[0];
		this->mBlock[0] = this->mBlock[4];
		this->mBlock[4] = tmp;
	}
}
*/

//ゲームをするときに使う関数
//次の目標ノードへの経路を決める．．
//引数：
//返値：（ポインタで返す）コスト(cost) と 経路導出に必要な情報(upda)
void LgameCalc_foundnextnode(LgameCalc* this, int start) {
	int cost, leng;//Route足しました
	int Route[7] = { 0,0,0,0,0,0,0 };
	int i, j, min_cost = M;// length = 0;
	int goal = 100;
						   // int block = 1;
	Dijk_map(this->mDijk, this->mBlock);
	Dijk_algo(this->mDijk, start);

	//ブロックまでの経路
	if (goal == 100) {
		for (i = 0; i < 4; i++) {
			if (this->mCarry[i] != T) {
				Dijk_route(this->mDijk, start, this->mBlock[i], Route, &cost, &leng);
				if (min_cost > cost) {
					for (j = 0; j < 7; j++) {
						this->mRoute[j] = Route[j];
					}
					min_cost = cost;
					this->mLength = leng;
				}
			}
		}
	}

	//置き場までの経路
	else {
		for (i = 0; i < 4; i++) {
			if (this->mCarry[i] != T) {
				Dijk_route(this->mDijk, start, this->mBlock[i], Route, &cost, &leng);
				if (min_cost > cost) {
					for (j = 0; j < 7; j++) {
						this->mRoute[j] = Route[j];
					}
					min_cost = cost;
					this->mLength = leng;
				}
			}
		}
	}

}

void LgameCalc_foundfinalnode(LgameCalc* this, int start) {
	int cost, leng;//Route足しました
	int Route[7] = { 0,0,0,0,0,0,0 };
	// int block = 1;
	Dijk_map(this->mDijk, this->mBlock);
	Dijk_algo(this->mDijk, start);
	Dijk_route(this->mDijk, start, 11, Route, &cost, &leng);
	if (cost == M) {
		Dijk_route(this->mDijk, start, 15, Route, &cost, &leng);
	}
	this->mLength = leng;
}

void LgameCalc_HAVEFLAG(LgameCalc *this, int block) {
	this->mCarry[block] = T;
}

//ルートを取得する．
int LgameCalc_getroute(LgameCalc* this, int *Route, int *length) {
	*length = this->mLength;
	int i;
	for (i = 0; i < 7; i++) {
		Route[i] = this->mRoute[i];
	}
	return 1;
}


//鶴田　ブロックの位置(mBlock)を更新する処理を書く
void LgameCalc_updBlock(LgameCalc* this, int curBlock, int curNode) {
	this->mBlock[curBlock] = curNode;
}

//鶴田　現在地のノードからmBlockのうち，どれを運ぶのかを返す．
int LgameCalc_curBlock(LgameCalc* this, int node) {
	int i;
	for (i = 0; i < 4; i++) {
		if (this->mBlock[i] == node)
			return i;
	}
	return -1; //仮　エラー的な
}

//工藤 全てのブロックを移動し終えたらフラグ
int LgameCalc_FINAL_Move(LgameCalc* this) {
	if (this->mCarry[0] == T && this->mCarry[1] == T && this->mCarry[2] == T && this->mCarry[3] == T) {
		return 1;
	}
	return 0;
}

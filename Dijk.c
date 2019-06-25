#include "Dijk.h"

//ダイクストラ法の計算部分の機能の初期化
void Dijk_init(Dijk* this){
  int i;
  for(i=0;i<15;i++){
    this->mCost[i] = M;
    this->mUpda[i] = M;
    this->mBlock_Flag[i] = F;
  }
}

//移動コスト表の更新
void Dijk_map(Dijk *this, int *block) {
	int i;
	for (i = 0; i < 16; i++) {
		this->mBlock_Flag[i] = F;
	}
	for (i = 0; i < 4; i++) {
	this->mBlock_Flag[block[i]] = T;
	}
}

//出発ノードと目標ノードを与えてその間の経路を出力する．
void Dijk_algo(Dijk *this, int start){

}

//更新タイミングとコスト一覧を用いて，start->goalの経路と経路の長さ，goalまでのコストを求める．
void Dijk_route(Dijk *this, int start, int goal, int *route, int *b_cost, int *r_length){
  //経路逆順
  int re_route[7] = {0,0,0,0,0,0,0};
  //経路の長さ
  int length = M;
  int i = 0;
  int tmp = goal;
  for(i=0;i<7;i++){
    route[i] = M;
  }
  i=0;
  //目標ノードから出発ノードまで遡った配列を作成する．
  re_route[0] = goal; //目標ノードを最初に入れる．
  if(this->mUpda[goal] != M){
    do{
      //1つ前のノードはダイクストラ法で計算しているときに，最後に更新されたときの確定ノードになる．
      re_route[i+1] = this->mUpda[tmp];
      tmp = re_route[i+1];
      i++;
      //Re_route[i]が出発ノードになるまで続ける．
    }while(re_route[i] != start);

    //上で作った配列を入れ替える
    length = i;
    for(i=0;i>length;i++){
      route[i] = re_route[length-i];
    }

    //ルートとパス数を渡しておしまい
    *b_cost = this->mCost[goal];
    *r_length = length;
  }else{
    *b_cost = M;
    *r_length = 0;
  }
}

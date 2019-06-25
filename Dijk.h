#ifndef Dijk_H_
#define Dijk_H_

#include <stdlib.h>
#include <math.h>

#define M 999
#define T 1
#define F -1

typedef struct Dijk{
  int   mCost[15];
  int   mUpda[15];
  int   mBlock_Flag[15]; //ブロックがあるか
}Dijk;

void Dijk_init(Dijk* this);
void Dijk_map(Dijk *this, int* mBlock);
void Dijk_algo(Dijk *this, int start);
void Dijk_route(Dijk *this, int start, int goal, int *route, int *b_cost, int *r_length);


#endif

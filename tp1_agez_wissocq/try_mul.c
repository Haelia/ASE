#include <stdlib.h>
#include <stdio.h>
#include "try.h"

static struct ctx_s pctx;


static int mul(){
  int i;
  switch(scanf("%d",&i)){
  case EOF : 
    return 1;
  case 0 :
    return mul();
  case 1:
    if(i)
      return i *mul();
    else
      throw(&pctx,0);

  }
}


int main(){
  static int product;
  printf("A list of int, please\n");
  product=try(&pctx,mul,0);
  printf("product = %d\n",product);
}

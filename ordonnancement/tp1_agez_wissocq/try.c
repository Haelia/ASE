#include <stdio.h>
#include <stdlib.h>
#include "try.h"

typedef int (funct_t)(int);



int try(struct ctx_s *pctx, funct_t *f, int arg){

asm("movl %%esp, %0" "\n\t" "movl %%ebp, %0"
    :"=r"(pctx->esp), "=r"(pctx->ebp)
    :
    :);

return f(arg);
}


int throw(struct ctx_s *pctx, int r){
    static int res;
    res=r;
    asm("movl %1, %%ebp"  "\n\t" "movl %1, %%esp"
       :
       :"r" (pctx->ebp), "r"(pctx->esp)
       :);
   
    return res;
}

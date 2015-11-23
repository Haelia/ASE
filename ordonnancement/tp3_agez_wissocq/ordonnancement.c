#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hw.h"
#define MAGIC 42
#include "ordonnancement.h"



/*Variable globale pointant sur le contexte activé en ce moment*/
static ctxt *ctx_act = NULL;

int create_ctx(int stack_size, func_t f, void *args){
  irq_disable();
  ctxt * ctx = malloc(sizeof(ctxt));
   if (ctx_act == NULL) {
    ctx_act = ctx;
    ctx->next = ctx;
    ctx->prec = ctx;
  } else {
    ctx->next = ctx_act->next;
    ctx->prec = ctx_act;
    ctx_act->next = ctx;
    ctx->next->prec=ctx;
  }  
  
  ctx->ctx_status=READY;
  ctx->ctx_f=f;
  ctx->ctx_args=args;
  ctx->ctx_stack=(unsigned char*) malloc(stack_size);
  ctx->esp=&ctx->ctx_stack[stack_size-sizeof(int)];
  ctx->ebp=&ctx->ctx_stack[stack_size-sizeof(int)];
  ctx->magic=MAGIC;
 
  return 1;
}

void exec_ctx(ctxt *ctx){
  ctx_act->ctx_status=ACTIVABLE;
  ctx_act->ctx_f(ctx_act->ctx_args);
  irq_disable();
ctx_act->ctx_status=TERMINATED;
free(ctx_act->ctx_stack);
free(ctx_act);
 if(ctx_act->next != ctx_act){
    ctxt *next_ctx = ctx_act->next;
    ctxt *last_ctx = ctx_act->prec;
    ctx_act = NULL;
    last_ctx->next = next_ctx;
    next_ctx->prec = last_ctx;
    switch_to(next_ctx);
  }
  else {
    printf("\nNo more task to execute\n");
    exit(EXIT_SUCCESS);
  }
}

void switch_to(ctxt *ctx){
irq_disable();
  assert(ctx != NULL);
  assert(ctx->magic==MAGIC);
  assert(ctx->ctx_status == READY || ctx->ctx_status == ACTIVABLE);
  
  /*Sauvegarder le contexte courant ctx_act=contexte courant*/
  if (ctx_act != NULL) {
    asm("movl %%esp, %0"
	:"=r" (ctx_act->esp)
	:
	:);
    asm("movl %%ebp, %0"
	:"=r" (ctx_act->ebp)
	:
	:);
  }
  ctx_act = ctx;
  asm("movl %0, %%ebp"
      :
      :"r"(ctx_act->ebp)
      :);
  asm("movl %0, %%esp"
      :
      :"r" (ctx_act->esp)
      :);
irq_enable();

  /*Relancer le contexte en paramètre : ctx */
  if(ctx_act->ctx_status== READY){
    exec_ctx(ctx_act);
  }
}

void yield(){
    switch_to(ctx_act->next);
}

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *arg);
void f_pong(void *arg);

void f_ping(void *args){
  while(1){
    printf("A\n");
    printf("B\n");
    printf("C\n");
  }
}

void f_pong(void *args){
  while(1){
    printf("1\n");
    printf("2\n");
  }
}

void start_sched(){
  /*associé handler avec timer */
  start_hw();
  setup_irq(TIMER_IRQ, yield);
  yield();
}

int main(int argc, char *argv[]){
  create_ctx(16384, f_pong,NULL);
  create_ctx(16384, f_ping, NULL);
  start_sched();
  exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void (func_t)(void *);

/*Enumération des différents états d'un contexte*/
enum ctx_state{READY, ACTIVABLE, TERMINATED};

/*Structure d'un contexte*/
struct ctx_s{
  enum ctx_state ctx_status;
  func_t *ctx_f;
  void *ctx_args;
  void *esp;
  void *ebp;
  unsigned char *ctx_stack;
  unsigned int magic;
};

/*Variable globale pointant sur le contexte activé en ce moment*/
static struct ctx_s *ctx_act = NULL;

int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args){
  ctx->ctx_status=READY;
  ctx->ctx_f=f;
  ctx->ctx_args=args;
  ctx->ctx_stack=(unsigned char*) malloc(stack_size);
  ctx->esp=&ctx->ctx_stack[stack_size-sizeof(int)];
  ctx->ebp=&ctx->ctx_stack[stack_size-sizeof(int)];
ctx->magic=666;
  return 1;
}

void exec_ctx(struct ctx_s *ctx){
  ctx_act->ctx_status=ACTIVABLE;
  ctx_act->ctx_f(ctx_act->ctx_args);
  printf("test");
  while(1);
}

void switch_to_ctx(struct ctx_s *ctx){
  assert(ctx != NULL);
  assert(ctx->magic==666);
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
 /*Relancer le contexte en paramètre : ctx */
  if(ctx_act->ctx_status== READY){
    exec_ctx(ctx_act);
  }
}


struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *arg);
void f_pong(void *arg);

void f_ping(void *args){
  while(1){
    printf("A");
    switch_to_ctx(&ctx_pong);
    printf("B");
    switch_to_ctx(&ctx_pong);
    printf("C");
    switch_to_ctx(&ctx_pong);
  }
}

void f_pong(void *args){
  while(1){
    printf("1");
    switch_to_ctx(&ctx_ping);
    printf("2");
    switch_to_ctx(&ctx_ping);
  }
}

int main(int argc, char *argv[]){
  init_ctx(&ctx_ping, 16384, f_ping, NULL);
  init_ctx(&ctx_pong, 16384, f_pong,NULL);
  ctx_act=&ctx_ping;
  switch_to_ctx(&ctx_ping);
  exit(EXIT_SUCCESS);
}

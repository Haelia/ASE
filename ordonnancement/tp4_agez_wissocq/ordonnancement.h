#ifndef ORDH
#define ORDH
/*Enumération des différents états d'un contexte*/
enum ctx_state{READY, ACTIVABLE, TERMINATED,BLOCKED};
typedef void (func_t)(void *);

/*Structure d'un contexte*/
struct ctx_s{
  enum ctx_state ctx_status;
  func_t *ctx_f;
  void *ctx_args;
  void *esp;
  void *ebp;
  unsigned char *ctx_stack;
  unsigned int magic;
  struct ctx_s *next, *prec;
  struct sem_s *sem;
};


typedef struct ctx_s ctxt;

/*Variable globale pointant sur le contexte activé en ce moment*/
ctxt *ctx_act;


int create_ctx(int stack_size, func_t f, void *args);
void exec_ctx(ctxt *ctx);
void switch_to(ctxt *ctx);
void yield();
void f_ping(void *args);
void f_pong(void *args);
void start_sched();

#endif

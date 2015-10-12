
/*Enumération des différents états d'un contexte*/
enum ctx_state{READY, ACTIVABLE, TERMINATED};
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
};


typedef struct ctx_s ctxt;


int create_ctx(int stack_size, func_t f, void *args);
void exec_ctx(ctxt *ctx);
void switch_to(ctxt *ctx);
void yield();
void f_ping(void *args);
void f_pong(void *args);
void start_sched();

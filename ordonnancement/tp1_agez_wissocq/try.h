struct ctx_s{
    void *esp;
    void *ebp;
};

typedef int (funct_t)(int);
int try(struct ctx_s *pctx, funct_t *f, int arg);
int throw(struct ctx_s *pctx, int r);



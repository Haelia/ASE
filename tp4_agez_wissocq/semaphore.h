#include <stdlib.h>
#include <stdio.h>
#ifndef ORDH
#define ORDH 1
#include "ordonnancement.h"
#endif

struct sem_s {
    int tokens;
    ctxt *context;
};

typedef struct sem_s semt;

void sem_init(semt *sem, unsigned int val);
void sem_down(semt *sem);
void sem_up(semt *sem);


#include "semaphore.h"
#include "hw.h"

void sem_init(semt *sem, unsigned int val){
    sem->tokens = val;
    sem->context = NULL;
}

void sem_down(semt *sem) {
    irq_disable();
    if (sem->tokens-- <= 0) {
        ctx_act->ctx_status = BLOCKED;
        ctx_act->next = sem->context->next;
        ctx_act->prec = sem->context;
        sem->context->next->prec = ctx_act;
        sem->context->next = ctx_act;
        sem->context = ctx_act;
        yield();
    }
    else{
    irq_enable();
    }
}

void sem_up(semt *sem) {
    irq_disable();
    sem->tokens++;
    if (ctx_act->ctx_status = BLOCKED) {
        ctx_act->ctx_status = ACTIVABLE;
        ctx_act->next->prec = ctx_act->prec;
        ctx_act->prec->next = ctx_act->next;
        ctx_act->next = NULL;
        ctx_act->prec = NULL;
    }
    irq_enable();
}

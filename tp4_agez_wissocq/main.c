#ifndef SEMH
#define SEMH 1
#include "semaphore.h"
#endif
#include "hw.h"
#define N 100

struct objet_s {
    char* nom;
    int n;
};


typedef struct objet_s objett;

struct tampon_s {
    objett objets[N];
    int in;
    int out;
};

typedef struct tampon_s tampont;

tampont *tampon;
semt mutex;
semt vide ;
semt plein;



void init_tampon(tampont *tampon, int s) {
    tampon->in = 0;
    tampon->out = 0;
}


void produire_objet(objett *objet) {
    objet = (objett *) malloc(sizeof(objett));
    objet->nom = "cacahuète";
    objet->n = 42;
}

void utiliser_objet(objett *objet) {
    objet->nom = "cacahuète mangée";
    free(objet);
}


void mettre_objet(objett *objet) {
    tampon->objets[tampon->in++] = *objet;
}

void retirer_objet(objett *objet) {
    *objet = tampon->objets[tampon->out++];
}

void producteur() {
    objett objet;

    while(1) {
        produire_objet(&objet);
        sem_down(&vide);
        sem_down(&mutex);
        mettre_objet(&objet);
        sem_up(&mutex);
        sem_up(&plein);
    }
}


void consommateur() {
    objett objet;

    while(1) {
        sem_down(&plein);
        sem_down(&mutex);
        retirer_objet(&objet);
        sem_up(&mutex);
        sem_up(&vide);
        utiliser_objet(&objet);
    }
}

int main (int argc, char *argv[]) {
  tampon = (tampont *) malloc(sizeof(tampont));
  init_tampon(tampon, N);
  sem_init(&mutex, 1);
  sem_init(&vide, N);
  sem_init(&plein, 0);
  create_ctx(16384,producteur,NULL);
  create_ctx(16384,consommateur,NULL);
  start_sched();
  return 0;
}




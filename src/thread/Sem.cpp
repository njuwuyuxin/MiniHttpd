#include "thread/Sem.h"

Sem::Sem(){
    sem_init(&_sem,0,0);
}

Sem::Sem(int initial_val){
    sem_init(&_sem,0,initial_val);
}

void Sem::wait(){
    sem_wait(&_sem);
}

void Sem::post()
{
    sem_post(&_sem);
}

Sem::~Sem(){
     sem_destroy(&_sem);
}
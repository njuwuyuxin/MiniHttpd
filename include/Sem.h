#ifndef SEM_H
#define SEM_H

#include <semaphore.h>

class Sem{
public:
    Sem();
    Sem(int initial_val);
    void wait();
    void post();
    ~Sem();

private:
    sem_t _sem;
};


#endif
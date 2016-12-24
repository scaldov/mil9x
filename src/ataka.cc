#include "ataka.hh"

xSemaphoreHandle ataka::sem_isr = 0;
xSemaphoreHandle ataka::sem_res = 0;
ataka *ataka::intance;

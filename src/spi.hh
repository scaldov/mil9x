#pragma once

#include "hw.hh"
#include "os.hh"

namespace lk {

class spi : public peripheral{
public:
    int index;
    static xSemaphoreHandle sem_isr;
    static xSemaphoreHandle sem_res;
    spi(int index, int master = 1) : index(index){
        SSP(index).cr0 = SSP::CR0::SPH;
    }
    int read(){

    }
    int write(){

    }
};
}

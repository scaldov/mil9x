#ifndef __LK_RCC_HH__
#define __LK_RCC_HH__

#include "hw.hh"

namespace lk {

typedef struct {
    uint32_t pll_src:2;
    uint32_t pll_mul:4;
    uint32_t prescaler:4;
    uint32_t clk_sel:12;
} cpuclkconf;

class device {
public:
    virtual int enable(){return 0;}
    virtual int disable(){return 0;}
    virtual int lpm(int state){return 0;}
    virtual int sleep(int state){return 0;}
    /*
    int enable(){return 0;}
    int disable(){return 0;}
    int lpm(int state){return 0;}
    int sleep(int state){return 0;}
*/
};

class peripheral : public device{
public:
    peripheral(){}
    //	uint32_t periph;
};

class cpu{
public:
    cpu() {}
    static void cpuclk() {
        RST().hsControl = RST::HsControl::HseOn;
        RST().pllControl = RST::PllControl::PllCpuOn | RST::PllControl::PllUsbOn | RST::PllControl::PllCpuRld | RST::PllControl::PllUsbRld |
                RST::PllControl::PllCpuMul(16 - 1) | RST::PllControl::PllUsbMul(16 - 1);
        RST().cpuClock = RST::CpuClock::CpuC2Sel::PllCpu | RST::CpuClock::CpuC3Sel::CpuC2 | RST::CpuClock::HclkSel::CpuC3;
        SystemCoreClock = HSE_VALUE * 16;
    }
};

}

#endif


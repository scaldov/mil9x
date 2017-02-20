#include "os.hh"

volatile uint32_t g_t[8];

#include "pin.hh"

namespace lk {

uint32_t systick::systick_ctrl;

void systick::wait(uint32_t tick)
{
    SysTick->LOAD = tick;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}

}

int gdb_printf(char *s, ...) {
    return 0;
}

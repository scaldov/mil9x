#include "os.hh"

volatile uint32_t g_t[8];

#include "pin.hh"

namespace lk {

uint32_t systick::systick_ctrl;

}

int gdb_printf(char *s, ...) {
    return 0;
}

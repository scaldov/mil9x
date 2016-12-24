#ifndef __LK_PIN_HH__
#define __LK_PIN_HH__

#include "hw.hh"

#include "gpio.hh"

namespace lk {

class pin{
public:
	uint32_t pin_num;
	uint32_t pin_mask;
	gpio *port;
	pin(gpio *port, uint32_t pin_num, int output = 1) {
		this->port = port;
		this->pin_num = pin_num;
		pin_mask = 1 << pin_num;
		port->ioconfig(pin_mask, PORT_FUNC_PORT,
				output ? PORT_OE_OUT: PORT_OE_IN,
				PORT_PD_DRIVER, PORT_PULL_UP_OFF, PORT_PULL_DOWN_OFF, PORT_SPEED_MAXFAST, PORT_PD_SHM_OFF, PORT_GFEN_OFF);
	}
	void toggle() {
		port->toggle(pin_mask);
	}
	void set() {
		port->set(pin_mask);
	}
	void reset() {
		port->reset(pin_mask);
	}
};
}

#endif

#include "gpio.hh"


namespace lk {

uint32_t gpio::init = 0;

MDR_PORT_TypeDef * const gpio::gpio_addr[] = {
		MDR_PORTA, MDR_PORTB,
        MDR_PORTC, MDR_PORTD,
		MDR_PORTE, MDR_PORTF
};

uint32_t const gpio::gpio_periph[] = {
		RST_CLK_PCLK_PORTA, RST_CLK_PCLK_PORTB,
		RST_CLK_PCLK_PORTC, RST_CLK_PCLK_PORTD,
		RST_CLK_PCLK_PORTE, RST_CLK_PCLK_PORTF
};

gpio *_ioport[] = {
		new gpio(gpio::PORTA), new gpio(gpio::PORTB),
		new gpio(gpio::PORTC), new gpio(gpio::PORTD),
		new gpio(gpio::PORTE), new gpio(gpio::PORTF)
};

}

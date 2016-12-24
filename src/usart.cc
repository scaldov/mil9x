#include "usart.hh"


namespace lk {

typedef struct {
	uint16_t port;
	uint16_t pin;
	uint16_t func;
	uint16_t driver;
	uint16_t speed;
	uint16_t pullup;
	uint16_t pulldown;
} ioconf;


extern const ioconf ioconf_uart_tx_;
const ioconf ioconf_spi1_miso_A6 = {gpio::PORTA, 6, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};

}

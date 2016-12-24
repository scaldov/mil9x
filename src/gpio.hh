#ifndef __LK_GPIO_HH__
#define __LK_GPIO_HH__

#include "hw.hh"

#include "rcc.hh"

namespace lk {

class gpio;
extern gpio *_ioport[];

typedef struct {
    uint16_t port:6;
    uint16_t pin:4;
    uint16_t func:2;
    uint16_t driver:2;
    uint16_t speed:2;
    uint16_t pullup:2;
    uint16_t pulldown:2;
} ioconf;

class gpio : public peripheral {
//class gpio {
	MDR_PORT_TypeDef *addr;
	static uint32_t init;
//	GPIO_TypeDef* port;
	static MDR_PORT_TypeDef * const gpio_addr[];
	static uint32_t const gpio_periph[];
public:
	int gpio_port;
	enum port {
		PORTA = 0,
		PORTB,
		PORTC,
		PORTD,
		PORTE,
		PORTF,
		PORTG,
		PORTH,
		PORTI,
		PORTJ,
		PORTK
	};
	gpio(port gpio) {
		uint32_t mask;
		gpio_port = gpio;
		addr = gpio_addr[gpio];
		mask = (1 << gpio);
		if(!(init & mask)) {
			enable();
			PORT_DeInit(addr);
			init |= mask;
			MDR_PORTD->OE = 0xFFFF;
		}
	}
	int enable() {
		RST_CLK_PCLKcmd(gpio_periph[gpio_port], ENABLE);
		return 0;
	}
	int disable() {
		RST_CLK_PCLKcmd(gpio_periph[gpio_port], DISABLE);
		return 0;
	}
    static void ioconfig(ioconf const *ioconfs) {
        lk::_ioport[ioconfs->port]->ioconfig(ioconfs->pin, ioconfs->func, 0,
                ioconfs->driver, ioconfs->pullup, ioconfs->pulldown,
				ioconfs->speed, PORT_PD_SHM_OFF, PORT_GFEN_OFF);
	}
	void ioconfig(uint32_t pin, char function = PORT_FUNC_PORT,
			char dir = PORT_OE_OUT, char driver = PORT_PD_DRIVER,
			char pullup = PORT_PULL_UP_OFF, char pulldown = PORT_PULL_DOWN_OFF,
			char speed = PORT_SPEED_MAXFAST, char trigger = PORT_PD_SHM_OFF,
			char filter = PORT_GFEN_OFF) {
		PORT_InitTypeDef conf;
		conf.PORT_Pin = pin;
		conf.PORT_OE = (PORT_OE_TypeDef)dir;
		conf.PORT_MODE = PORT_MODE_DIGITAL;
		conf.PORT_FUNC = (PORT_FUNC_TypeDef)function;
		conf.PORT_PD = (PORT_PD_TypeDef)driver;
		conf.PORT_SPEED = (PORT_SPEED_TypeDef)speed;
		conf.PORT_PULL_UP = (PORT_PULL_UP_TypeDef)pullup;
		conf.PORT_PULL_DOWN = (PORT_PULL_DOWN_TypeDef)pulldown;
		conf.PORT_PD_SHM = (PORT_PD_SHM_TypeDef)trigger;
		conf.PORT_GFEN = (PORT_GFEN_TypeDef)filter;
		PORT_Init(addr, &conf);
	}
	uint32_t in(int bits) {
		return PORT_ReadInputDataBit(addr, bits);
	}
	uint32_t in() {
		return PORT_ReadInputData(addr);
	}
	void out(int bits, int val) {
		PORT_WriteBit(addr, bits, val ? Bit_SET : Bit_RESET);
	}
	void out(int val) {
		PORT_Write(addr, val);
	}
	uint32_t output(int bits) {
		return in(bits);
	}
	uint32_t output() {
		return in();
	}
	void set(int bits) {
		PORT_SetBits(addr, bits);
	}
	void reset(int bits) {
		PORT_ResetBits(addr, bits);
	}
	void toggle(int bits) {
		addr->RXTX ^= bits;
	}
};

#define gpio_a _ioport[lk::gpio::PORTA]
#define gpio_b _ioport[lk::gpio::PORTB]
#define gpio_c _ioport[lk::gpio::PORTC]
#define gpio_d _ioport[lk::gpio::PORTD]
#define gpio_e _ioport[lk::gpio::PORTE]
#define gpio_f _ioport[lk::gpio::PORTF]
#define gpio_g _ioport[lk::gpio::PORTG]
#define gpio_h _ioport[lk::gpio::PORTH]
#define gpio_i _ioport[lk::gpio::PORTI]
#define gpio_j _ioport[lk::gpio::PORTJ]
#define gpio_k _ioport[lk::gpio::PORTK]

}

//#define gpio_d lk::ioport[lk::gpio::PORTD]

#endif

#ifndef __LK_USART_HH__
#define __LK_USART_HH__


#include "gpio.hh"
#include "hw.hh"

namespace lk {

class usart {
	MDR_UART_TypeDef *devptr;
	int chn;
public:
	usart(int chn)
	{
		this->chn = chn;
		devptr = chn == 1 ? MDR_UART2 : MDR_UART1;
	}
	int enable() {
		uint32_t clkmask = chn == 1 ? RST_CLK_PCLK_UART2 : RST_CLK_PCLK_UART1;
		RST_CLK_PCLKcmd(clkmask, ENABLE);
        printf_("uart%d enabled\n", chn);
		return 0;
	}
	int disable() {
		uint32_t clkmask = chn == 1 ? RST_CLK_PCLK_UART2 : RST_CLK_PCLK_UART1;
		RST_CLK_PCLKcmd(clkmask, DISABLE);
	}
//	void config(uint32_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOSpeed_TypeDef speed, GPIOPuPd_TypeDef pupd);
	int read(char *bfr, int len) {
		return 0;
	}

	int send(char *bfr, int len) {
		return 0;
	}
};

}

#endif

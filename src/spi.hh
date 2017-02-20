#ifndef __LK_SPI_HH__
#define __LK_SPI_HH__

#include "hw.hh"
#include "os.hh"

namespace lk {

extern const ioconf ioconf_spi1_miso_A6;
extern const ioconf ioconf_spi1_miso_B4;
extern const ioconf ioconf_spi1_mosi_A7;
extern const ioconf ioconf_spi1_mosi_B5;
extern const ioconf ioconf_spi1_sck_A5;
extern const ioconf ioconf_spi1_sck_B3;
extern const ioconf ioconf_spi1_NSS_A4;
extern const ioconf ioconf_spi1_NSS_A15;

extern const ioconf ioconf_spi2_miso_B4;
extern const ioconf ioconf_spi2_miso_C2;
extern const ioconf ioconf_spi2_mosi_B15;
extern const ioconf ioconf_spi2_mosi_C3;
extern const ioconf ioconf_spi2_sck_B10;
extern const ioconf ioconf_spi2_sck_B13;
extern const ioconf ioconf_spi2_NSS_B12;
extern const ioconf ioconf_spi2_NSS_B9;

extern const ioconf ioconf_spi3_miso_B4;
extern const ioconf ioconf_spi3_miso_C11;
extern const ioconf ioconf_spi3_mosi_B5;
extern const ioconf ioconf_spi3_mosi_C12;
extern const ioconf ioconf_spi3_sck_C10;
extern const ioconf ioconf_spi3_sck_B3;
extern const ioconf ioconf_spi3_NSS_A4;
extern const ioconf ioconf_spi3_NSS_A15;

class spi : public peripheral{
public:
	spi() {
		lk::gpio::ioconfig(&ioconf_spi1_miso_A6);
	}

	int enable() {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		return 0;
	}

	int disable() {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
		return 0;
	}

	int master() {

	}

};
}

#endif

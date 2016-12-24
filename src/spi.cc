#include "spi.hh"

namespace lk {
const ioconf ioconf_spi1_miso_A6 = {gpio::PORTA, 6, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_miso_B4 = {gpio::PORTB, 4, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_mosi_A7 = {gpio::PORTA, 7, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_mosi_B5 = {gpio::PORTB, 5, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_sck_A5 = {gpio::PORTA, 5, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_sck_B3 = {gpio::PORTB, 3, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_NSS_A4 = {gpio::PORTA, 4, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi1_NSS_A15 = {gpio::PORTA, 15, GPIO_AF_SPI1, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};

const ioconf ioconf_spi2_miso_B4 = {gpio::PORTB, 14, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_miso_C2 = {gpio::PORTC, 2, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_mosi_B15 = {gpio::PORTB, 15, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_mosi_C3 = {gpio::PORTC, 3, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_sck_B10 = {gpio::PORTB, 10, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_sck_B13 = {gpio::PORTB, 13, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_NSS_B12 = {gpio::PORTB, 12, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi2_NSS_B9 = {gpio::PORTB, 9, GPIO_AF_SPI2, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};

const ioconf ioconf_spi3_miso_B4 = {gpio::PORTB, 4, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_miso_C11 = {gpio::PORTC, 11, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_mosi_B5 = {gpio::PORTB, 5, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_mosi_C12 = {gpio::PORTC, 12, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_sck_C10 = {gpio::PORTC, 10, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_sck_B3 = {gpio::PORTB, 3, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_NSS_A4 = {gpio::PORTA, 4, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};
const ioconf ioconf_spi3_NSS_A15 = {gpio::PORTA, 15, GPIO_AF_SPI3, GPIO_OType_PP, GPIO_High_Speed, GPIO_PuPd_UP};

}

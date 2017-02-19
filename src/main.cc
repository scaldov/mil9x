#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <map>

#include <math.h>
#include "led.hh"
#include "os.hh"
#include "hw.hh"
#include "eeprom.hh"
#include "usart.hh"
#include "ataka.hh"
#include "spi.hh"

//#include "murmur3.hh"
//d428

void task_interface(void *);
void task_semtest(void *);

lk::thread thread_interface(task_interface, "LED", 1024, 0, 4);

lk::led led0(lk::gpio_d, 10);
lk::led led1(lk::gpio_d, 11);
lk::led led2(lk::gpio_d, 12);
lk::led led3(lk::gpio_d, 13);
lk::pin led4(lk::gpio_d, 14);

/**
 * Thread performs LED control by light pattern.
 */
void task_interface(void *param) {
    //	std::map<int, int> c;
    //	c[3] = 3;
    int led_t = 0;
    int led_i = 0;
    int segdisp_i = 0;
    int sin_pos = 0;
    int i = 0;
    led1.pattern = lk::led::PTRN_BLINK;
    led2.pattern = lk::led::PTRN_ALARM;
    led3.pattern = 0x3101;
    for (;;) {
        lk::led::clk();
        vTaskDelay(200);
        i++;
        if (i % 500 == 0) {
            printf_("led 500 cycles\n");
        }
        if (i % 100 == 0){
            //switch signals
            int u[16];
            for(int i = 0; i < 16; i ++) {
                for(int j = 0; j < 32; j ++) {
                    if((dma_adc2_bfr_hi[j] >> 16) == i) u[i] = dma_adc2_bfr_hi[j] & 0xffff;
                }
            }
            printf_("u[0]=%d\tu[1]=%d\tu[2]=%d\tu[3]=%d\t" "u[4]=%d\tu[5]=%d\tu[6]=%d\tu[7]=%d\t"
                    "u[8]=%d\tu[9]=%d\tu[10]=%d\tu[11]=%d\t" "u[12]=%d\tu[13]=%d\tu[14]=%d\tu[15]=%d\n",
                    u[0] * 32340 >>12, u[1] * 32340 >>12, u[2] * 32340 >>12, u[3] * 32340 >>12,
                    u[4] * 32340 >>12, u[5] * 32340 >>12, u[6] * 32340 >>12, u[7] * 32340 >>12,
                    u[8] * 32340 >>12, u[9] * 32340 >>12, u[10] * 32340 >>12, u[11] * 32340 >>12,
                    u[12] * 32340 >>12, u[13] * 32340 >>12, u[14] * 32340 >>12, u[15] * 32340 >>12);
        }
    }
}

ataka emulator;
lk::spi spi1slave(1, 0);
lk::spi spi2master(2, 1);

//lk::eeprom::nvram config(4 * 31, 4, 0);
lk::eeprom::nvram config(0, 4, lk::eeprom::INFO);

void print_hex (void *p, int addr, int len) {
    uint8_t *ptr = (uint8_t*) p;
    int j, i;
    int l = (len + 0xF) & ~0xF;
    for(i = addr & ~0x0F; i < (addr + l); i ++)
    {
        if((i & 0xF) == 0) {
            printf_("%08X\t", i);
        }
        if(i < addr) {
            printf_("   ");
        } else {
            printf_("%02X ", *ptr);
            ptr ++;
        }
        if((i & 0xF) == 0xF) {
            printf_("\n");
        }
    }
}


int main() {
    setbuf(stdout, NULL);
    printf_("data+bss=%d\tfree=%d\n", sizeof(void*) * (&__bss_end__ - &__data_start__), sizeof(void*) * (&__StackLimit - &__bss_end__));
//    printf_("ramfunc section length=%d\n", &__exec_mem_end__ - &__exec_mem_start__);
    lk::cpu::cpuclk();
    emulator.init();
    lk::thread thread_ataka_emu(ataka::task_ataka_emu, "ataka", 1024, ataka::intance, 0);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    //init ports
    //    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF;
    printf_("clock=%d\n", SystemCoreClock);
    printf_("DMA_ControlTable=%08X\n", &DMA::controlTable[0]);
    printf_("sizeof(DMA::ControlData)=%i\n", sizeof(DMA::ControlData));
    printf_("dma_adc1_bfr_lo=%p\n", dma_adc1_bfr_lo);
    printf_("dma_adc1_bfr_hi=%p\n", dma_adc1_bfr_hi);
    printf_("dma_adc2_bfr_lo=%p\n", dma_adc2_bfr_lo);
    printf_("dma_adc2_bfr_hi=%p\n", dma_adc2_bfr_hi);
    printf_("DMA::Chn::ADC2=%08X\n", DMA::Chn::ADC2);
    printf_("(1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1)=%08X\n", (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1));
    Reg32 x1 = 0;
    printf("Reg<>== (1): %i\n", (x1 == DMA::Ctrl::Cycle::Stop) ? 1 : 0);
    x1 = 1;
    printf("Reg<>== (0): %i\n", (x1 == DMA::Ctrl::Cycle::Stop) ? 1 : 0);
    x1 = 1 | 0x8000;
    printf("Reg<>== (1): %i\n", (x1 == DMA::Ctrl::Cycle::Basic) ? 1 : 0);
    x1 = 1<<DMA::Chn::ADC1;
    RegMap x2 = RegMap((uint32_t)&x1);
    printf("Regmap == (1): %i\n", ((int)x2 & (1<<DMA::Chn::ADC1)) ? 1 : 0);
    x1 = (int)x1 << 1;
    printf("Regmap == (0): %i\n", ((int)x2 & (1<<DMA::Chn::ADC1)) ? 1 : 0);
    x1 = (int)x1 >> 1;
    printf("Regmap == (1): %i\n", ((int)x2 & (1<<DMA::Chn::ADC1)) ? 1 : 0);
    printf_("PortA.baseAddr=%08X\n", Port(Port::B).baseAddr);
    printf_("PortX(1).baseAddr=%08X\n", Port(1).baseAddr);
    RST::perClock |= RST::PerClock::Port::F;
    IOConf::Config(IOConf::SSP1::CLK::B13);
    IOConf::Config(IOConf::SSP1::RX::B14);
    SSP(0).cr0 = SSP::CR0::SCR(0b00);

    for(int i = 0; i < 4096/4; i ++) {
        config.memory[i] = i;
    }
    Reg32 cmd = EEPROM::cmd & EEPROM::CMD::Delay.mask();
    ConstRegMap cmdreg = ConstRegMap((uint32_t)&cmd.x);
    uint32_t cmd1 = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
    printf("eeprom %08X\t%08X\t%08X\t%08X\n", (int)cmd, cmd1, EEPROM::CMD::Delay.mask(), EEPROM_CMD_DELAY_Msk);
    cmd = EEPROM::cmd & EEPROM::CMD::Delay.mask();
    cmd1 = MDR_EEPROM->CMD & EEPROM::CMD::Delay.mask();
    printf("eeprom %08X\t%08X\t%08X\t%08X\n", cmd, cmd1, EEPROM::CMD::Delay.mask(), EEPROM_CMD_DELAY_Msk);
    printf("eeprom %08X\t%08X\n", (int)EEPROM::CMD::IFREN, EEPROM_CMD_IFREN);
    printf_("eeprom bits = %08X\n", ((EEPROM::CMD::MAS1 | EEPROM::CMD::ERASE | EEPROM::CMD::IFREN | EEPROM::CMD::SE)) );
    printf_("eeprom ~bits = %08X\n", (~(EEPROM::CMD::MAS1 | EEPROM::CMD::ERASE | EEPROM::CMD::IFREN | EEPROM::CMD::SE)) );
    cmdreg |= EEPROM::CMD::XE;
    printf_("cmd |= EEPROM::CMD::YE = %08X\n", (int)cmdreg );
    cmdreg &= ~EEPROM::CMD::XE;
    printf_("cmd &= ~EEPROM::CMD::YE = %08X\n", (int)cmdreg );
    printf_("EEPROM::CMD::XE = %08X\n", (int)~EEPROM::CMD::XE );
    printf_("EEPROM::CMD::YE = %08X\n", (int)~EEPROM::CMD::YE );
    printf_("eeprom IFREN(0) = %08X\n", EEPROM::CMD::IFREN(0));
    printf_("eeprom IFREN(1) = %08X\n", EEPROM::CMD::IFREN(1));
//
//    printf_("murmur3('hello, world') = %08X\n", cx::murmur3_32("hello, world", 0) );
    //

    printf_("&mem=%p\n", config.memory);
    int r = config.read(lk::eeprom::VERIFY);
    printf_("verify=%d\n", r);
    config.write();
    r = config.read(lk::eeprom::VERIFY);
    printf_("verify=%d\n", r);
    config.read(0);
//    print_hex (config.memory, 0x08000000, 64);
    config.memory[100] = 0xAAAAAAAA;
    r = config.read(lk::eeprom::VERIFY);
    printf_("verify=%d\n", r);
    config.write();
    r = config.read(lk::eeprom::VERIFY);
    printf_("verify=%d\n", r);
    config.read(0);
//    print_hex (config.memory + 96, 0x08000000 + 96, 64);

    lk::os::start();
}


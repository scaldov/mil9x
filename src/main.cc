#include "hw.hh"
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <map>

#include <math.h>
#include "led.hh"
#include "os.hh"
#include "eeprom.hh"
#include "usart.hh"
#include "ataka.hh"
//#include "spi.hh"
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

lk::eeprom::nvram config(0, 4, lk::eeprom::INFO);
void print_hex (void *p, int addr, int len) {
        uint8_t *ptr = (uint8_t*) p;
        int j, i;
        int l = (len + 0xF) & ~0xF;
        for(i = addr & ~0x0F; i < (addr + l); i ++)
        {
                if((i & 0xF) == 0) {
                        printf("%08X\t", i);
                }
                if(i < addr) {
                        printf("   ");
                } else {
                        printf("%02X ", *ptr);
                        ptr ++;
                }
                if((i & 0xF) == 0xF) {
                printf("\n");
                }
        }
}


int main() {
    setbuf(stdout, NULL);
    printf_("data+bss=%d\tfree=%d\n", sizeof(void*) * (&__bss_end__ - &__data_start__), sizeof(void*) * (&__StackLimit - &__bss_end__));
    lk::cpu::cpuclk();
    emulator.init();
    lk::thread thread_ataka_emu(ataka::task_ataka_emu, "ataka", 1024, ataka::intance, 0);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    //init ports
//    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF;
    printf_("clock=%d\n", SystemCoreClock);
    printf_("DMA_ControlTable=%08X\n", &DMA_::controlTable[0]);
    printf_("sizeof(DMA::ControlData)=%i\n", sizeof(DMA::ControlData));
    printf_("PortA.baseAddr=%08X\n", PortB.baseAddr);
    printf_("PortX(1).baseAddr=%08X\n", Port(1).baseAddr);


    for(int i = 0; i < 4096/4; i ++) {
        config.memory[i] = i;
    }
    printf("&mem=%p\n", config.memory);
    int r = config.read(lk::eeprom::VERIFY);
    printf("verify=%d\n", r);
    config.write();
    r = config.read(lk::eeprom::VERIFY);
    printf("verify=%d\n", r);
    config.read(0);
    print_hex (config.memory, 0x08000000, 64);
    config.memory[100] = 0xAAAAAAAA;
    r = config.read(lk::eeprom::VERIFY);
    printf("verify=%d\n", r);
    config.write();
    r = config.read(lk::eeprom::VERIFY);
    printf("verify=%d\n", r);
    config.read(0);
    print_hex (config.memory + 96, 0x08000000 + 96, 64);

    lk::os::start();
}


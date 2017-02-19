#pragma once

#include "hw.hh"
#include "os.hh"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

BaseAddr constexpr EEPROMBase (int i) {
    return i == 0 ? MDR_EEPROM_BASE :  MDR_EEPROM_BASE;
}

class EEPROM_ {
public:
    struct CMD{
        BITDEF(NVSTR, BitAttr(13,1), 1);
        BITDEF(PROG, BitAttr(12,1), 1);
        BITDEF(MAS1, BitAttr(11,1), 1);
        BITDEF(ERASE, BitAttr(10,1), 1);
        BITDEF(IFREN, BitAttr(9,1), 1);
        BITDEF(SE, BitAttr(8,1), 1);
        BITDEF(YE, BitAttr(7,1), 1);
        BITDEF(XE, BitAttr(6,1), 1);
        BITDEF(Delay, BitAttr(3,3), 0);
        BITDEF(RD, BitAttr(2,1), 1);
        BITDEF(WR, BitAttr(1,1), 1);
        BITDEF(CON, BitAttr(0,1), 1);
    };
    static constexpr int FREQ = 128_MHz;
    static constexpr int TNVS = 5_us;       //nonvolatile select
    static constexpr int TME = 40_ms;       //memory erase
    static constexpr int TNVH1 = 100_us;    //nonvolatile hold 1
    static constexpr int TREV = 1_us;       //reverse
    static constexpr int TPGS = 10_us;      //page select
    static constexpr int TPROG = 40_us;     //programming
    static constexpr int TADH = 20_ns;      //address hold
    static constexpr int TPGH = 20_ns;      //page hold
    static constexpr int TNVH = 5_us;       //nonvolatile hold
    static constexpr int TXA = 30_us;       //X access
    static constexpr int TYA = 30_us;       //Y access
    static constexpr int KEY = ((uint32_t)0x8AAA5551);
};

class EEPROM : public EEPROM_ {
public:
    static constexpr auto cmd = ConstRegMap(EEPROMBase(0) + 0x00);
    static constexpr auto addr = ConstRegMap(EEPROMBase(0) + 0x04);
    static constexpr auto DI = ConstRegMap(EEPROMBase(0) + 0x08);
    static constexpr auto DO = ConstRegMap(EEPROMBase(0) + 0x0c);
    static constexpr auto key = ConstRegMap(EEPROMBase(0) + 0x10);
};


namespace lk {

class eeprom {
public:
    enum flag {INFO = 1, VERIFY = 2};


    static inline void wait(uint32_t tick)
    {
        SysTick->LOAD = tick;
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
        while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    static inline void const begin(uint32_t &systick_ctrl)
    {
        systick_ctrl = SysTick->CTRL;
        SysTick->LOAD = 0xFFFFFF;
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
    }
    static inline void const end(uint32_t &systick_ctrl)
    {
        SysTick->CTRL = systick_ctrl;
    }

    static uint32_t const addr_next(uint32_t addr) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        if((addr & 0b111111110000) ^ 0b111111110000) {
            return addr + 0x0010;
        }
        addr = addr & ~0b111111110000;
        if((addr & 0b000000001100) ^ 0b000000001100) {
            return addr + 0x0004;
        }
        addr = addr & ~0b000000001100;
        return addr + 0x1000;
    }

    static void const addr_xe_set(uint32_t addr) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        EEPROM::addr = addr;
        EEPROM::cmd |= EEPROM::CMD::XE | EEPROM::CMD::PROG;
        wait(systick::nsec(EEPROM::TNVS, EEPROM::FREQ));
        EEPROM::cmd |= EEPROM::CMD::NVSTR;
        wait(systick::nsec(EEPROM::TPGS, EEPROM::FREQ));
    }

    static void const addr_xe_reset(void) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        EEPROM::cmd &= ~EEPROM::CMD::PROG;
        wait(systick::nsec(EEPROM::TNVH, EEPROM::FREQ));
        EEPROM::cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::NVSTR);
    }

    static void const erase_page_sector(uint32_t addr, uint32_t flag) __attribute__ ((noinline))  __attribute__ ((__section__(".data.ramfunc")))
    {
        uint32_t systick_ctrl;
        Reg32 cmd = EEPROM::cmd & EEPROM::CMD::Delay.mask();
        __disable_irq();
        begin(systick_ctrl);
        EEPROM::key = EEPROM::KEY;
        EEPROM::cmd |= EEPROM::CMD::CON;
        EEPROM::addr = addr;
        EEPROM::cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::YE | EEPROM::CMD::SE | EEPROM::CMD::MAS1 |
                             EEPROM::CMD::ERASE | EEPROM::CMD::NVSTR | EEPROM::CMD::PROG);
        EEPROM::cmd |= EEPROM::CMD::XE | EEPROM::CMD::ERASE  | EEPROM::CMD::IFREN((flag & INFO) ? 1 : 0);
        wait(systick::nsec(EEPROM::TNVS, EEPROM::FREQ));
        EEPROM::cmd |= EEPROM::CMD::NVSTR;
        wait(systick::nsec(EEPROM::TME, EEPROM::FREQ));
        EEPROM::cmd &= ~EEPROM::CMD::ERASE;
        wait(systick::nsec(EEPROM::TNVH1, EEPROM::FREQ));
        EEPROM::cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::NVSTR);
        wait(systick::nsec(EEPROM::TREV, EEPROM::FREQ));
        EEPROM::cmd = cmd;
        wait(systick::nsec(EEPROM::TME, EEPROM::FREQ));
        end(systick_ctrl);
        __enable_irq();
    }

    static void const write(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        Reg32 cmd = EEPROM::cmd & EEPROM::CMD::Delay.mask();
        uint32_t *data = (uint32_t*)ram;
        uint32_t prev_addr;
        uint32_t systick_ctrl;
        __disable_irq();
        begin(systick_ctrl);
        EEPROM::key = EEPROM::KEY;
        EEPROM::cmd |= EEPROM::CMD::CON;
        EEPROM::cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::YE | EEPROM::CMD::SE | EEPROM::CMD::MAS1 |
                             EEPROM::CMD::ERASE | EEPROM::CMD::NVSTR | EEPROM::CMD::PROG);
        EEPROM::cmd |= EEPROM::CMD::IFREN((flag & INFO) ? 1 : 0);
        prev_addr = addr;
        addr_xe_set(addr);
        for(int i = 0; i < len; i ++) {
            if((addr ^ prev_addr) & 0b1100) {
                addr_xe_reset();
                addr_xe_set(addr);
            }
            EEPROM::addr = addr;
            EEPROM::DI = *data;
            wait(systick::nsec(EEPROM::TADH, EEPROM::FREQ));
            EEPROM::cmd |= EEPROM::CMD::YE;
            wait(systick::nsec(EEPROM::TPROG, EEPROM::FREQ));
            EEPROM::cmd &= ~EEPROM::CMD::YE;
            wait(systick::nsec(EEPROM::TADH, EEPROM::FREQ));
            prev_addr = addr;
            addr = by_sector ? addr_next(addr) : addr;
            data ++;
        }
        addr_xe_reset();
        wait(systick::nsec(EEPROM::TREV, EEPROM::FREQ));
        EEPROM::cmd = cmd;
        end(systick_ctrl);
        __enable_irq();
    }

    static int const read(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        uint32_t systick_ctrl;
        int verify = 0;
        uint32_t word;
        Reg32 cmd = EEPROM::cmd & EEPROM::CMD::Delay.mask();
        uint32_t *data = (uint32_t*)ram;
        __disable_irq();
        begin(systick_ctrl);
        EEPROM::key = EEPROM::KEY;
        EEPROM::cmd |= EEPROM::CMD::CON;
        EEPROM::cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::YE | EEPROM::CMD::SE | EEPROM::CMD::MAS1 |
                         EEPROM::CMD::ERASE | EEPROM::CMD::NVSTR | EEPROM::CMD::PROG);
        EEPROM::cmd |= EEPROM::CMD::XE | EEPROM::CMD::YE | EEPROM::CMD::SE | EEPROM::CMD::IFREN((flag & INFO) ? 1 : 0);
        for(int i = 0; i < len; i ++){
            EEPROM::addr = addr;
            wait(systick::nsec(EEPROM::TXA, EEPROM::FREQ));
            word = (int)EEPROM::DO;
            verify += (word ^ *data) ? 1 : 0;
            if(!(flag & VERIFY)) *data = word;
            addr = by_sector ? addr_next(addr) : addr;
            data ++;
        }
        EEPROM::cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::YE | EEPROM::CMD::SE);
        wait(systick::nsec(EEPROM::TREV*10, EEPROM::FREQ));
        EEPROM::cmd = cmd;
        end(systick_ctrl);
        __enable_irq();
        return verify;
    }

    static void const write_word(uint32_t addr, uint32_t data, uint32_t flag) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        uint32_t w = data;
        write(&w, addr, 1, 0, flag);
    }

    static uint32_t const read_word(uint32_t addr, uint32_t flag) __attribute__ ((noinline)) __attribute__ ((__section__(".data.ramfunc")))
    {
        uint32_t data;
        read(&data, addr, 1, 0, flag & INFO);
        return data;
    }

    static inline const uint32_t page(int p)
    {
        return 4096 * p;
    }

    class sector {
    public:
        enum sector_name {A = 0, B = 4, C = 8, D = 12};
        static void write(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t info);
        static int read(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t info);
    };
    class linear {
    public:
        static void write(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t flag);
        static int read(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t flag);
    };
    //operate via sector update map
    class nvram {
    public:
        uint32_t *memory;
        uint32_t *bitmap;
        int start;
        int n_sectors;
        uint32_t flag;
        nvram(int start_sector, int num_sectors, int flag) : flag(flag) {
            start = start_sector;
            n_sectors = num_sectors;
            memory = new uint32_t[num_sectors * 1024 / 4];
            bitmap = new uint32_t[(num_sectors + 31) >> 5];
        }
        int read(int verify_flag) {
            int offset = ((start & 0b11) << 2) | ((start & 0b1111100) << 10);
            return sector::read(memory, 0x08000000 + offset, 1024 / 4 * n_sectors, (flag & INFO) | (verify_flag & VERIFY));
        }
        void write() {
            for(int i = 0; i < n_sectors; i++) {
                int offset = (((i + start) & 0b11) << 2) | (((i + start) & 0b1111100) << 10);
                int addr = 0x08000000 + offset;
                int r = sector::read(memory + i * 1024 / 4, addr, 1024 / 4, (flag & INFO) | VERIFY);
                if(r) erase_page_sector(addr, (flag & INFO));
                sector::write(memory + i * 1024 / 4, addr, 1024 / 4, (flag & INFO));
            }
        }
    };
};

}

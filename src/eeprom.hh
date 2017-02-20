#ifndef __LK_EEPROM_HH__
#define __LK_EEPROM_HH__

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
    static constexpr int TNVS = 5_us;
    static constexpr int TME = 40_ms;
    static constexpr int TNVH1 = 100_us;
    static constexpr int TREV = 1_us;
    static constexpr int TPGS = 10_us;
    static constexpr int TPROG = 40_us;
    static constexpr int TADH = 20_ns;
    static constexpr int TPGH = 20_ns;
    static constexpr int TNVH = 5_us;
    static constexpr int TXA = 30_us;
    static constexpr int TYA = 30_us;
    static constexpr int KEY = ((uint32_t)0x8AAA5551);
};

class EEPROM : public EEPROM_ {
public:
    uint32_t baseAddr;
    EEPROM(BaseAddr base) : baseAddr(base) {
        cmd = RegMap(baseAddr + 0x00);
        addr = RegMap(baseAddr + 0x04);
        DI = RegMap(baseAddr + 0x08);
        DO = RegMap(baseAddr + 0x0c);
        key = RegMap(baseAddr + 0x10);
    }
    EEPROM(int base = 0) : EEPROM(EEPROMBase(base)) {}
    RegMap cmd;
    RegMap addr;
    RegMap DI;
    RegMap DO;
    RegMap key;
};


namespace lk {

class eeprom {
public:
    static constexpr int FLASH_PROG_FREQ = 128_MHz;
    static constexpr int FLASH_PROG_TNVS = 5_us;
    static constexpr int FLASH_PROG_TME = 40_ms;
    static constexpr int FLASH_PROG_TNVH1 = 100_us;
    static constexpr int FLASH_PROG_TREV = 1_us;
    static constexpr int FLASH_PROG_TPGS = 10_us;
    static constexpr int FLASH_PROG_TPROG = 40_us;
    static constexpr int FLASH_PROG_TADH = 20_ns;
    static constexpr int FLASH_PROG_TPGH = 20_ns;
    static constexpr int FLASH_PROG_TNVH = 5_us;
    static constexpr int FLASH_PROG_TXA = 30_us;
    static constexpr int FLASH_PROG_TYA = 30_us;
    static constexpr int FLASH_PROG_KEY = ((uint32_t)0x8AAA5551);
    enum flag {INFO = 1, VERIFY = 2};
    static void erase_page_sector(uint32_t addr, uint32_t flag)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static uint32_t addr_next(uint32_t addr)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static void addr_xe_set(uint32_t addr)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static void addr_xe_reset(void)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static void write(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static int read(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static void write_word(uint32_t addr, uint32_t data, uint32_t flag)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static uint32_t read_word(uint32_t addr, uint32_t flag)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    static constexpr uint32_t page(int p)__attribute__ ((noinline))__attribute__ ((__section__(".data")));
    class sector {
    public:
        enum sector_name {A = 0, B = 4, C = 8, D = 12};
        static void write(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t info)
        {
            eeprom::write(ram, eeprom, len, 1, info);
        }
        static int read(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t info)
        {
            return eeprom::read(ram, eeprom, len, 1, info);
        }
    };
    class linear {
    public:
        static void write(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t flag)
        {
            eeprom::write(ram, eeprom, len, 0, flag);
        }
        static int read(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t flag)
        {
            return eeprom::read(ram, eeprom, len, 0, flag);
        }
    };
    //operate via sector update map
    class nvram {
    public:
        uint32_t *memory;
        uint32_t *bitmap;
        //        uint32_t memory[1024];
        //        uint32_t bitmap[128];
        int start;
        int n_sectors;
        uint32_t flag;
        nvram(int start_sector, int num_sectors, int flag) {
            start = start_sector;
            n_sectors = num_sectors;
            memory = new uint32_t[num_sectors * 1024 / 4];
            bitmap = new uint32_t[(num_sectors + 31) >> 5];
            nvram::flag = INFO;
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

#endif

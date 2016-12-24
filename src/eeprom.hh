#ifndef __LK_EEPROM_HH__
#define __LK_EEPROM_HH__

#include "hw.hh"
#include "os.hh"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace lk {

#define FLASH_PROG_FREQ		128_MHz
#define FLASH_PROG_TNVS		5_us
#define FLASH_PROG_TME		40_ms
#define FLASH_PROG_TNVH1	100_us
#define FLASH_PROG_TREV		1_us
#define FLASH_PROG_TPGS		10_us
#define FLASH_PROG_TPROG	40_us
#define FLASH_PROG_TADH		20_ns
#define FLASH_PROG_TPGH		20_ns
#define FLASH_PROG_TNVH		5_us
#define FLASH_PROG_TXA		30_us
#define FLASH_PROG_TYA		30_us
#define FLASH_PROG_KEY		((uint32_t)0x8AAA5551)

class eeprom {
public:
//static constexpr int FLASH_PROG_FREQ = 128_MHz;
//static constexpr int FLASH_PROG_TNVS = 5_us;
//static constexpr int FLASH_PROG_TME = 40_ms;
//static constexpr int FLASH_PROG_TNVH1 = 100_us;
//static constexpr int FLASH_PROG_TREV = 1_us;
//static constexpr int FLASH_PROG_TPGS = 10_us;
//static constexpr int FLASH_PROG_TPROG = 40_us;
//static constexpr int FLASH_PROG_TADH = 20_ns;
//static constexpr int FLASH_PROG_TPGH = 20_ns;
//static constexpr int FLASH_PROG_TNVH = 5_us;
//static constexpr int FLASH_PROG_TXA = 30_us;
//static constexpr int FLASH_PROG_TYA = 30_us;
//static constexpr int FLASH_PROG_KEY = ((uint32_t)0x8AAA5551);
	enum flag {INFO = 1, VERIFY = 2};
	static void erase_page_sector(uint32_t addr, uint32_t flag)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
	{
		uint32_t cmd = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
		__disable_irq();
		systick::begin();
		MDR_EEPROM->KEY = FLASH_PROG_KEY;
		MDR_EEPROM->CMD |= EEPROM_CMD_CON;
		MDR_EEPROM->ADR = addr;
		MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE | EEPROM_CMD_MAS1 |
				EEPROM_CMD_ERASE | EEPROM_CMD_NVSTR | EEPROM_CMD_PROG);
		MDR_EEPROM->CMD |= EEPROM_CMD_XE | EEPROM_CMD_ERASE  | ((flag & INFO) ? EEPROM_CMD_IFREN : 0);
		systick::wait(systick::nsec(FLASH_PROG_TNVS, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD |= EEPROM_CMD_NVSTR;
		systick::wait(systick::nsec(FLASH_PROG_TME, FLASH_PROG_FREQ));
		/*
		//40 ms is too long so we yield a time to scheduler
		lk::systick::end();
		__enable_irq();
		os::wait(os::usec(40000));
		__disable_irq();
		systick::begin();
		 */
		MDR_EEPROM->CMD &= ~EEPROM_CMD_ERASE;
		systick::wait(systick::nsec(FLASH_PROG_TNVH1, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
		systick::wait(systick::nsec(FLASH_PROG_TREV, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD = cmd;
		systick::wait(systick::nsec(FLASH_PROG_TME, FLASH_PROG_FREQ));
		lk::systick::end();
		__enable_irq();
	}

	static uint32_t addr_next(uint32_t addr)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
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

	static void addr_xe_set(uint32_t addr)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
	{
		MDR_EEPROM->ADR = addr;
		MDR_EEPROM->CMD |= EEPROM_CMD_XE | EEPROM_CMD_PROG;
		systick::wait(systick::nsec(FLASH_PROG_TNVS, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD |= EEPROM_CMD_NVSTR;
		systick::wait(systick::nsec(FLASH_PROG_TPGS, FLASH_PROG_FREQ));
	}

	static void addr_xe_reset(void)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
	{
		MDR_EEPROM->CMD &= ~EEPROM_CMD_PROG;
		systick::wait(systick::nsec(FLASH_PROG_TNVH, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
	}

	static void write(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
	{
		uint32_t cmd = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
		uint32_t *data = (uint32_t*)ram;
		uint32_t prev_addr;
		__disable_irq();
		systick::begin();
		MDR_EEPROM->KEY = FLASH_PROG_KEY;
		MDR_EEPROM->CMD |= EEPROM_CMD_CON;
		MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE | EEPROM_CMD_MAS1 |
				EEPROM_CMD_ERASE | EEPROM_CMD_NVSTR | EEPROM_CMD_PROG);
		MDR_EEPROM->CMD |= (flag & INFO) ? EEPROM_CMD_IFREN : 0;
		prev_addr = addr;
		addr_xe_set(addr);
		for(int i = 0; i < len; i ++) {
			if((addr ^ prev_addr) & 0b1100) {
				addr_xe_reset();
				addr_xe_set(addr);
			}
			MDR_EEPROM->ADR = addr;
			MDR_EEPROM->DI = *data;
			systick::wait(systick::nsec(FLASH_PROG_TADH, FLASH_PROG_FREQ));
			MDR_EEPROM->CMD |= EEPROM_CMD_YE;
			systick::wait(systick::nsec(FLASH_PROG_TPROG, FLASH_PROG_FREQ));
			MDR_EEPROM->CMD &= ~EEPROM_CMD_YE;
			systick::wait(systick::nsec(FLASH_PROG_TADH, FLASH_PROG_FREQ));
			prev_addr = addr;
			addr = by_sector ? addr_next(addr) : addr;
			data ++;
		}
		addr_xe_reset();
		systick::wait(systick::nsec(FLASH_PROG_TREV, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD = cmd;
		systick::end();
		__enable_irq();
	}

	static int read(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
	{
		int verify = 0;
		uint32_t word;
		uint32_t cmd = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
		uint32_t *data = (uint32_t*)ram;
		__disable_irq();
		systick::begin();
		MDR_EEPROM->KEY = FLASH_PROG_KEY;
		MDR_EEPROM->CMD |= EEPROM_CMD_CON;
		MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE | EEPROM_CMD_MAS1 |
				EEPROM_CMD_ERASE | EEPROM_CMD_NVSTR | EEPROM_CMD_PROG);
		MDR_EEPROM->CMD |= EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE | ((flag & INFO) ? EEPROM_CMD_IFREN : 0);
		for(int i = 0; i < len; i ++){
			MDR_EEPROM->ADR = addr;
			systick::wait(systick::nsec(FLASH_PROG_TXA, FLASH_PROG_FREQ));
			word = MDR_EEPROM->DO;
			verify += (word ^ *data) ? 1 : 0;
			if(!(flag & VERIFY)) *data = word;
			addr = by_sector ? addr_next(addr) : addr;
			data ++;
		}
		MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE);
		systick::wait(systick::nsec(FLASH_PROG_TREV*10, FLASH_PROG_FREQ));
		MDR_EEPROM->CMD = cmd;
		systick::end();
		__enable_irq();
		return verify;
	}

	static void write_word(uint32_t addr, uint32_t data, uint32_t flag)
	{
		uint32_t w = data;
		write(&w, addr, 1, 0, flag);
	}

	static uint32_t read_word(uint32_t addr, uint32_t flag)
	{
		uint32_t data;
		read(&data, addr, 1, 0, flag & INFO);
		return data;
	}

	static constexpr uint32_t page(int p){
		return 4096 * p;
	}

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
//        uint32_t *memory;
//		uint32_t *bitmap;
        uint32_t memory[1024];
        uint32_t bitmap[128];
        int start;
		int n_sectors;
		uint32_t flag;
		nvram(int start_sector, int num_sectors, int flag) {
			start = start_sector;
			n_sectors = num_sectors;
//			memory = new uint32_t[num_sectors * 1024 / 4];
//			bitmap = new uint32_t[(num_sectors + 31) >> 5];
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

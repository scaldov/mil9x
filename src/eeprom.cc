#include "eeprom.hh"

#include "pin.hh"
extern lk::pin led4;

namespace lk {

void eeprom::erase_page_sector(uint32_t addr, uint32_t flag)
{
    uint32_t cmd = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
    __disable_irq();
    systick::begin();
    EEPROM().key = EEPROM::KEY;
    EEPROM().cmd = EEPROM::CMD::CON;
    EEPROM().addr = addr;
    EEPROM().cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::YE | EEPROM::CMD::SE | EEPROM::CMD::MAS1 |
                       EEPROM::CMD::ERASE | EEPROM::CMD::NVSTR | EEPROM::CMD::PROG);
    EEPROM().cmd |= EEPROM::CMD::XE | EEPROM::CMD::ERASE  | EEPROM::CMD::IFREN((flag & INFO) ? 1 : 0);
//    MDR_EEPROM->KEY = FLASH_PROG_KEY;
//    MDR_EEPROM->CMD |= EEPROM_CMD_CON;
//    MDR_EEPROM->ADR = addr;
//    MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE | EEPROM_CMD_MAS1 |
//                         EEPROM_CMD_ERASE | EEPROM_CMD_NVSTR | EEPROM_CMD_PROG);
//    MDR_EEPROM->CMD |= EEPROM_CMD_XE | EEPROM_CMD_ERASE  | ((flag & INFO) ? EEPROM_CMD_IFREN : 0);
    systick::wait(systick::nsec(FLASH_PROG_TNVS, FLASH_PROG_FREQ));
    EEPROM().cmd |= EEPROM::CMD::NVSTR;
//    MDR_EEPROM->CMD |= EEPROM_CMD_NVSTR;
    systick::wait(systick::nsec(FLASH_PROG_TME, FLASH_PROG_FREQ));
    /*
    //40 ms is too long so we yield a time to scheduler
    lk::systick::end();
    __enable_irq();
    os::wait(os::usec(40000));
    __disable_irq();
    systick::begin();
     */
    EEPROM().cmd &= ~EEPROM::CMD::ERASE;
//    MDR_EEPROM->CMD &= ~EEPROM_CMD_ERASE;
    systick::wait(systick::nsec(FLASH_PROG_TNVH1, FLASH_PROG_FREQ));
    EEPROM().cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::NVSTR);
//    MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
    systick::wait(systick::nsec(FLASH_PROG_TREV, FLASH_PROG_FREQ));
    EEPROM().cmd = cmd;
//    MDR_EEPROM->CMD = cmd;
    systick::wait(systick::nsec(FLASH_PROG_TME, FLASH_PROG_FREQ));
    lk::systick::end();
    __enable_irq();
}

uint32_t eeprom::addr_next(uint32_t addr)
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

void eeprom::addr_xe_set(uint32_t addr)
{
    EEPROM().addr = addr;
//    MDR_EEPROM->ADR = addr;
    EEPROM().cmd |= EEPROM::CMD::XE | EEPROM::CMD::PROG;
//    MDR_EEPROM->CMD |= EEPROM_CMD_XE | EEPROM_CMD_PROG;
    systick::wait(systick::nsec(FLASH_PROG_TNVS, FLASH_PROG_FREQ));
    EEPROM().cmd |= EEPROM::CMD::NVSTR;
//    MDR_EEPROM->CMD |= EEPROM_CMD_NVSTR;
    systick::wait(systick::nsec(FLASH_PROG_TPGS, FLASH_PROG_FREQ));
}

void eeprom::addr_xe_reset(void)
{
    EEPROM().cmd &= ~EEPROM::CMD::PROG;
//    MDR_EEPROM->CMD &= ~EEPROM_CMD_PROG;
    systick::wait(systick::nsec(FLASH_PROG_TNVH, FLASH_PROG_FREQ));
    EEPROM().cmd &= ~(EEPROM::CMD::XE | EEPROM::CMD::NVSTR);
//    MDR_EEPROM->CMD &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
}

void eeprom::write(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag)
{
    Reg<uint32_t> cmd1 = EEPROM().cmd & EEPROM::CMD::Delay.mask();
    int xx = EEPROM::CMD::Delay.mask();
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

int eeprom::read(void *ram, uint32_t addr, int len, int by_sector, uint32_t flag)
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

void eeprom::write_word(uint32_t addr, uint32_t data, uint32_t flag)
{
    uint32_t w = data;
    write(&w, addr, 1, 0, flag);
}

uint32_t eeprom::read_word(uint32_t addr, uint32_t flag)
{
    uint32_t data;
    read(&data, addr, 1, 0, flag & INFO);
    return data;
}

constexpr uint32_t eeprom::page(int p){
    return 4096 * p;
}


}

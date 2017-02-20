#include "eeprom.hh"

#include "pin.hh"
extern lk::pin led4;

namespace lk {

void eeprom::sector::write(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t info)
{
    eeprom::write(ram, eeprom, len, 1, info);
}
int eeprom::sector::read(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t info)
{
    return eeprom::read(ram, eeprom, len, 1, info);
}

void eeprom::linear::write(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t flag)
{
    eeprom::write(ram, eeprom, len, 0, flag);
}
int eeprom::linear::read(uint32_t *ram, uint32_t eeprom, size_t len, uint32_t flag)
{
    return eeprom::read(ram, eeprom, len, 0, flag);
}

}

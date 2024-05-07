
#ifndef Z80_BUS_H
#define Z80_BUS_H
#include <stdint.h>

typedef struct {
    uint8_t data;
    uint8_t addr_lo;
    uint8_t addr_hi;
} Z80Bus;

void z80_bus_init();
Z80Bus z80_bus_read();

#endif //Z80_BUS_H

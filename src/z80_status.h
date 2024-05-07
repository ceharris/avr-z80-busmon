
#ifndef Z80_STATUS_H
#define Z80_STATUS_H
#include <stdbool.h>

void z80_status_init();
uint8_t z80_status_read();

bool is_z80_rd(uint8_t status);
bool is_z80_wr(uint8_t status);
bool is_z80_mreq(uint8_t status);
bool is_z80_iorq(uint8_t status);
bool is_z80_m1(uint8_t status);
bool is_z80_rfsh(uint8_t status);

#endif //Z80_STATUS_H

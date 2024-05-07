#include <avr/io.h>
#include <avr/interrupt.h>

#include "z80_status.h"
#include "step_control.h"

#define Z80_STAT_DDR DDRC
#define Z80_STAT_PIN PINC
#define Z80_STAT_PORT PORTC

#define Z80_RD _BV(PC0)
#define Z80_WR _BV(PC1)
#define Z80_MREQ _BV(PC2)
#define Z80_IORQ _BV(PC3)
#define Z80_M1 _BV(PC4)
#define Z80_RFSH _BV(PC5)
#define Z80_STAT_MASK (Z80_RD | Z80_WR | Z80_MREQ | Z80_IORQ | Z80_M1 | Z80_RFSH)

void z80_status_init() {
  Z80_STAT_DDR = ~Z80_STAT_MASK;
//    Z80_STAT_PORT |= Z80_STAT_MASK;
}

uint8_t z80_status_read() {
  return ~Z80_STAT_PIN & Z80_STAT_MASK;
}

inline bool is_z80_rd(uint8_t status) {
  return status & Z80_RD;
}

inline bool is_z80_wr(uint8_t status) {
  return status & Z80_WR;
}

inline bool is_z80_mreq(uint8_t status) {
  return status & Z80_MREQ;
}

inline bool is_z80_iorq(uint8_t status) {
  return status & Z80_IORQ;
}

inline bool is_z80_m1(uint8_t status) {
  return status & Z80_M1;
}

inline bool is_z80_rfsh(uint8_t status) {
  return status & Z80_RFSH;
}


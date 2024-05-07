#include <avr/io.h>
#include <util/delay.h>
#include <spi.h>

#include "z80_bus.h"

#define Z80_BUS_DDR DDRB
#define Z80_BUS_PORT PORTB
#define Z80_BUS_LOAD _BV(PB1)

void z80_bus_init() {
  Z80_BUS_DDR |= Z80_BUS_LOAD;
  Z80_BUS_PORT |= Z80_BUS_LOAD;
}

Z80Bus z80_bus_read() {
  Z80_BUS_PORT &= ~Z80_BUS_LOAD;
  _delay_us(1);
  Z80_BUS_PORT |= Z80_BUS_LOAD;
  Z80Bus sample;
  sample.data = spi_transfer(0);
  sample.addr_lo = spi_transfer(0);
  sample.addr_hi = spi_transfer(0);
  return sample;
}

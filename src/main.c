#include <avr/interrupt.h>
#include <spi.h>
#include <max7221.h>
#include "step_control.h"
#include "z80_bus.h"
#include "z80_status.h"


void setup() {
  z80_bus_init();
  z80_status_init();
  spi_init();
  spi_enable();
  max7221_init();
  max7221_config();
  max7221_spin_pattern();
  max7221_blank_display();
  step_control_init();
  sei();
}

void loop() {
  Z80Bus sample = z80_bus_read();
  uint8_t status = z80_status_read();
  if (!is_z80_rfsh(status)) {
    max7221_display_hex4(7, sample.data, is_z80_rd(status));
    max7221_display_hex4(6, sample.data >> 4, is_z80_wr(status));
    max7221_blank_digit(4);
    max7221_blank_digit(5);
    max7221_display_hex4(3, sample.addr_lo, is_z80_mreq(status));
    max7221_display_hex4(2, sample.addr_lo >> 4, is_z80_iorq(status));
    max7221_display_hex4(1, sample.addr_hi, is_z80_m1(status));
    max7221_display_hex4(0, sample.addr_hi >> 4, is_z80_rfsh(status));
  }
  step_control_scan();
}

int main() {
  setup();
  while (1) loop();
}
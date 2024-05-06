#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "max7221.h"

#define SR_DDR DDRB
#define SR_PORT PORTB
#define SR_LD _BV(PB1)

#define ST_DDR DDRC
#define ST_PIN PINC
#define ST_PORT PORTC

#define ST_RD _BV(PC0)
#define ST_WR _BV(PC1)
#define ST_MREQ _BV(PC2)
#define ST_IORQ _BV(PC3)
#define ST_M1 _BV(PC4)
#define ST_RFSH _BV(PC5)
#define ST_MASK (ST_RD | ST_WR | ST_MREQ | ST_IORQ | ST_M1 | ST_RFSH)

typedef struct {
    uint8_t data;
    uint8_t addr_lo;
    uint8_t addr_hi;
} BusSample;

void sr_init() {
    SR_DDR |= SR_LD;
    SR_PORT |= SR_LD;
}

BusSample sr_read() {
    SR_PORT &= ~SR_LD;
    _delay_us(1);
    SR_PORT |= SR_LD;
    BusSample sample;
    sample.data = spi_transfer(0);
    sample.addr_lo = spi_transfer(0);
    sample.addr_hi = spi_transfer(0);
    return sample;
}

void st_init() {
    ST_DDR = ~ST_MASK;
//    ST_PORT |= ST_MASK;
}

uint8_t st_read() {
    return ~ST_PIN & ST_MASK;
}

int main() {
    st_init();
    sr_init();
    spi_begin();
    max7221_init();
    max7221_config();
    max7221_spin_pattern();
    max7221_blank_display();

    while (1) {
        BusSample sample = sr_read();
        uint8_t status = st_read();
        if (status & ST_RFSH) continue;
        max7221_display_hex4(7, sample.data, status & ST_RD);
        max7221_display_hex4(6, sample.data >> 4, status & ST_WR);
        max7221_blank_digit(4);
        max7221_blank_digit(5);
        max7221_display_hex4(3, sample.addr_lo, status & ST_MREQ);
        max7221_display_hex4(2, sample.addr_lo >> 4, status & ST_IORQ);
        max7221_display_hex4(1, sample.addr_hi, status & ST_M1);
        max7221_display_hex4(0, sample.addr_hi >> 4, status & ST_RFSH);
    }
}
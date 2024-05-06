#include <avr/io.h>

#include "spi.h"

#define SPI_DDR DDRB
#define SPI_MOSI _BV(PB3)
#define SPI_SCLK _BV(PB5)

void spi_begin() {
    // Set MOSI and SCLK as outputs
    SPI_DDR |= SPI_MOSI | SPI_SCLK;

    // Enable SPI master with default clock divisor
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

void spi_end() {
    SPCR = ~_BV(SPE);
}

uint8_t spi_transfer(uint8_t value) {
    SPDR = value;
    while (!(SPSR & _BV(SPIF)))
        ; // wait for transfer to complete
    return SPDR;
}


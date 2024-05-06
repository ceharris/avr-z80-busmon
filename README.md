avr-z80-busmon
==============

A Z80 bus monitor and display based on an AVR microcontroller. 
Software and hardware design is open source, and uses the MIT license.

When building Z80 retro systems, it is often desirable to single step
the processor while observing the state of the data bus, address bus,
and CPU status and control signals.

This project uses an AVR microcontroller (ATmega168), along with a 
24-bit parallel shift register (based on the 74HC165), and a MAX 7221
serially interfaced LED display driver to continuously monitor the Z80
bus, displaying the values on the address bus and data bus in
hexadecimal and the state of the CPU status signals (e.g. RD, WR, MREQ, 
etc). 

The shift register and CPU status signals are buffered and include an
optional pulldown resistor packs to prevent floating inputs during slow
stepping of the Z80 process.

The monitor program observes the state of the RFSH signal to reduce
confusion by inhibiting update of the display during the Z80 memory
refresh cycle.

Planned additions include integration of a push-button instruction step
function, allowing single step for each Z80 instruction. This feature will
use the M1 signal to detect the start of a new instruction, and the WAIT
signal to hold the Z80 in timing state T2 until the user presses a button
to allow the CPU to continue to the next instruction.


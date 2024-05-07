#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "step_control.h"

#define BTN_DDR DDRD
#define BTN_PIN PIND
#define BTN_PCMSK PCMSK2
#define BTN_PCINT (_BV(PCINT23) | _BV(PCINT22))
#define BTN_PCIE _BV(PCIE2)
#define BTN_TOGGLE _BV(PD7)
#define BTN_STEP _BV(PD6)

#define STEP_DDR DDRD
#define STEP_PORT PORTD
#define STEP_RUN _BV(PD5)
#define STEP_NEXT _BV(PD4)

#define OVF_COUNT 2

typedef struct {
    uint8_t ovf_count;
    uint8_t bits;
    uint8_t current;
    uint8_t previous;
} ButtonState;

typedef struct {
    uint8_t bits;
    uint8_t mask;
} Buttons;

static ButtonState button_state;

ISR(TIMER0_OVF_vect) {
  button_state.ovf_count++;
  if (button_state.ovf_count == OVF_COUNT) {
    button_state.previous = button_state.current;
    button_state.current = button_state.bits;
    button_state.ovf_count = 0;
  }
}

ISR(PCINT2_vect) {
  TCNT0 = 0;
  button_state.ovf_count = 0;
  button_state.bits = BTN_PIN;
}

void step_control_init() {
  STEP_DDR |= STEP_NEXT | STEP_RUN;
  STEP_PORT |= STEP_NEXT | STEP_RUN;
  BTN_DDR &= ~(BTN_TOGGLE | BTN_STEP);
  BTN_PCMSK |= BTN_PCINT;
  PCICR |= BTN_PCIE;
  TCCR0B |= _BV(CS02) | _BV(CS00);
  TIMSK0 |= _BV(TOIE0);
  button_state.ovf_count = 0;
  button_state.bits = BTN_PIN;
  button_state.current = button_state.bits;
  button_state.previous = button_state.previous;
}

static void read_buttons(Buttons* state) {
  cli();
  ButtonState copy = button_state;
  button_state.previous = button_state.current;
  sei();
  state->mask = copy.current ^ copy.previous;
  state->bits = copy.current;
}

static void release_wait() {
  STEP_PORT &= ~STEP_NEXT;
  STEP_PORT |= STEP_NEXT;
}

void step_control_scan() {
  Buttons buttons;
  read_buttons(&buttons);
  if ((buttons.mask & BTN_STEP) != 0) {
    if ((buttons.bits & BTN_STEP) == 0) {
      release_wait();
    }
  }
  if ((buttons.mask & BTN_TOGGLE) != 0) {
    if ((buttons.bits & BTN_TOGGLE) == 0) {
      STEP_PORT ^= STEP_RUN;
      if ((STEP_PORT & STEP_RUN) == 0) {
        release_wait();
      }
    }
  }
}

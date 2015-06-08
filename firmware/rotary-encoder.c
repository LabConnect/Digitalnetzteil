/************************************************************************/
/*                                                                      */
/*                      Reading rotary encoder                          */
/*                      one, two and four step encoders supported       */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

// target: ATmega16
//------------------------------------------------------------------------

volatile int8_t enc_delta; // -128 ... 127
static int8_t last;

bool button(void) {
  static uint8_t zustand;
  bool rw = false;

  if (zustand == 0 && !(TASTERPORT & (1 << TASTERBIT))) {
    zustand = 1;
    rw = true;
  } else if (zustand == 1 && !(TASTERPORT & (1 << TASTERBIT))) {
    zustand = 2;
    rw = false;
  } else if (zustand == 2 && (TASTERPORT & (1 << TASTERBIT))) {
    zustand = 3;
    rw = false;
  } else if (zustand == 3 && (TASTERPORT & (1 << TASTERBIT))) {
    zustand = 0;
    rw = false;
  }
  return rw;
}

void encode_init(void) {
  PORTB |= 0x0f;
  int8_t new;

  new = 0;
  if (PHASE_A)
    new = 3;
  if (PHASE_B)
    new ^= 1; // convert gray to binary
  last = new; // power on state
  enc_delta = 0;
  TCCR0 = 1 << WGM01 ^ 1 << CS01 ^ 1 << CS00; // CTC, XTAL / 64
  OCR0 = (uint8_t)(XTAL / 64.0 * 1e-3 - 0.5); // 1ms
  TIMSK |= 1 << OCIE0;
}

ISR(TIMER0_COMP_vect) // 1ms for manual movement
{
  int8_t new, diff;

  new = 0;
  if (PHASE_A)
    new = 3;
  if (PHASE_B)
    new ^= 1;                    // convert gray to binary
  diff = last - new;             // difference last - new
  if (diff & 1) {                // bit 0 = value (1)
    last = new;                  // store new as next last
    enc_delta += (diff & 2) - 1; // bit 1 = direction (+/-)
  }
}

int8_t encode_read(void) // read single step encoders
{
  int8_t val;

  cli();
  val = enc_delta;
  enc_delta = 0;
  sei();
  return val; // counts since last call
}

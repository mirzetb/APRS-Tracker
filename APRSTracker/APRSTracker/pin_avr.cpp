 // trackuino copyright (C) 2010  EA5HAV Javi, APRSTracker.ino l.7

#ifdef AVR

#include "pin.h"
#include <stdint.h>
#include <pins_arduino.h>
#include <Arduino.h>

// This is a digitalWrite() replacement that does not disrupt
// timer 2.
void pin_write(uint8_t pin, uint8_t val)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *out;

  if (port == NOT_A_PIN) return;

  out = portOutputRegister(port);

  if (val == LOW) {
    uint8_t oldSREG = SREG;
    cli();
    *out &= ~bit;
    SREG = oldSREG;
  } else {
    uint8_t oldSREG = SREG;
    cli();
    *out |= bit;
    SREG = oldSREG;
  }
}

#endif  // AVR

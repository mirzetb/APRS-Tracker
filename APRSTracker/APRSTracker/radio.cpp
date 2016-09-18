#include "radio.h"
#include "config.h"
#include "pin.h"
#include <Arduino.h>

void Radio::setup()
{
  // Configure pins
  pinMode(PTT_PIN, OUTPUT);
  pin_write(PTT_PIN, HIGH);
  pinMode(AUDIO_PIN, OUTPUT);
}

void Radio::ptt_on()
{
  pin_write(PTT_PIN, LOW);
  delay(50); // bilo 25   
}

void Radio::ptt_off()
{
  pin_write(PTT_PIN, HIGH);
}

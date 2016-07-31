 // trackuino copyright (C) 2010  EA5HAV Javi, APRSTracker.ino l.7

#include "radio.h"
#include "config.h"
#include "pin.h"
#include <Arduino.h>

void Radio::setup()
{
  // Configure pins
  pinMode(PTT_PIN, OUTPUT);
  pin_write(PTT_PIN, LOW);
  pinMode(AUDIO_PIN, OUTPUT);
}

void Radio::ptt_on()
{
  pin_write(PTT_PIN, HIGH);
  delay(25);   // The HX1 takes 5 ms from PTT to full RF, give it 25
}

void Radio::ptt_off()
{
  pin_write(PTT_PIN, LOW);
}

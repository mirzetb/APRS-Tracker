/*
 * APRS Tracker [2016]
 * Mirzet Brkić E71TMB
 * Faculty of Electrical Engineering University of Sarajevo
 */

/* trackuino copyright (C) 2010  EA5HAV Javi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

// APRSTracker custom libs
#include "config.h"
#include "afsk_avr.h"
#include "aprs.h"
#include "gps.h"
#include "pin.h"
#include "power.h"
#include "sensors_avr.h"


// Arduino/AVR libs
#include <Arduino.h>

// Module constants
static const uint32_t VALID_POS_TIMEOUT = 2000;  // ms

// Module variables
static int32_t next_aprs = 0;


void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pin_write(LED_PIN, LOW);
  pinMode(PD, OUTPUT);
  pin_write(PD, HIGH);
  
  Serial.begin(GPS_BAUDRATE);
#ifdef DEBUG_RESET
  Serial.println("RESET");
#endif

  afsk_setup();
  gps_setup();
  sensors_setup();

#ifdef DEBUG_SENS
  Serial.print("V=");
  Serial.print(read_battery_voltage());
  Serial.print(", T=");
  Serial.print(read_temperature_dht22());
  Serial.print(", H=");
  Serial.println(read_humidity_dht22());
  Serial.print(", P=");
  Serial.println(read_pressure_bmp180());
#endif

  // Do not start until we get a valid time reference
  // for slotted transmissions.
  if (APRS_SLOT >= 0) {
    do {
      while (! Serial.available())
        power_save();
    } while (! gps_decode(Serial.read()));

    next_aprs = millis() + 1000 *
      (APRS_PERIOD - (gps_seconds + APRS_PERIOD - APRS_SLOT) % APRS_PERIOD);
  }
  else {
    next_aprs = millis();
  }
}

void get_pos()
{
  // Get a valid position from the GPS
  int valid_pos = 0;
  uint32_t timeout = millis();
  do {
    if (Serial.available())
      valid_pos = gps_decode(Serial.read());
  } while ( (millis() - timeout < VALID_POS_TIMEOUT) && ! valid_pos) ;
}

void loop()
{
  // Time for another APRS frame
  if ((int32_t) (millis() - next_aprs) >= 0) {
    get_pos();
    aprs_send();
    next_aprs += APRS_PERIOD * 1000L;
    while (afsk_flush()) {
      power_save();
    }

#ifdef DEBUG_MODEM
    // Show modem ISR stats from the previous transmission
    afsk_debug();
#endif
  }

  power_save(); // Incoming GPS data or interrupts will wake us up
}

 // trackuino copyright (C) 2010  EA5HAV Javi, APRSTracker.ino l.7

#include "config.h"
#include "ax25.h"
#include "gps.h"
#include "aprs.h"
#include "sensors_avr.h"
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

// Module functions
float meters_to_feet(float m)
{
  // 10000 ft = 3048 m
  return m / 0.3048;
}

// Exported functions
void aprs_send()
{
  char temp[12];                   // Temperature (int/ext)
  const struct s_address addresses[] = {
    {D_CALLSIGN, D_CALLSIGN_ID},  // Destination callsign
    {S_CALLSIGN, S_CALLSIGN_ID},  // Source callsign (-11 = balloon, -9 = car)
#ifdef DIGI_PATH1
    {DIGI_PATH1, DIGI_PATH1_TTL}, // Digi1 (first digi in the chain)
#endif
#ifdef DIGI_PATH2
    {DIGI_PATH2, DIGI_PATH2_TTL}, // Digi2 (second digi in the chain)
#endif
  };

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_byte('/');                // Report w/ timestamp, no APRS messaging. $ = NMEA raw data

  // ax25_send_string("021709z");     // 021709z = 2nd day of the month, 17:09 zulu (UTC/GMT)
  ax25_send_string(gps_time);         // 170915 = 17h:09m:15s zulu (not allowed in Status Reports)
  ax25_send_byte('h');
  ax25_send_string(gps_aprs_lat);     // Lat: 38deg and 22.20 min (.20 are NOT seconds, but 1/100th of minutes)
  ax25_send_byte('/');                // Symbol table
  ax25_send_string(gps_aprs_lon);     // Lon: 000deg and 25.80 min
  ax25_send_byte('F');                // Symbol: F=tractor, -=QTH
  snprintf(temp, 4, "%03d", (int)(gps_course + 0.5));
  ax25_send_string(temp);             // Course (degrees)
  ax25_send_byte('/');                // and
  snprintf(temp, 4, "%03d", (int)(gps_speed + 0.5));
  ax25_send_string(temp);             // speed (knots)
  ax25_send_string("/A=");            // Altitude (feet). Goes anywhere in the comment area
  snprintf(temp, 7, "%06ld", (long)(meters_to_feet(gps_altitude) + 0.5));
  ax25_send_string(temp);

  ax25_send_string("/V=");
  dtostrf(read_battery_voltage(), 4, 2, temp);
  ax25_send_string(temp);
  
  ax25_send_string("/T=");
  dtostrf(read_temperature_dht22(), 6, 2, temp);
  ax25_send_string(temp);

  ax25_send_string("/H=");
  dtostrf(read_humidity_dht22(), 6, 2, temp);
  ax25_send_string(temp);
/*
  ax25_send_string("/P=");
  dtostrf(read_pressure_bmp180(), 7, 2, temp);
  ax25_send_string(temp);
  */

  ax25_send_byte(' ');
  ax25_send_string(APRS_COMMENT);     // Comment
  ax25_send_footer();

  ax25_flush_frame();                 // Tell the modem to go
}

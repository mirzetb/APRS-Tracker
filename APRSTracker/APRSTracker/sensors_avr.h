#ifdef AVR
#ifndef __SENSORS_AVR_H__
#define __SENSORS_AVR_H__

void sensors_setup();
double read_temperature_dht22();
double read_humidity_dht22();
double read_pressure_bmp180();
double read_battery_voltage();

#endif // ifndef __SENSORS_AVR_H__
#endif // ifdef AVR

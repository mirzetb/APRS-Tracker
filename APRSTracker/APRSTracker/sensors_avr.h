 // trackuino copyright (C) 2010  EA5HAV Javi, APRSTracker.ino l.7

#ifdef AVR
#ifndef __SENSORS_AVR_H__
#define __SENSORS_AVR_H__

void sensors_setup();
unsigned long sensors_aref();
long sensors_internal_temp();
int sensors_int_lm60();
int sensors_ext_lm60();
int sensors_vin();

#endif // ifndef __SENSORS_AVR_H__
#endif // ifdef AVR

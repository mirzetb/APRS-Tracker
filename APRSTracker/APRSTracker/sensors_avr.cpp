#ifdef AVR

#include "config.h"
#include "pin.h"
#include "sensors_avr.h"
#include <Arduino.h>
#include <Wire.h>

// External libraries
#include <dht.h>
#include <SFE_BMP180.h>

dht DHT;
SFE_BMP180 pressure;

void sensors_setup()
{
  pressure.begin();  
}

double read_temperature_dht22()
{
  DHT.read22(DHT22_PIN); 
  return DHT.temperature;
}

double read_humidity_dht22()
{
  DHT.read22(DHT22_PIN); 
  return DHT.humidity;
}

double read_pressure_bmp180() 
{
  char status;
  double T, P;

  status = pressure.startTemperature();
  if(status == 0) return 0;

  delay(status);
  status = pressure.getTemperature(T);
  if(status == 0) return 0;

  status = pressure.startPressure(3);
  if(status == 0) return 0;
  delay(status);
  
  status = pressure.getPressure(P,T);
  if(status == 0) return 0;

  return P;
}

double read_battery_voltage() 
{
  // analogRead [0 - 1023]
  int analog = analogRead(V_PIN);
 
  //if (battery_voltage < 3.3 V)
  if (analog > 164) 
    pin_write(LED_PIN, HIGH);
  else
    pin_write(LED_PIN, LOW);

  double voltage = V_DIODE / analog;

  return voltage;   
}

#endif // ifdef AVR

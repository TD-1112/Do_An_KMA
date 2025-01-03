#ifndef __SENSOR__
#define __SENSOR__


#include <Arduino.h>

#define SENSOR_PIN 4

extern int lastStatus;

void sensor_init();
int sensor_read();

#endif 
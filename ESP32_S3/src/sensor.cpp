#include "sensor.h"

void sensor_init() 
{
    pinMode(SENSOR_PIN, INPUT);
}

int sensor_read() 
{
    return digitalRead(SENSOR_PIN);
}

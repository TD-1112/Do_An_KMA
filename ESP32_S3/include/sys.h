#ifndef __SYS_H__
#define __SYS_H__

#include <Arduino.h>
#include "sim900a.h"
#include "lcd.h"
#include "mqtt.h"
#include "servo.h"
#include "sensor.h"

// System States
enum SystemState {
    IDLE,
    MOTION_DETECTED,
    WAITING_RESPONSE,
    OPENING_DOOR,
    CLOSING_DOOR
};

#define PUBLISH_INTERVAL 1000 // 1 second interval
#define WAIT_TIMEOUT 5000    // 5 second timeout for waiting response

extern SystemState currentState;
extern bool canDetect;
extern bool check;

void system_init();
void system_loop();


#endif
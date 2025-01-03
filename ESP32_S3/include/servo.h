#ifndef __SERVO__
#define __SERVO__


#include <Arduino.h>
#include <ESP32Servo.h>


#define SERVO_PIN 20
#define SERVO_OPEN_ANGLE 90
#define SERVO_CLOSE_ANGLE 0  
#define SERVO_DELAY 5000     // 5 seconds for servo wait


extern bool servoAt90;
extern unsigned long lastServoMoveTime;
extern Servo servo;

void controlServo(int angle, bool* servoStatus);
void servo_init();


#endif

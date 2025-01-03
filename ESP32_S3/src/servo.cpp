#include "servo.h"

Servo servo;



void servo_init() 
{
    servo.attach(SERVO_PIN);
    controlServo(SERVO_CLOSE_ANGLE, &servoAt90);
}


void controlServo(int angle, bool* servoStatus) 
{
    servo.write(angle);
    *servoStatus = (angle == SERVO_OPEN_ANGLE);
}


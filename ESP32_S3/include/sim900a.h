#ifndef __SIM900A_H__
#define __SIM900A_H__


#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>

// Alert settings
#define ALERT_PHONE  "0395172459"
#define ALERT_MESSAGE  "canh bao co nguoi xam nhap"
#define SMS_TIMEOUT 10000    // 10 second timeout for SMS

extern bool check_sms;
extern bool smsSending;
extern unsigned long smsStartTime;

void sim_init();
void sendSms(String phoneNumber, String message);


#endif
#ifndef __LCD_H__
#define __LCD_H__

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

#define DISPLAY_DELAY 50     // 50ms delay after clearing display

void lcd_display(String message);
void displayStatus(const char* status);

#endif

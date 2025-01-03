#include "lcd.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_display(String message) 
{
    lcd.clear();
    delay(50); // Short delay after clearing
    lcd.setCursor(0, 0);
    lcd.print("Status: ");

    if (message == "Access granted" || message == "ok") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Door Opening");
    }
    else if (message == "Access denied" || message == "denied") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Not Allowed");
    }
    else if (message == "Door closed" || message == "closed") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Door Closed");
    }
    else if (message == "Waiting...") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Wait Response");
    }
    else if (message == "System Ready" || message == "Ready") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Ready to Detect");
    }
    else if (message == "Timeout-Resuming") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Timeout-Resume");
    }
    else if (message == "Door auto-closed") 
    {
        lcd.setCursor(0, 1);
        lcd.print("Auto Closing");
    }
    else 
    {
        lcd.setCursor(0, 1);
        lcd.print(message);
    }
}


void displayStatus(const char* status) 
{
    lcd.clear();
    delay(DISPLAY_DELAY);
    lcd.setCursor(0, 0);
    lcd_display(status);
    Serial.println(status);
}
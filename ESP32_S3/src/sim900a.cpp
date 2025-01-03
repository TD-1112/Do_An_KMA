#include "sim900a.h" 


void sim_init() 
{
    Serial1.begin(9600, SERIAL_8N1, 17, 18);
    delay(5000);
    Serial1.println("AT+CREG?");
    delay(1000);
    if (Serial1.available()) 
    {
        String response = Serial1.readString();
        Serial.print("Network Registration Response: ");
        Serial.println(response);
    }

    Serial1.println("AT+CSQ");
    
    delay(1000);
    if (Serial1.available()) 
    {
        String response = Serial1.readString();
        Serial.print("Signal Quality Response: ");
        Serial.println(response);
    }
}

void sendSms(String phoneNumber, String message) 
{
    Serial1.println("AT+CMGF=1");
    delay(1000);
    Serial1.print("AT+CMGS=\"");   
    Serial1.print(phoneNumber);
    Serial1.println("\"");         
    delay(1000);
    Serial1.print(message);
    delay(500);
    Serial1.write(26);
    delay(2000);
}

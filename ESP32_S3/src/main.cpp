#include "sys.h"

// Mode selection
#define DEBUG 0
#define RELEASE 1
#define MODE RELEASE  // Change to RELEASE for production

#if MODE == DEBUG
void setup() 
{
    Serial.begin(115200);
    pinMode(4, INPUT);
    Serial.println("Debug mode started");
}

void loop() 
{
    int sensorStatus = digitalRead(4);
    Serial.println(sensorStatus);
    delay(1000); // Add delay to prevent flooding Serial
}

#elif MODE == RELEASE
void setup() 
{
    system_init();
    Serial.println("Release mode started");
}

void loop()
{
    system_loop();
}

#else
#error "Invalid MODE selected"
#endif
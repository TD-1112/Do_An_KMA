#include "sys.h"

SystemState currentState = IDLE;

unsigned long lastPublishTime = 0;
unsigned long lastServoMoveTime = 0;
unsigned long lastMessageTime = 0;
unsigned long smsStartTime = 0;

bool servoAt90 = false;
bool check = false;
bool check_sms = false;
bool smsSending = false;
int lastStatus = LOW;
bool waitingForResponse = false;
bool canDetect = true;


void system_init() {
    Serial.begin(115200);
    Serial.println("System initializing...");

    sim_init();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    WiFi_Connect();
    MQTT_Connect();
    
    sensor_init();
    servo_init();
    
    displayStatus("System Ready");
    delay(3000);
    lcd.clear();
    delay(DISPLAY_DELAY);
}

void system_loop() {
    unsigned long currentTime = millis();
    
    // Check MQTT connection
    if (!client.connected()) {
        MQTT_Reconnect();
    }
    client.loop();

    // Handle SMS timeout
    if (smsSending && (currentTime - smsStartTime >= SMS_TIMEOUT)) {
        smsSending = false;
        check_sms = false;
        displayStatus("Ready");
        currentState = IDLE;
        canDetect = true;
        Serial.println("SMS timeout - system reset");
    }

    // State machine
    switch (currentState) {
        case IDLE:
            if (canDetect && (currentTime - lastPublishTime >= PUBLISH_INTERVAL)) {
                lastPublishTime = currentTime;
                int sensorStatus = sensor_read();
                
                if (lastStatus == LOW && sensorStatus == HIGH) {
                    MQTT_Publish(TOPIC_PUB, "1");
                    Serial.println("Motion detected - waiting for response!");
                    displayStatus("Waiting...");
                    waitingForResponse = true;
                    lastMessageTime = currentTime;
                    currentState = WAITING_RESPONSE;
                    canDetect = false;
                }
                lastStatus = sensorStatus;
            }
            break;

        case WAITING_RESPONSE:
            // Check for timeout first
            if (currentTime - lastMessageTime >= WAIT_TIMEOUT) {
                Serial.println("Response wait timeout - resuming sensor checks");
                displayStatus("Timeout-Resuming");
                // Reset all flags
                waitingForResponse = false;
                NewMessage = false;
                message = "";
                currentState = IDLE;
                canDetect = true;
                lastPublishTime = currentTime; // Enable immediate sensor check
                break;
            }

            // Handle incoming messages
            if (NewMessage) {
                if (message == "ok") {
                    check = true;
                    displayStatus("Access granted");
                    
                    if (!servoAt90) {
                        controlServo(SERVO_OPEN_ANGLE, &servoAt90);
                        lastServoMoveTime = currentTime;
                        currentState = OPENING_DOOR;
                        check = false;
                    }
                } 
                else if (message == "no" && !check) {
                    displayStatus("Access denied");
                    if (!check_sms && !smsSending) {
                        check_sms = true;
                        smsSending = true;
                        smsStartTime = currentTime;
                        sendSms(ALERT_PHONE, ALERT_MESSAGE);
                        Serial.println("SMS sending...");
                    }
                    currentState = IDLE;
                    canDetect = true;
                    lastPublishTime = currentTime;
                }
                else if (message == "no_detect") {
                    canDetect = true;
                    currentState = IDLE;
                    displayStatus("Ready");
                    lastPublishTime = currentTime;
                }
                waitingForResponse = false;
                NewMessage = false;
            }
            break;

        case OPENING_DOOR:
            if (currentTime - lastServoMoveTime >= SERVO_DELAY) {
                controlServo(SERVO_CLOSE_ANGLE, &servoAt90);
                displayStatus("Door closed");
                currentState = IDLE;
                canDetect = true;
                lastPublishTime = currentTime;
                check = false;
                NewMessage = false;
                waitingForResponse = false;
            }
            break;
    }

    // Auto-close safety check
    if (servoAt90 && (currentTime - lastServoMoveTime >= SERVO_DELAY) && 
        currentState != OPENING_DOOR) {
        controlServo(SERVO_CLOSE_ANGLE, &servoAt90);
        displayStatus("Door auto-closed");
        currentState = IDLE;
        canDetect = true;
        lastPublishTime = currentTime;
        check = false;
        NewMessage = false;
        waitingForResponse = false;
    }

    delay(5);
}

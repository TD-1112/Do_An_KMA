#include "mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);

String message = "";
bool NewMessage = false;

void WiFi_Connect() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {   
        delay(1000);
        Serial.println("Connecting to WiFi...");
        lcd.setCursor(0, 0);
        lcd.print("Connecting...");
    }
    lcd.clear();
    Serial.println("Connected to WiFi");
    lcd.setCursor(0, 0);
    lcd.print("Connected: ");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(3000);
    lcd.clear();
}

void MQTT_Callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    NewMessage = true;
    Serial.println("Message: " + message);
}

void MQTT_Connect() {
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(MQTT_Callback);
    while (!client.connected()) {
        String client_id = "esp32-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT broker with client ID: %s\n", client_id.c_str());
        if (client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("MQTT connected");
            lcd.setCursor(0, 0);
            lcd.print("MQTT Connected");
            delay(2000);
            lcd.clear();
            client.subscribe(TOPIC_SUB);
        } else {
            Serial.print("MQTT connection failed, state: ");
            Serial.println(client.state());
            lcd.setCursor(0, 0);
            lcd.print("MQTT Failed");
            delay(3000);
            lcd.clear();
        }
    }
}

void MQTT_Reconnect() {
    if (!client.connected()) {
        Serial.println("Attempting MQTT reconnection...");
        if (client.connect("ESP32Client", MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("MQTT reconnected");
            client.subscribe(TOPIC_SUB);
        } else {
            Serial.print("Reconnect failed, state: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void MQTT_Publish(const char* topic, const char* message) {
    client.publish(topic, message);
}

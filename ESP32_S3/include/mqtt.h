#ifndef __MQTT_H__
#define __MQTT_H__

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "lcd.h"

#define WIFI_SSID "Thang"
#define WIFI_PASSWORD "11122004"
#define TOPIC_SUB "face_detect"
#define TOPIC_PUB "face_detect"
#define MQTT_PORT 1883
#define MQTT_USERNAME "Thang"
#define MQTT_PASSWORD "1112"
#define MQTT_SERVER "broker.emqx.io"

extern WiFiClient espClient;
extern PubSubClient client;
extern String message;
extern bool NewMessage;
extern unsigned long lastPublishTime;
extern unsigned long lastMessageTime;
extern bool waitingForResponse;

void WiFi_Connect();
void MQTT_Connect();
void MQTT_Reconnect();
void MQTT_Callback(char* topic, byte* payload, unsigned int length);
void MQTT_Publish(const char* topic, const char* message);


#endif

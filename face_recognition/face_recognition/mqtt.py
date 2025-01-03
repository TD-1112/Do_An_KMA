import paho.mqtt.client as mqtt
import time
import warnings 
warnings.filterwarnings("ignore", category=DeprecationWarning)
# MQTT Configuration
MQTT_BROKER = "broker.emqx.io"
MQTT_PORT = 1883
MQTT_USERNAME = "Thang"
MQTT_PASSWORD = "1112"
TOPIC_PUB = "face_de"
TOPIC_SUB = "face_de"

# Global variable to store received message
received_message = None
check_new_message = False

# Callback when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(TOPIC_SUB)
    else:
        print(f"Failed to connect, return code {rc}")

# Callback when a PUBLISH message is received from the server
def on_message(client, userdata, msg):
    global received_message
    received_message = msg.payload.decode()
    print(f"Received message: {received_message}")

# Create MQTT client
#client = mqtt.Client()  # Use MQTT version 5
client = mqtt.Client(protocol=mqtt.MQTTv5)
client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
client.on_connect = on_connect
client.on_message = on_message

# Connect to broker
client.connect(MQTT_BROKER, MQTT_PORT)
client.loop_start()

# Function to publish a message
def publish_message(topic, message):
    print(f"Publishing message '{message}' to topic '{topic}'")  # Add this line to print the message being published
    result = client.publish(topic, message)
    status = result.rc
    if status == mqtt.MQTT_ERR_SUCCESS:
        print(f"Sent '{message}' to topic '{topic}'")
    else:
        print(f"Failed to send message to topic {topic}")
    # time.sleep(1)

# Function to get the received message
def get_received_message():
    global received_message
    while received_message is None:
        # time.sleep(0.1)
        pass
    message = received_message
    received_message = None  # Reset the message after reading
    check_new_message = True
    return message


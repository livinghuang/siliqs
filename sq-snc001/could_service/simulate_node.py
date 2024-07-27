import paho.mqtt.client as mqtt
from paho.mqtt.client import CallbackAPIVersion
import time
import random
import struct
import base64

# MQTT settings
broker_address = '13.210.141.86'  # Change to your broker's address
port = 1883  # Default port for MQTT
topic = 'test_v2/topic'

"""
Generates sensor data with random values for type, id, timestamp, PM2.5, PM10, CO2, temperature, humidity, air pressure, wind speed, and wind direction.

Returns the base64 encoded sensor data.
"""
def generate_sensor_data(id):
    data = struct.pack('<2H1I8H',
                       random.randint(0, 1),        # type
                       id,  # id
                       int(time.time()),            # timestamp
                       random.randint(0, 15),      # PM2.5
                       random.randint(0, 50),      # PM10
                       random.randint(400, 450),   # CO2, adjusted max value
                       random.randint(20, 30),      # temperature, adjusted range
                       random.randint(60, 80),     # humidity
                       random.randint(900, 1000),   # air_pressure
                       random.randint(0, 10),      # wind_speed
                       random.randint(0, 35)       # wind_direction, max 359
                      )
    return base64.b64encode(data)

def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n" % rc)

# Set up the MQTT client
mqtt_client = mqtt.Client(callback_api_version=CallbackAPIVersion.VERSION2)  # Use latest callback API version
mqtt_client.on_connect = on_connect
mqtt_client.username_pw_set("siliq", "siliq1234")  # Set MQTT username and password

mqtt_client.on_connect = on_connect

mqtt_client.connect(broker_address, port)

mqtt_client.loop_start()

while True:
    for sensor_id in range(11):  # Loop over sensor ids from 0 to 10
        sensor_id=10
        data = generate_sensor_data(sensor_id)
        print(f"Publishing: {data}")
        mqtt_client.publish(topic, data)
    time.sleep(300)  # Adjust remaining time to complete the 5 seconds cycle
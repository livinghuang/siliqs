import paho.mqtt.client as mqtt
from paho.mqtt.client import CallbackAPIVersion
import time
import random
import struct
import base64
from formats import main_struct_format, louver_format, wind_format, gnss_format

# MQTT settings
broker_address = '13.210.141.86'  # Change to your broker's address
port = 1883  # Default port for MQTT
topic = 'test_v2/topic'


def generate_sensor_data(stationId):
    """
    生成模拟传感器数据并编码为 Base64 字符串
    """
    station_type = 1  # 假设传感器类型为 1
    station_id = 10
    timestamp = int(time.time())
    station_mac = bytes([0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f])
    battery_voltage = random.uniform(3.0, 4.2)
    station_reserved = bytes([0] * 8)

    pm2p5 = random.randint(0, 100)
    pm10 = random.randint(0, 100)
    co2 = random.randint(400, 500)
    humidity = random.randint(30, 70)
    hdc1080_temperature = random.uniform(20.0, 30.0)
    dsp310_temperature = random.uniform(20.0, 30.0)
    air_pressure = random.uniform(950.0, 1050.0)

    louver_mac = bytes([0x10, 0x11, 0x12, 0x13, 0x14, 0x15])
    louver_reserved = bytes([0] * 10)

    wind_speed = random.randint(0, 15)
    wind_direction = random.randint(0, 360)
    latitude = random.uniform(-90.0, 90.0)
    longitude = random.uniform(-180.0, 180.0)
    altitude = random.uniform(0.0, 1000.0)
    gnss_reserved = bytes([0] * 4)

    # 按照定义的 struct 格式打包数据
    louver_data = struct.pack(louver_format, louver_mac,louver_reserved, pm2p5, pm10, co2, humidity, hdc1080_temperature, dsp310_temperature, air_pressure)
    wind_data = struct.pack(wind_format, wind_speed, wind_direction)
    gnss_data = struct.pack(gnss_format, latitude, longitude, altitude, gnss_reserved)

    packed_data = struct.pack(
        main_struct_format,
        station_type,
        station_id,
        timestamp,
        station_mac,
        battery_voltage,
        station_reserved,
        louver_data,
        wind_data,
        gnss_data
    )

    encoded_data = base64.b64encode(packed_data).decode()
    return encoded_data

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
        data = generate_sensor_data(sensor_id)
        print(f"Publishing: {data}")
        mqtt_client.publish(topic, data)
        time.sleep(1)
    time.sleep(30)  # Adjust remaining time to complete the 5 seconds cycle
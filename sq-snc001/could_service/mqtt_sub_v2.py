import paho.mqtt.client as mqtt
from paho.mqtt.client import CallbackAPIVersion
from influxdb import InfluxDBClient
import base64
import struct
import time
import random
# import binascii

# from formats import main_struct_format, louver_format, wind_format, gnss_format

# Define the struct format to unpack the entire byte array
main_struct_format = '<2H1I16sf8s36s4s16s'  # Little-endian: 2 unsigned 16-bit ints, 1 unsigned 32-bit int, 8-byte string, float, 40-byte string, 4-byte string, 16-byte string

louver_format = '<6s10s4H3f'
wind_format = '<2H'
gnss_format = '<3f4s'

# MQTT Settings
MQTT_BROKER = '13.210.141.86'
MQTT_PORT = 1883
MQTT_TOPIC = 'test_v2/topic'

# # InfluxDB Settings
# INFLUXDB_HOST = 'localhost'
# INFLUXDB_PORT = 8086
# INFLUXDB_USERNAME = 'root'
# INFLUXDB_PASSWORD = 'root'
# INFLUXDB_DATABASE = 'sinica_test_v2'

# # Connect to InfluxDB
# influxdb_client = InfluxDBClient(
#     host=INFLUXDB_HOST,
#     port=INFLUXDB_PORT,
#     username=INFLUXDB_USERNAME,
#     password=INFLUXDB_PASSWORD,
#     database=INFLUXDB_DATABASE
# )

def format_mac_address_as_acsii(mac_bytes):
    ascii_string = ''.join([chr(byte) for byte in mac_bytes])
    print(ascii_string)
    return ascii_string


def decode_and_parse_sensor_data(encoded_data):
    """
    Decodes a Base64 encoded string, checks the type and id from the initial bytes,
    and unpacks the remaining data according to the predefined struct format for sensor data.

    :param encoded_data: str - Base64 encoded string containing binary sensor data
    :return: dict or str - Dictionary containing parsed sensor data or an error message
    """
    # Decode the Base64 string to bytes
    decoded_bytes = base64.b64decode(encoded_data)
    # print("decoded_bytes:", decoded_bytes)

    # Check if we have enough data to unpack
    if len(decoded_bytes) >= struct.calcsize(main_struct_format):
        unpacked_data = struct.unpack(main_struct_format, decoded_bytes)
        # Unpack louver_data
        louver_data = struct.unpack(louver_format, unpacked_data[6])
        # Unpack wind_data
        wind_data = struct.unpack(wind_format, unpacked_data[7])
        # Unpack gnss_data
        gnss_data = struct.unpack(gnss_format, unpacked_data[8])
        # Create a dictionary with the data, mapping each field to a meaningful name
        data_dict = {
            "type": unpacked_data[0],
            "station_id": unpacked_data[1],
            "timestamp": unpacked_data[2],
            "station_mac": format_mac_address_as_acsii(unpacked_data[3]),#.upper(),
            "battery_voltage": unpacked_data[4],
            "station_reserved": format_mac_address_as_acsii(unpacked_data[5]),
            "louver_data": {
                "louver_mac": louver_data[0].hex(),
                "reserved": louver_data[1].hex(),
                "pm2p5": louver_data[2],
                "pm10": louver_data[3],
                "co2": louver_data[4],
                "humidity": louver_data[5],
                "hdc1080_temperature": louver_data[6],
                "dsp310_temperature": louver_data[7],
                "air_pressure": louver_data[8],
                "wind_speed": wind_data[0],
                "wind_direction": wind_data[1]
            },
            "wind_data": {
                "wind_speed": wind_data[0],
                "wind_direction": wind_data[1]
            },
            "gnss_data": {
                "latitude": gnss_data[0],
                "longitude": gnss_data[1],
                "altitude": gnss_data[2],
                "reserved": gnss_data[3].hex()
            }
        }

        print(data_dict)
        return data_dict
    else:
        error_msg = "Error: Data length mismatch. Expected at least {} bytes, got {} bytes.".format(
            struct.calcsize(main_struct_format), len(decoded_bytes))
        print(error_msg)
        return {"error": error_msg}

# def insert_data_into_influxdb(data):
#     if "error" not in data:
#         # Convert timestamp from seconds to nanoseconds
#         timestamp_ns = int(data['timestamp']) * 1000000000

#         # Initialize an empty list to hold the JSON data for InfluxDB
#         json_body = []

#         # Dictionary mapping sensor data fields to their respective measurements and conversion factors (if applicable)
#         measurements = {
#             "battery_voltage": ("battery_voltage", 1),
#             "co2": ("co2", 1),
#             "pm2p5": ("pm2p5", 1),
#             "pm10": ("pm10", 1),
#             "hdc1080_temperature": ("hdc1080_temperature", 1),
#             "dsp310_temperature": ("dsp310_temperature", 1),
#             "humidity": ("humidity", 1),
#             "air_pressure": ("air_pressure", 1),
#             "wind_speed": ("wind_speed", 1),
#             "wind_direction": ("wind_direction", 1),
#             "latitude": ("latitude", 1),
#             "longitude": ("longitude", 1),
#             "altitude": ("altitude", 1)
#         }

#         # Iterate over each sensor type and create a separate entry for each
#         for field, (measurement, conversion_factor) in measurements.items():
#             if field in data['louver_data']:
#                 converted_value = float(data['louver_data'][field]) * conversion_factor
#                 entry = {
#                     "measurement": measurement,
#                     "tags": {
#                         "station_id": str(data['station_id']),
#                         "station_type": str(data['type']),
#                         "louver_mac": str(data['louver_data']['louver_mac']),
#                         "station_mac": str(data['station_mac']),
#                     },
#                     "time": timestamp_ns,
#                     "fields": {
#                         "value": converted_value
#                     }
#                 }
#                 json_body.append(entry)
#             elif field in data['wind_data']:
#                 converted_value = float(data['wind_data'][field]) * conversion_factor
#                 entry = {
#                     "measurement": measurement,
#                     "tags": {
#                         "station_id": str(data['station_id']),
#                         "station_type": str(data['type']),
#                         "station_mac": str(data['station_mac']),
#                     },
#                     "time": timestamp_ns,
#                     "fields": {
#                         "value": converted_value
#                     }
#                 }
#                 json_body.append(entry)
#             elif field in data['gnss_data']:
#                 converted_value = float(data['gnss_data'][field]) * conversion_factor
#                 entry = {
#                     "measurement": measurement,
#                     "tags": {
#                         "station_id": str(data['station_id']),
#                         "station_type": str(data['type']),
#                         "station_mac": str(data['station_mac']),
#                     },
#                     "time": timestamp_ns,
#                     "fields": {
#                         "value": converted_value
#                     }
#                 }
#                 json_body.append(entry)
#             elif field in data:
#                 converted_value = float(data[field]) * conversion_factor
#                 entry = {
#                     "measurement": measurement,
#                     "tags": {
#                         "station_id": str(data['station_id']),
#                         "station_type": str(data['type']),
#                         "station_mac": str(data['station_mac']),
#                     },
#                     "time": timestamp_ns,
#                     "fields": {
#                         "value": converted_value
#                     }
#                 }
#                 json_body.append(entry)

#         # Write all entries to InfluxDB
#         influxdb_client.write_points(json_body)
#         print("Data inserted into InfluxDB:", json_body)
#     else:
#         print("Error inserting data into InfluxDB:", data['error'])
# Define MQTT callback functions
def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print("Connected successfully.")
        client.subscribe(MQTT_TOPIC)
    else:
        print(f"Failed to connect, return code {rc}")
def on_message(client, userdata, message):
    print(f"Message received on topic {message.topic}: {message.payload.decode()}")
    encoded_data = message.payload.decode()
    parsed_data = decode_and_parse_sensor_data(encoded_data)
    print("Parsed Data Dictionary:")
    print(parsed_data)
    # insert_data_into_influxdb(parsed_data)

#Set up MQTT client
mqtt_client = mqtt.Client(callback_api_version=CallbackAPIVersion.VERSION2)  # Use latest callback API version
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.username_pw_set("siliq", "siliq1234")  # Set MQTT username and password

# Connect to MQTT Broker
mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Start the loop
mqtt_client.loop_forever()


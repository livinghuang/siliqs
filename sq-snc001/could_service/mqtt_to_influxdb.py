import paho.mqtt.client as mqtt
from paho.mqtt.client import CallbackAPIVersion
from influxdb import InfluxDBClient
import base64
import struct

# MQTT Settings
MQTT_BROKER = 'localhost'
MQTT_PORT = 1883
MQTT_TOPIC = 'test/topic'

# InfluxDB Settings
INFLUXDB_HOST = 'localhost'
INFLUXDB_PORT = 8086
INFLUXDB_USERNAME = 'root'
INFLUXDB_PASSWORD = 'root'
INFLUXDB_DATABASE = 'sinica_test'

# Connect to InfluxDB
influxdb_client = InfluxDBClient(
    host=INFLUXDB_HOST,
    port=INFLUXDB_PORT,
    username=INFLUXDB_USERNAME,
    password=INFLUXDB_PASSWORD,
    database=INFLUXDB_DATABASE
)

# Define the list of weather station names
weather_station_names = [
    "CY Park WS",
    "Wenhua WS",
    "Chiayi Mus",
    "Hinoki WS",
    "Art Mus WS",
    "Creatv WS",
    "Tower WS",
    "Lantan WS",
    "Mitsuko WS",
    "Basebl WS",
    "Railpk WS"
]


def decode_and_parse_sensor_data(encoded_data):
    """
    Decodes a Base64 encoded string, checks the type and id from the initial bytes,
    and unpacks the remaining data according to the predefined struct format for sensor data.

    :param encoded_data: str - Base64 encoded string containing binary sensor data
    :return: dict or str - Dictionary containing parsed sensor data or an error message
    """
    # Decode the Base64 string to bytes
    decoded_bytes = base64.b64decode(encoded_data)

    # Define the struct format to unpack the entire byte array
    # This format matches the new SensorData struct:
    # 1 uint16 for type, 1 uint16 for id, 1 uint32 for timestamp, and 8 uint16 for various sensor readings
    struct_format = '<2H1I3HfHf2H'  # Little-endian: 2 unsigned 16-bit ints, 1 unsigned 32-bit int, 3 unsigned 16-bit ints, 1 float, 1 unsigned 16-bit int, 1 float, 2 unsigned 16-bit ints

    # Check if we have enough data to unpack
    if len(decoded_bytes) >= struct.calcsize(struct_format):
        unpacked_data = struct.unpack(struct_format, decoded_bytes)
        # Create a dictionary with the data, mapping each field to a meaningful name
        data_dict = {
            "type": unpacked_data[0],
            "id": unpacked_data[1],
            "timestamp": unpacked_data[2],
            "PM2_5": unpacked_data[3],
            "PM10": unpacked_data[4],
            "CO2": unpacked_data[5],
            "temperature": unpacked_data[6],
            "humidity": unpacked_data[7],
            "air_pressure": unpacked_data[8],
            "wind_speed": unpacked_data[9],
            "wind_direction": unpacked_data[10]
        }
        print(data_dict)
        return data_dict
    else:
        error_msg = "Error: Data length mismatch. Expected at least {} bytes, got {} bytes.".format(
            struct.calcsize(struct_format), len(decoded_bytes))
        return {"error": error_msg}

def insert_data_into_influxdb(sensor_data):
    if "error" not in sensor_data:
        # Convert timestamp from seconds to nanoseconds
        timestamp_ns = int(sensor_data['timestamp']) * 1000000000

        # Initialize an empty list to hold the JSON data for InfluxDB
        json_body = []

        # Dictionary mapping sensor data fields to their respective measurements and conversion factors (if applicable)
        measurements = {
            "PM2_5": ("particulate_matter_2_5", 1),  # Assuming no conversion needed
            "PM10": ("particulate_matter_10", 1),   # Assuming no conversion needed
            "CO2": ("carbon_dioxide", 1),           # Assuming no conversion needed
            "temperature": ("temperature", 1),    # Converts tenths of a degree to degrees
            "humidity": ("humidity", 0.1),            # Assuming no conversion needed
            "air_pressure": ("air_pressure", 1),    # Assuming no conversion needed
            "wind_speed": ("wind_speed", 1),        # Assuming no conversion needed
            "wind_direction": ("wind_direction", 1) # Assuming no conversion needed
        }

        # Iterate over each sensor type and create a separate entry for each
        for field, (measurement, conversion_factor) in measurements.items():
            if field in sensor_data:
                converted_value = float(sensor_data[field]) * conversion_factor
                entry = {
                    "measurement": measurement,
                    "tags": {
                        "application": "urban_sensing",
                        "station_name": weather_station_names[sensor_data['id']],
                        "station_location": "Chiayi",
                        "sensor_id": str(sensor_data['id']),
                        "sensor_type": str(sensor_data['type'])
                    },
                    "time": timestamp_ns,
                    "fields": {
                        "value": converted_value
                    }
                }
                json_body.append(entry)

        # Write all entries to InfluxDB
        influxdb_client.write_points(json_body)
        print("Data inserted into InfluxDB:", json_body)
    else:
        print("Error inserting data into InfluxDB:", sensor_data['error'])




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
    insert_data_into_influxdb(parsed_data)


def test_insert_function():
    # Simulated encoded data as received from the sensor, for testing purposes
    # This should be Base64 encoded representation of the struct defined as in your 'SensorData'
    encoded_test_data = base64.b64encode(struct.pack('<2H1I3HfHf2H', 0, 0, 1622559200, 15, 20, 400, 25.3, 65, 101300.2, 5, 180)).decode()
    print(encoded_test_data)
    # parsed_data = decode_and_parse_sensor_data(encoded_test_data)
    # insert_data_into_influxdb(parsed_data)

# Set up MQTT client
# mqtt_client = mqtt.Client(callback_api_version=CallbackAPIVersion.VERSION2)  # Use latest callback API version
# mqtt_client.on_connect = on_connect
# mqtt_client.on_message = on_message
# mqtt_client.username_pw_set("siliq", "siliq1234")  # Set MQTT username and password

# Connect to MQTT Broker
# mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Start the loop
# mqtt_client.loop_forever()

# #Uncomment this line to test independently without MQTT
# test_insert_function()


decode_and_parse_sensor_data("AAAAAF8NUWYDAAsA3ggsG+dBHANUksVHAADhAA==")
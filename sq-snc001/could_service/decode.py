import base64
import struct
import time
import random

from formats import main_struct_format, louver_format, wind_format, gnss_format



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
            "station_mac": unpacked_data[3].hex(),
            "battery_voltage": unpacked_data[4],
            "station_reserved": unpacked_data[5].hex(),
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

decode_and_parse_sensor_data("AQABAGDSpmIAAQIDBAUGBwgJCgsMDQ4PzcxsQAAAAAAAAAAAERITFBUWAAAAAAAAAAAAADIAUADCATwAAADIQQAA0EEAAHpEBQC0AAAA8EEAAPBCAADIQgAAAAA=")
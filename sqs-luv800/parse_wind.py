import struct

# Define the wind data structure
class WindData:
    def __init__(self):
        self.wind_direction = 0
        self.wind_speed = 0

# Define the Modbus response structure for wind data
class ModbusResponseWind:
    def __init__(self, data):
        if len(data) < 9:
            raise ValueError("Data length is too short")
        self.modbus_id = data[0]
        self.function_code = data[1]
        self.byte_count = data[2]
        self.register_data = data[3:7]
        self.crc = struct.unpack('>H', data[7:9])[0]

# Function to parse the Modbus response buffer into the wind data structure
def parse_wind_response(response):
    wind_result = WindData()
    wind_result.wind_direction = struct.unpack('>H', response.register_data[0:2])[0]
    wind_result.wind_speed = struct.unpack('>H', response.register_data[2:4])[0]
    return wind_result

def main():
    # Example response buffer for wind data (fill with your actual response data)
    response_data_wind = bytes([0x01, 0x03, 0x04, 0x00, 0xD3, 0x01, 0x1F, 0x4B, 0x92])

    # Create a ModbusResponseWind object for wind data
    response_wind = ModbusResponseWind(response_data_wind)

    # Print the Modbus response data for wind
    print(f"Modbus ID: {response_wind.modbus_id}")
    print(f"Function code: {response_wind.function_code}")
    print(f"Byte count: {response_wind.byte_count}")
    print(f"CRC: {response_wind.crc}")

    # Parse the wind sensor data
    wind_result = parse_wind_response(response_wind)

    # Print the parsed wind data
    print(f"Wind Direction: {wind_result.wind_direction} deg")
    print(f"Wind Speed: {wind_result.wind_speed/100} m/s")

if __name__ == "__main__":
    main()
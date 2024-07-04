import struct

# Define the sensor data structure
class SensorData:
    def __init__(self):
        self.dsp310_temperature = 0.0
        self.pressure = 0.0
        self.co2 = 0
        self.pm2p5 = 0
        self.pm10 = 0
        self.temperature = 0
        self.humidity = 0

# Define the Modbus response structure
class ModbusResponse:
    def __init__(self, data):
        if len(data) < 34:
            raise ValueError("Data length is too short")
        
        self.modbus_id = data[0]
        self.function_code = data[1]
        self.byte_count = data[2]
        self.register_data = data[3:3+28]
        self.version = data[31]
        self.crc = struct.unpack('<H', data[32:34])[0]

# Function to parse the Modbus response buffer into the sensor data structure
def parse_modbus_response(response):
    sensor_result = SensorData()
    sensor_result.dsp310_temperature = struct.unpack('<f', response.register_data[0:4])[0]
    sensor_result.pressure = struct.unpack('<f', response.register_data[4:8])[0]
    sensor_result.co2 = struct.unpack('<H', response.register_data[8:10])[0]
    sensor_result.pm2p5 = struct.unpack('<H', response.register_data[10:12])[0]
    sensor_result.pm10 = struct.unpack('<H', response.register_data[12:14])[0]
    sensor_result.temperature = struct.unpack('<h', response.register_data[14:16])[0]
    sensor_result.humidity = struct.unpack('<H', response.register_data[16:18])[0]
    
    return sensor_result

def main():
    # Example response buffer (fill with your actual response data)
    response_data = bytes([0x10, 0x20, 0x15, 0xCC, 0x5B, 0xE6, 0x41, 0x41, 0x02, 0xC6, 0x47,
                           0x90, 0x08, 0x02, 0x00, 0x02, 0x00, 0x19, 0x01, 0xCC, 0x02,
                           0x00, 0x00, 0x01, 0x2D, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00,
                           0x01, 0x2C, 0xC7])

    # Ensure the response_data length is at least 34 bytes
    if len(response_data) < 34:
        response_data += bytes(34 - len(response_data))

    # Create a ModbusResponse object
    response = ModbusResponse(response_data)

    # Print the Modbus response data
    print(f"Modbus ID: {response.modbus_id}")
    print(f"Function code: {response.function_code}")
    print(f"Byte count: {response.byte_count}")
    print(f"Version: {response.version}")
    print(f"CRC: {response.crc}")

    # Parse the sensor data
    sensor_result = parse_modbus_response(response)

    # Print the parsed sensor data
    print(f"Temperature (DSP310): {sensor_result.dsp310_temperature:.2f} °C")
    print(f"Air Pressure: {sensor_result.pressure:.2f} hPa")
    print(f"CO2 Concentration: {sensor_result.co2} ppm")
    print(f"PM2.5 Concentration: {sensor_result.pm2p5} µg/m³")
    print(f"PM10 Concentration: {sensor_result.pm10} µg/m³")
    print(f"Temperature (HDC1080): {sensor_result.temperature / 10.0:.1f} °C")
    print(f"Humidity: {sensor_result.humidity / 10.0:.1f} %")

if __name__ == "__main__":
    main()
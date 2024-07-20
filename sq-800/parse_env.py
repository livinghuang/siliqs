import struct
louver_format = '<2f3HhH'
louver_format_v2 = '<6s10s4H3f'

def modbus_crc(buffer):
    crc = 0xFFFF
    for pos in buffer:
        crc ^= pos
        for _ in range(8):
            if crc & 0x0001:
                crc >>= 1
                crc ^= 0xA001
            else:
                crc >>= 1
    return crc

class SensorData:
    def __init__(self, data):
        self.temperature, self.pressure, self.co2, self.pm2p5, self.pm10, self.temperature_adc, self.humidity = struct.unpack(louver_format, data)

class LouverDataStructV2:
    def __init__(self, data):
        self.deviceId, self.reserved, self.pm2p5, self.pm10, self.co2, self.humidity, self.hdc1080_temperature, self.dsp310_temperature, self.airPressure = struct.unpack(louver_format_v2, data)
def main():
    response = bytearray([0x10, 0x20, 0x15, 0xFC, 0xF2, 0xFB, 0x41, 0xBD, 0x6B, 0xC5, 0x47, 0x88, 0x0B, 0x02, 0x00, 0x0A, 0x00, 0xF9, 0x00, 0xD6, 0x02, 0x00, 0x00, 0x01, 0x61, 0xF5])
    response_v2 = bytearray([0x01, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47])

    if modbus_crc(response) == 0:
        data=response[3:(len(response)-5)]
        print(data)
        print(len(data))
        if len(data) == struct.calcsize(louver_format):
            global_sensor_result = SensorData(data)
            print(f"PM2.5={global_sensor_result.pm2p5} ug/m3")
            print(f"PM10={global_sensor_result.pm10} ug/m3")
            print(f"CO2={global_sensor_result.co2} ppm")
            print(f"Humidity={global_sensor_result.humidity} %")
            print(f"Temperature={global_sensor_result.temperature:.2f} C")
            print(f"Air Pressure={global_sensor_result.pressure:.2f} hPa")
        else:
            print("len error")
            print(len(data))
            print(struct.calcsize(louver_format))
    else:
        print("CRC error")

    if modbus_crc(response_v2) == 0:
        data=response_v2[3:(len(response_v2)-3)]
        print(data)
        if len(data) == struct.calcsize(louver_format_v2):
            global_louver_result = LouverDataStructV2(data)
            print(f"PM2.5={global_louver_result.pm2p5} ug/m3")
            print(f"PM10={global_louver_result.pm10} ug/m3")
            print(f"CO2={global_louver_result.co2} ppm")
            print(f"Humidity={global_louver_result.humidity} %")
            print(f"HDC1080 Temperature={global_louver_result.hdc1080_temperature:.2f} C")
            print(f"DSP310 Temperature={global_louver_result.dsp310_temperature:.2f} C")
            print(f"Air Pressure={global_louver_result.airPressure:.2f} hPa")
        else:
            print("len error")
            print(len(data))
            print(struct.calcsize(louver_format_v2))

    else:
        print("CRC error")

if __name__ == "__main__":
    main()
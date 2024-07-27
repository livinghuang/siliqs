import base64
import struct
import time

from formats import main_struct_format, louver_format, wind_format, gnss_format

def generate_sensor_data():
    """
    生成模拟传感器数据并编码为 Base64 字符串
    """
    station_type = 1  # 假设传感器类型为 1
    timestamp = 1655100000  # 固定时间戳
    station_id = 1
    station_mac = bytes([0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f])
    battery_voltage = 3.7
    station_reserved = bytes([0] * 8)
 
    louver_mac = bytes([0x11, 0x12, 0x13, 0x14, 0x15, 0x16])
    louver_reserved = bytes([0] * 10)
    pm2p5 = 50
    pm10 = 80
    co2 = 450
    humidity = 60
    hdc1080_temperature = 25.0
    dsp310_temperature = 26.0
    air_pressure = 1000.0
    
    
    wind_speed = 5
    wind_direction = 180

    latitude = 30.0
    longitude = 120.0
    altitude = 100.0
    gnss_reserved = bytes([0] * 4)

    # 按照定义的 struct 格式打包数据
    louver_data = struct.pack(louver_format, louver_mac, louver_reserved,pm2p5, pm10, co2, humidity,hdc1080_temperature, dsp310_temperature, air_pressure)
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

    # print size of packed_data
    print(len(packed_data))
    encoded_data = base64.b64encode(packed_data).decode()
    return encoded_data

# 测试生成函数
print(generate_sensor_data())



# SQS-LUV800: 8-in-1 Weather Sensor

![SQS-LUV800](./SQS-LUV800.png?raw=true)

## Overview

The SQS-LUV800 is an 8-in-1 weather sensor that integrates multiple functions including air pressure, temperature, CO2, PM2.5, PM10, humidity, wind direction, and wind speed. It offers a compact solution for comprehensive environmental monitoring with output via RS485.

## Key Features

- **Multiple Sensors Integration**: Combines air pressure, temperature, CO2, PM2.5, PM10, humidity, wind direction, and wind speed sensors.
- **RS485 Output**: Provides easy data transmission and integration with other systems.

## Application Scenarios

- Weather Stations
- Agriculture
- Building Environmental Monitoring
- General Environmental Monitoring

## Technical Specifications

- **Wind Speed**: ±0.5+2%m/s@60%RH,25°C
- **Wind Direction**: ±3°@60%RH,25°C
- **Temperature**: -40 to 85 °C±0.5°C
- **Humidity**: 0 to 100%±2%
- **CO2**: 400 to 3000 ppm±(50ppm+5%*result)
- **PM2.5**: 0～500 μg/m³±(10μg/m3@0～100μg/m3,10%@100～500μg/m3)
- **Air Pressure**: 300 to 1200 hPa±0.002hPa

## Usage Guide

### Querying Sensor Data

To query sensor data from the SQS-LUV800, send a request via RS485 using the Modbus protocol. The sensor will respond with two parts of the data packet containing the requested environmental data and wind data. You should use different commands to fetch each type of data.

## Modbus Data

### Environment Modbus Data

We support two commands (function calls) to fetch the data. Here is the command list:

- **Command_v1**: `10 20 02 00 00 4F C3` (old, not supported in the future)
- **Command_v2**: `10 21 02 00 00 4E 3F`

### Environment Modbus Data Example

Command_v1 : 10 20 02 00 00 4F C3

Response: 0x10, 0x20, 0x15, 0xFC, 0xF2, 0xFB, 0x41, 0xBD, 0x6B, 0xC5, 0x47, 0x88, 0x0B, 0x02, 0x00, 0x0A, 0x00, 0xF9, 0x00, 0xD6, 0x02, 0x00, 0x00, 0x02, 0x61, 0xF5

| Byte Index | Example Data          | Value Example (Hex) | Description              | Data Type | Present Value | Unit  | High-Low Byte Explanation                      |
|------------|-----------------------|---------------------|--------------------------|-----------|---------------|-------|------------------------------------------------|
| 0          | 0x10                  | 0x10                | Device Address           |           |               |       | No byte reversal needed                        |
| 1          | 0x20                  | 0x20                | Function Code            |           |               |       | No byte reversal needed                        |
| 2          | 0x15                  | 0x15                | Length                   | int8      |               |       | No byte reversal needed                        |
| 3 - 6      | 0xFC, 0xF2, 0xFB, 0x41| 0x41FBF2FC          | Temperature (DSP310)     | float     | 31.49         | °C    | Bytes need to be reversed to form correct float|
| 7 - 10     | 0xBD, 0x6B, 0xC5, 0x47| 0x47C56BBD          | Air Pressure             | float     | 101079.48        | hPa   | Bytes need to be reversed to form correct float|
| 11 - 12    | 0x88, 0x0B            | 0x0B88              | CO2 Concentration        | int16     | 2952          | ppm   | High and low bytes reversed                    |
| 13 - 14    | 0x02, 0x00            | 0x0002              | PM2.5 Concentration      | int16     | 2             | µg/m³ | High and low bytes reversed                    |
| 15 - 16    | 0x0A, 0x00            | 0x000A              | PM10 Concentration       | int16     | 10            | µg/m³ | High and low bytes reversed                    |
| 17 - 18    | 0xF9, 0x00            | 0x00F9              | Temperature (HDC1080)    | int16     | 31.49          | 0.1 °C| High and low bytes reversed                    |
| 19 - 20    | 0xD6, 0x02            | 0x02D6              | Humidity                 | int16     | 72.6          | 0.1 % | High and low bytes reversed                    |
| 21 - 22    | 0x00, 0x00            | 0x0000              | Reserved                 |           |               |       | No byte reversal needed                        |
| 23         | 0x02                  | 0x02                | Version                  | int8      | 2             |       | No byte reversal needed                        |
| 24 - 25    | 0x61, 0xF5            | 0xF561              | CRC                      |           |               |       | High and low bytes reversed                    |

### Environment Modbus Data Example_V2

Command: 10 21 02 00 00 4E 3F

Response: 0x10, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47

Based on the response data provided (0x10, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47), the table can be updated as follows:

| Byte Index | Example Data                | Value Example (Hex) | Description              | Data Type | Present Value | Unit  | High-Low Byte Explanation                      |
|------------|-----------------------------|---------------------|--------------------------|-----------|---------------|-------|------------------------------------------------|
| 0          | 0x10                        | 0x10                | Device Address           |           |               |       | No byte reversal needed                        |
| 1          | 0x21                        | 0x21                | Function Code            |           |               |       | No byte reversal needed                        |
| 2          | 0x25                        | 0x25                | Length                   | int8      |               |       | No byte reversal needed                        |
| 3 - 8      | 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C| 0x50E3B66E9E9C | Device ID               |           |               |       | No byte reversal needed                        |
| 9 - 18     | 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00| 0x00000000000000000000 | Reserved   |           |               |       | No byte reversal needed                        |
| 19 - 20    | 0x01, 0x00                  | 0x0001              | PM2.5 Concentration      | int16     | 1             | µg/m³ | High and low bytes reversed                    |
| 21 - 22    | 0x01, 0x00                  | 0x0001              | PM10 Concentration       | int16     | 1             | µg/m³ | High and low bytes reversed                    |
| 23 - 24    | 0xA1, 0x07                  | 0x07A1              | CO2 Concentration        | int16     | 1953          | ppm   | High and low bytes reversed                    |
| 25 - 26    | 0x9B, 0x02                  | 0x029B              | Humidity                 | int16     | 66.7          | %     | High and low bytes reversed                    |
| 27 - 30    | 0x1C, 0xC6, 0x90, 0x43      | 0x4390C61C          | Temperature (HDC1080)    | float     | 288.56        | °C    | Bytes need to be reversed to form correct float|
| 31 - 34    | 0xB8, 0xD6, 0xEB, 0x41      | 0x41EBD6B8          | Temperature (DSP310)     | float     | 29.73         | °C    | Bytes need to be reversed to form correct float|
| 35 - 38    | 0xBE, 0xDA, 0xC4, 0x47      | 0x47C4DABE          | Air Pressure             | float     | 1007.89       | hPa   | Bytes need to be reversed to form correct float|
| 39         | 0x02                        | 0x02                | Version                  | int8      | 2             |       | No byte reversal needed                        |
| 40 - 41    | 0xFF, 0x47                  | 0x47FF              | CRC                      |           |               |       | High and low bytes reversed                    |

#### Notes:
- **Device Address**: Identifies the device on the Modbus network.
- **Function Code**: Indicates the type of operation to be performed.
- **Length**: Specifies the length of the data field.
- **Reserved**: Bytes reserved for future use or padding.
- **Version**: Indicates the version of the firmware version.
- **CRC**: Used for error checking.

#### Example Parse Code in C:

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint16_t modbusCRC(uint8_t *buffer, int len)
{
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buffer[pos];
    for (int i = 8; i > 0; i--)
    { // 使用 > 保证循环次数明确
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

struct sensor_data
{
  float dsp310_temperature;
  float pressure;
  uint16_t co2;
  uint16_t pm2p5;
  uint16_t pm10;
  int16_t temperature;
  uint16_t humidity;
};

struct louver_data_struct_v2
{
  uint8_t deviceId[6];       // 6 bytes
  uint8_t reserved[10];      // 10 bytes (corrected comment)
  uint16_t pm2p5;            // 2 bytes
  uint16_t pm10;             // 2 bytes
  uint16_t co2;              // 2 bytes
  uint16_t humidity;         // 2 bytes
  float hdc1080_temperature; // 4 bytes
  float dsp310_temperature;  // 4 bytes
  float airPressure;         // 4 bytes
};

typedef struct sensor_data sensor_data_t;
typedef struct louver_data_struct_v2 louver_data_struct_v2_t;

sensor_data_t global_sensor_result;
louver_data_struct_v2_t global_louver_result;

int main()
{
  // Response buffer (fill with your actual response data)
  uint8_t response[] = {0x10, 0x20, 0x15, 0xFC, 0xF2, 0xFB, 0x41, 0xBD, 0x6B, 0xC5, 0x47, 0x88, 0x0B, 0x02, 0x00, 0x0A, 0x00, 0xF9, 0x00, 0xD6, 0x02, 0x00, 0x00, 0x01, 0x61, 0xF5};
  uint8_t response_v2[] = {0x10, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47}; // Interpret the response

  if (modbusCRC(response, sizeof(response)) == 0)
  {
    memcpy(&global_sensor_result, &response[3], sizeof(global_sensor_result));
    printf("PM2.5=%d ug/m3\n", global_sensor_result.pm2p5);
    printf("PM10=%d ug/m3\n", global_sensor_result.pm10);
    printf("CO2=%d ppm\n", global_sensor_result.co2);
    printf("Humidity=%d %%\n", global_sensor_result.humidity);
    printf("Temperature=%.2f C\n", global_sensor_result.temperature);
    printf("Air Pressure=%.2f hPa\n", global_sensor_result.pressure);
  }
  else
  {
    printf("CRC error\n");
  }

  if (modbusCRC(response_v2, sizeof(response_v2)) == 0)
  {
    memcpy(&global_louver_result, &response_v2[3], sizeof(global_louver_result));
    printf("PM2.5=%d ug/m3\n", global_louver_result.pm2p5);
    printf("PM10=%d ug/m3\n", global_louver_result.pm10);
    printf("CO2=%d ppm\n", global_louver_result.co2);
    printf("Humidity=%d %%\n", global_louver_result.humidity);
    printf("HDC1080 Temperature=%.2f C\n", global_louver_result.hdc1080_temperature);
    printf("DSP310 Temperature=%.2f C\n", global_louver_result.dsp310_temperature);
    printf("Air Pressure=%.2f hPa\n", global_louver_result.airPressure);
  }
  else
  {
    printf("CRC error\n");
  }

  return 0;
}
```

#### Example Parse Code in Python:

```python
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
```

### Modbus Data Example for Wind Data

**Command:** `01 03 00 00 00 02 C4 0B`

**Response:** `01 03 04 00 d3 01 1f 4b 92`

The given data `0x01, 0x03, 0x04, 0x00, 0xD3, 0x01, 0x1F, 0x4B, 0x92` can be interpreted as follows:

| Byte Index   | Example Data          | Value Example (Hex) | Description                          | Data Type | Present Value | Unit      | High-Low Byte Explanation                  |
|--------------|-----------------------|---------------------|--------------------------------------|-----------|---------------|-----------|-------------------------------------------|
| 0            | 0x01                  | 0x01                | Device Address                       |           |               |           |                                           |
| 1            | 0x03                  | 0x03                | Function Code                        |           |               |           |                                           |
| 2            | 0x04                  | 0x04                | Byte Count                           | int8      |               |           |                                           |
| 3 - 4        | 0x00, 0xD3            | 0x00D3              | Wind Direction                       | int16     | 211           | Degrees   | High and low bytes reversed                |
| 5 - 6        | 0x01, 0x1F            | 0x011F              | Wind Speed                           | int16     | 287           | 0.01m/s   | High and low bytes reversed                |
| 7 - 8        | 0x4B, 0x92            | 0x4B92              | CRC                                  |           |               |           | High and low bytes reversed                |

#### Example Parse Code in C:

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Define the wind data structure
struct wind_data
{
  uint16_t wind_direction;
  uint16_t wind_speed;
};

// Define the Modbus response structure for wind data
struct modbus_response_wind
{
  uint8_t modbus_id;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t register_data[4]; // 4 bytes for wind direction and speed
  uint16_t crc;
};

// Declare the global wind data result
struct wind_data wind_result;

// Function to parse the Modbus response buffer into the wind data structure
void parse_wind_response(const struct modbus_response_wind *response)
{
  wind_result.wind_direction = (response->register_data[0] << 8) | response->register_data[1];
  wind_result.wind_speed = (response->register_data[2] << 8) | response->register_data[3];
}

int main()
{
  // Example response buffer for wind data (fill with your actual response data)
  uint8_t response_data_wind[] = {0x01, 0x03, 0x04, 0x00, 0xD3, 0x01, 0x1F, 0x4B, 0x92};

  // Map the response data to the Modbus response structure for wind data
  struct modbus_response_wind response_wind;
  memcpy(&response_wind, response_data_wind, sizeof(response_wind));

  // Print the Modbus response data for wind
  printf("Modbus ID: %u\n", response_wind.modbus_id);
  printf("Function code: %u\n", response_wind.function_code);
  printf("Byte count: %u\n", response_wind.byte_count);
  printf("CRC: %u\n", response_wind.crc);

  // Parse the wind sensor data
  parse_wind_response(&response_wind);

  // Print the parsed wind data
  printf("Wind Direction: %u deg\n", wind_result.wind_direction);
  printf("Wind Speed: %.2f m/s\n", wind_result.wind_speed / 100.0);

  return 0;
}
```

#### Example Parse Code in Python:

```python
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
```

#### Explanation:
1. **Modbus Table**: Provides a detailed explanation of each byte in the Modbus response for wind data.
2. **Struct Definition**: The `wind_data` struct in C and `WindData` class in Python hold the parsed wind values.
3. **Modbus Response Structure**: The `modbus_response_wind` struct in C and `ModbusResponseWind` class in Python handle the Modbus response specific to wind data.
4. **Parsing Function**: `parse_wind_response` extracts wind direction and speed from the Modbus response.
5. **Main Function**: The example response buffer is mapped to the `modbus_response_wind` structure, parsed, and the wind values are printed.

### Summary

The response buffer from the SQS-LUV800 sensor contains environmental data encoded in a specific format. The provided code snippets in C and Python show how to interpret this data to extract useful information such as temperature, pressure, CO2 concentration, PM2.5 and PM10 concentrations, and humidity. By correctly parsing these values, users can integrate and use the data for various environmental monitoring applications.

### Setup

#### Wiring Connection

To connect the SQS-LUV800 sensor to your main system, follow the wiring instructions below:

- **Red Wire**: POWER IN (+10~24V)
- **Blue Wire**: GND
- **Yellow Wire**: RS485 A
- **Green Wire**: RS485 B

Refer to the wiring diagram below to ensure proper connections:

![Wiring Diagram](./wires.png?raw=true)

#### Implementation

For accurate setup, ensure that the sensor is oriented correctly with the designated point facing north. Refer to the image below for guidance on the correct positioning:

![North Point Setup](./point_to_north.png?raw=true)

By following these steps, you will ensure that the SQS-LUV800 sensor is properly connected and oriented for optimal performance.

## Additional Resources

(Include links or references to additional resources, manuals, or support documents.)

# ===
# SQS-LUV800：8合1氣象傳感器

![SQS-LUV800](./SQS-LUV800.png?raw=true)

## 概述

SQS-LUV800是一款8合1氣象傳感器，集成了多種功能，包括氣壓、溫度、二氧化碳、PM2.5、PM10、濕度、風向和風速。它提供了一個緊湊的解決方案，用於全面的環境監測，通過RS485輸出數據。

## 主要特點

- **多傳感器集成**：結合了氣壓、溫度、二氧化碳、PM2.5、PM10、濕度、風向和風速傳感器。
- **RS485輸出**：提供便捷的數據傳輸和與其他系統的集成。

## 應用場景

- 氣象站
- 農業
- 建築環境監測
- 一般環境監測

## 技術規格

- **風速**：±0.5+2%m/s@60%RH,25°C
- **風向**：±3°@60%RH,25°C
- **溫度**：-40 至 85 °C±0.5°C
- **濕度**：0 至 100%±2%
- **二氧化碳**：400 至 3000 ppm±(50ppm+5%*結果)
- **PM2.5**：0～500 μg/m³±(10μg/m3@0～100μg/m3,10%@100～500μg/m3)
- **氣壓**：300 至 1200 hPa±0.002hPa

## 使用指南

### 查詢傳感器數據

要從SQS-LUV800查詢傳感器數據，請通過RS485使用Modbus協議發送請求。傳感器將以兩部分數據包回應，分別包含所請求的環境數據和風速數據。需要使用不同的命令來獲取這些數據。

## Modbus數據

### 環境Modbus數據

我們支持兩種命令（函數調用）來獲取數據。以下是命令列表：

- **Command_v1**：`10 20 02 00 00 4F C3`（舊版，未來不再支持）
- **Command_v2**：`10 21 02 00 00 4E 3F`

### 環境Modbus數據範例

**Command_v1**：`10 20 02 00 00 4F C3`

回應：0x10, 0x20, 0x15, 0xFC, 0xF2, 0xFB, 0x41, 0xBD, 0x6B, 0xC5, 0x47, 0x88, 0x0B, 0x02, 0x00, 0x0A, 0x00, 0xF9, 0x00, 0xD6, 0x02, 0x00, 0x00, 0x02, 0x61, 0xF5

| 字節索引 | 範例數據             | 範例值 (Hex)  | 描述                     | 數據類型 | 當前值     | 單位    | 高低字節說明                           |
|----------|----------------------|---------------|--------------------------|----------|------------|---------|----------------------------------------|
| 0        | 0x10                 | 0x10          | 設備地址                 |          |            |         | 無需字節反轉                           |
| 1        | 0x20                 | 0x20          | 功能碼                   |          |            |         | 無需字節反轉                           |
| 2        | 0x15                 | 0x15          | 數據長度                 | int8     |            |         | 無需字節反轉                           |
| 3 - 6    | 0xFC, 0xF2, 0xFB, 0x41 | 0x41FBF2FC   | 溫度 (DSP310)            | float    | 31.49      | °C      | 需要反轉字節順序以形成正確的浮點數     |
| 7 - 10   | 0xBD, 0x6B, 0xC5, 0x47 | 0x47C56BBD   | 氣壓                    | float    | 101079.48  | hPa     | 需要反轉字節順序以形成正確的浮點數     |
| 11 - 12  | 0x88, 0x0B           | 0x0B88        | CO2 濃度                 | int16    | 2952       | ppm     | 高低字節反轉                           |
| 13 - 14  | 0x02, 0x00           | 0x0002        | PM2.5 濃度               | int16    | 2          | µg/m³   | 高低字節反轉                           |
| 15 - 16  | 0x0A, 0x00           | 0x000A        | PM10 濃度                | int16    | 10         | µg/m³   | 高低字節反轉                           |
| 17 - 18  | 0xF9, 0x00           | 0x00F9        | 溫度 (HDC1080)           | int16    | 31.49      | 0.1 °C  | 高低字節反轉                           |
| 19 - 20  | 0xD6, 0x02           | 0x02D6        | 濕度                    | int16    | 72.6       | 0.1 %   | 高低字節反轉                           |
| 21 - 22  | 0x00, 0x00           | 0x0000        | 保留                    |          |            |         | 無需字節反轉                           |
| 23       | 0x02                 | 0x02          | 版本                    | int8     | 2          |         | 無需字節反轉                           |
| 24 - 25  | 0x61, 0xF5           | 0xF561        | CRC                     |          |            |         | 高低字節反轉                           |

### 環境Modbus數據範例_V2

命令：10 21 02 00 00 4E 3F

回應：0x10, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47

根據提供的回應數據（0x10, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47），表格可以更新如下：

| 字節索引 | 範例數據                | 範例值 (Hex)  | 描述                     | 數據類型 | 當前值     | 單位    | 高低字節說明                           |
|----------|------------------------|---------------|--------------------------|----------|------------|---------|----------------------------------------|
| 0        | 0x10                  | 0x10          | 設備地址                 |          |            |         | 無需字節反轉                           |
| 1        | 0x21                  | 0x21          | 功能碼                   |          |            |         | 無需字節反轉                           |
| 2        | 0x25                  | 0x25          | 數據長度                 | int8     |            |         | 無需字節反轉                           |
| 3 - 8    | 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C| 0x50E3B66E9E9C | 設備 ID               |          |            |         | 無需字節反轉                           |
| 9 - 18   | 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00| 0x00000000000000000000 | 保留    |          |            |         | 無需字節反轉                           |
| 19 - 20  | 0x01, 0x00            | 0x0001        | PM2.5 濃度               | int16    | 1          | µg/m³   | 高低字節反轉                           |
| 21 - 22  | 0x01, 0x00            | 0x0001        | PM10 濃度                | int16    | 1          | µg/m³   | 高低字節反轉                           |
| 23 - 24  | 0xA1, 0x07            | 0x07A1        | CO2 濃度                 | int16    | 1953       | ppm     | 高低字節反轉                           |
| 25 - 26  | 0x9B, 0x02            | 0x029B        | 濕度                    | int16    | 66.7       | %       | 高低字節反轉                           |
| 27 - 30  | 0x1C, 0xC6, 0x90, 0x43| 0x4390C61C     | 溫度 (HDC1080)           | float    | 288.56     | °C      | 需要反轉字節順序以形成正確的浮點數     |
| 31 - 34  | 0xB8, 0xD6, 0xEB, 0x41| 0x41EBD6B8    | 溫度 (DSP310)            | float    | 29.73      | °C      | 需要反轉字節順序以形成正確的浮點數     |
| 35 - 38  | 0xBE, 0xDA, 0xC4, 0x47| 0x47C4DABE    | 氣壓                    | float    | 1007.89    | hPa     | 需要反轉字節順序以形成正確的浮點數     |
| 39       | 0x02                  | 0x02          | 版本                    | int8     | 2          |         | 無需字節反轉                           |
| 40 - 41  | 0xFF, 0x47            | 0x47FF        | CRC                     |          |            |         | 高低字節反轉                           |

#### 備註：
- **設備地址**：標識Modbus網絡上的設備。
- **功能碼**：表示要執行的操作類型。
- **數據長度**：指定數據字段的長度。
- **保留**：保留未來使用的字節或填充字節。
- **版本**：表示固件版本。
- **CRC**：用於錯誤檢查。

#### C語言範例解析代碼：

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint16_t modbusCRC(uint8_t *buffer, int len)
{
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buffer[pos];
    for (int i = 8; i > 0; i--)
    { // 使用 > 保證循環次數明確
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

struct sensor_data
{
  float dsp310_temperature;
  float pressure;
  uint16_t co2;
  uint16_t pm2p5;
  uint16_t pm10;
  int16_t temperature;
  uint16_t humidity;
};

struct louver_data_struct_v2
{
  uint8_t deviceId[6];       // 6 bytes
  uint8_t reserved[10];      // 10 bytes (corrected comment)
  uint16_t pm2p5;            // 2 bytes
  uint16_t pm10;             // 2 bytes
  uint16_t co2;              // 2 bytes
  uint16_t humidity;         // 2 bytes
  float hdc1080_temperature; // 4 bytes
  float dsp310_temperature;  // 4 bytes
  float airPressure;         // 4 bytes
};

typedef struct sensor_data sensor_data_t;
typedef struct louver_data_struct_v2 louver_data_struct_v2_t;

sensor_data_t global_sensor_result;
louver_data_struct_v2_t global_louver_result;

int main()
{
  // 回應緩衝區（用你的實際回應數據填充）
  uint8_t response[] = {0x10, 0x20, 0x15, 0xFC, 0xF2, 0xFB, 0x41, 0xBD, 0x6B, 0xC5, 0x47, 0x88, 0x0B, 0x02, 0x00, 0x0A, 0x00, 0xF9, 0x00, 0xD6, 0x02, 0x00, 0x00, 0x01, 0x61, 0xF5};
  uint8_t response_v2[] = {0x10, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47}; // 解釋回應

  if (modbusCRC(response, sizeof(response)) == 0)
  {
    memcpy(&global_sensor_result, &response[3], sizeof(global_sensor_result));
    printf("PM2.5=%d ug/m3\n", global_sensor_result.pm2p5);
    printf("PM10=%d ug/m3\n", global_sensor_result.pm10);
    printf("CO2=%d ppm\n", global_sensor_result.co2);
    printf("Humidity=%d %%\n", global_sensor_result.humidity);
    printf("Temperature=%.2f C\n", global_sensor_result.temperature);
    printf("Air Pressure=%.2f hPa\n", global_sensor_result.pressure);
  }
  else
  {
    printf("CRC error\n");
  }

  if (modbusCRC(response_v2, sizeof(response_v2)) == 0)
  {
    memcpy(&global_louver_result, &response_v2[3], sizeof(global_louver_result));
    printf("PM2.5=%d ug/m3\n", global_louver_result.pm2p5);
    printf("PM10=%d ug/m3\n", global_louver_result.pm10);
    printf("CO2=%d ppm\n", global_louver_result.co2);
    printf("Humidity=%d %%\n", global_louver_result.humidity);
    printf("HDC1080 Temperature=%.2f C\n", global_louver_result.hdc1080_temperature);
    printf("DSP310 Temperature=%.2f C\n", global_louver_result.dsp310_temperature);
    printf("Air Pressure=%.2f hPa\n", global_louver_result.airPressure);
  }
  else
  {
    printf("CRC error\n");
  }

  return 0;
}
```

#### Python語言範例解析代碼：

```python
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
```

### 風速數據Modbus範例

**命令**：`01 03 00 00 00 02 C4 0B`

**回應**：`01 03 04 00 d3 01 1f 4b 92`

給定數據`0x01, 0x03, 0x04, 0x00, 0xD3, 0x01, 0x1F, 0x4B, 0x92`可以解釋如下：

| 字節索引   | 範例數據          | 範例值 (Hex) | 描述                          | 數據類型 | 當前值     | 單位      | 高低字節說明                      |
|------------|-------------------|--------------|-------------------------------|----------|------------|-----------|----------------------------------|
| 0          | 0x01              | 0x01         | 設備地址                      |          |            |           |                                  |
| 1          | 0x03              | 0x03         | 功能碼                        |          |            |           |                                  |
| 2          | 0x04              | 0x04         | 字節數                        | int8     |            |           |                                  |
| 3 - 4      | 0x00, 0xD3        | 0x00D3       | 風向                          | int16    | 211        | 度        | 高低字節反轉                     |
| 5 - 6      | 0x01, 0x1F        | 0x011F       | 風速                          | int16    | 287        | 0.01m/s   | 高低字節反轉                     |
| 7 - 8      | 0x4B, 0x92        | 0x4B92       | CRC                           |          |            |           | 高低字節反轉                     |

#### C語言範例解析代碼：

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// 定義風速數據結構
struct wind_data
{
  uint16_t wind_direction;
  uint16_t wind_speed;
};

// 定義風速數據的Modbus回應結構
struct modbus_response_wind
{
  uint8_t modbus_id;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t register_data[4]; // 4個字節，表示風向和風速
  uint16_t crc;
};

// 聲明全局風速數據結果
struct wind_data wind_result;

// 解析Modbus回應緩衝區到風速數據結構的函數
void parse_wind_response(const struct modbus_response_wind *response)
{
  wind_result.wind_direction = (response->register_data[0] << 8) | response->register_data[1];
  wind_result.wind_speed = (response->register_data[2] << 8) | response->register_data[3];
}

int main()
{
  // 風速數據的回應緩衝區（用你的實際回應數據填充）
  uint8_t response_data_wind[] = {0x01, 0x03, 0x04, 0x00, 0xD3, 0x01, 0x1F, 0x4B, 0x92};

  // 將回應數據映射到風速數據的Modbus回應結構
  struct modbus_response_wind response_wind;
  memcpy(&response_wind, response_data_wind, sizeof(response_wind));

  // 打印風速數據的Modbus回應
  printf("Modbus ID: %u\n", response_wind.modbus_id);
  printf("Function code: %u\n", response_wind.function_code);
  printf("Byte count: %u\n", response_wind.byte_count);
  printf("CRC: %u\n", response_wind.crc);

  // 解析風速傳感器數據
  parse_wind_response(&response_wind);

  // 打印解析出的風速數據
  printf("Wind Direction: %u deg\n", wind_result.wind_direction);
  printf("Wind Speed: %.2f m/s\n", wind_result.wind_speed / 100.0);

  return 0;
}
```

#### Python語言範例解析代碼：

```python
import struct

# 定義風速數據結構
class WindData:
    def __init__(self):
        self.wind_direction = 0
        self.wind_speed = 0

# 定義風速數據的Modbus回應結構
class ModbusResponseWind:
    def __init__(self, data):
        if len(data) < 9:
            raise ValueError("數據長度過短")
        self.modbus_id = data[0]
        self.function_code = data[1]
        self.byte_count = data[2]
        self.register_data = data[3:7]
        self.crc = struct.unpack('>H', data[7:9])[0]

# 解析Modbus回應緩衝區到風速數據結構的函數
def parse_wind_response(response):
    wind_result = WindData()
    wind_result.wind_direction = struct.unpack('>H', response.register_data[0:2])[0]
    wind_result.wind_speed = struct.unpack('>H', response.register_data[2:4])[0]
    return wind_result

def main():
    # 風速數據的回應緩衝區（用你的實際回應數據填充）
    response_data_wind = bytes([0x01, 0x03, 0x04, 0x00, 0xD3, 0x01, 0x1F, 0x4B, 0x92])

    # 創建風速數據的Modbus回應對象
    response_wind = ModbusResponseWind(response_data_wind)

    # 打印風速數據的Modbus回應
    print(f"Modbus ID: {response_wind.modbus_id}")
    print(f"Function code: {response_wind.function_code}")
    print(f"Byte count: {response_wind.byte_count}")
    print(f"CRC: {response_wind.crc}")

    # 解析風速傳感器數據
    wind_result = parse_wind_response(response_wind)

    # 打印解析出的風速數據
    print(f"Wind Direction: {wind_result.wind_direction} deg")
    print(f"Wind Speed: {wind_result.wind_speed/100} m/s")

if __name__ == "__main__":
    main()
```

#### 說明：
1. **Modbus表**：提供Modbus回應中每個字節的詳細說明，適用於風速數據。
2. **結構定義**：C語言中的`wind_data`結構和Python中的`WindData`類保存解析的風速值。
3. **Modbus回應結構**：C語言中的`modbus_response_wind`結構和Python中的`ModbusResponseWind`類處理特定於風速數據的Modbus回應。
4. **解析函數**：`parse_wind_response`從Modbus回應中提取風向和風速。
5. **主函數**：範例回應緩衝區映射到`modbus_response_wind`結構，進行解析並打印風速值。

### 總結

SQS-LUV800傳感器的回應緩衝區包含以特定格式編碼的環境數據。提供的C語言和Python代碼片段展示了如何解析這些數據，從而提取有用的信息，如溫度、氣壓、二氧化碳濃度、PM2.5和PM10濃度以及濕度。通過正確解析這些值，用戶可以將數據整合並用於各種環境監測應用。

### 設置

#### 線路連接

要將SQS-LUV800傳感器連接到您的主系統，請按照以下接線說明進行：

- **紅線**：電源輸入 (+10~24V)
- **藍線**：GND
- **黃線**：RS485 A
- **綠線**：RS485 B

請參考以下接線圖，以確保正確連接：

![接線圖](./wires.png?raw=true)

#### 實施

為確保準確設置，請確保傳感器正確定位，指定的點朝北。請參考下圖以獲取正確定位的指導：

![北點設置](./point_to_north.png?raw=true)

按照這些步驟操作，確保SQS-LUV800傳感器正確連接和定位，以達到最佳性能。

## 附加資源

（包括鏈接或參考附加資源、手冊或支持文件。）
#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/README.md"
# Louver Box for Weather Detector

The Louver Box is a robust and efficient enclosure designed specifically for housing weather detection sensors. It integrates a variety of sensors to measure environmental parameters and transmits this data via Modbus protocol. This device is ideal for meteorological stations, environmental monitoring, and any application where precise weather data collection is needed.

## Features

- **High Precision Sensors**: Equipped with sensors to measure temperature, pressure, CO2 levels, particulate matter (PM2.5 and PM10), and humidity.
- **Modbus Communication**: Allows for easy integration into existing systems using the Modbus protocol (not strictly enforced in this device).
- **Compact and Durable Design**: Ensures that the sensors are protected in harsh environmental conditions while maintaining accurate readings.

## Technical Specifications

### Sensor Specifications

#### DSP310 Pressure Sensor

- **Operation Range**:
  - **Pressure**: 300 to 1200 hPa
  - **Temperature**: -40°C to 85°C
- **Pressure Sensor Precision**:
  - ±0.002 hPa (or ±0.02 m) in high precision mode
- **Relative Accuracy**:
  - ±0.06 hPa (or ±0.5 m)
- **Absolute Accuracy**:
  - ±1 hPa (or ±8 m)
- **Temperature Accuracy**:
  - ±0.5°C
- **Pressure Temperature Sensitivity**:
  - 0.5 Pa/K
- **Measurement Time**:
  - Typical: 27.6 ms for standard mode (16x)
  - Minimum: 3.6 ms for low precision mode

#### DS-CO2-20 CO2 Sensor

- **Model**: DS-CO2-20
- **Effective Measurement Range**: 400 to 3000 ppm
- **Maximum Range**: 400 to 5000 ppm
- **Resolution**: 1 ppm
- **Accuracy**: ±(50 ppm + 5% of reading)
- **Response Time**: T90 < 3 minutes (Time it takes for a 90% step change in concentration)
- **Data Refresh Time**: < 3 seconds (Standard is 1 second)
- **Warm-Up Time**:
  - < 25 seconds (Operational)
  - < 3 minutes for 90% accuracy
  - < 10 minutes for maximum accuracy
- **Operating Temperature Range**: -10°C to +50°C
- **Operating Humidity Range**: 0% to 85% RH, non-condensing

#### G5T (PMS5003T) for Humidity, PM2.5, PM10, and Temperature

- **Particulate Matter Measurement Range**:
  - **0.3 – 1.0 µm**
  - **1.0 – 2.5 µm**
  - **2.5 – 10 µm**

- **Particle Counting Efficiency**:
  - **50% @ 0.3 µm**
  - **98% @ ≥0.5 µm**

- **Particulate Matter Mass Concentration Effective Range (PM2.5 Standard)**:
  - **0 – 500 µg/m³**

- **Particulate Matter Mass Concentration Maximum Range (PM2.5 Standard)**:
  - **≥ 1000 µg/m³**

- **Particulate Matter Mass Concentration Resolution**:
  - **1 µg/m³**

- **Particulate Matter Mass Concentration Consistency (PM2.5 Standard)**:
  - **±10% @ 100 – 500 µg/m³**
  - **±10 µg/m³ @ 0 – 100 µg/m³**

- **Comprehensive Response Time**:
  - **≤ 10 seconds**

- **Temperature Measurement Range**:
  - **0 – 99°C**

- **Temperature Measurement Resolution**:
  - **0.1°C**

- **Operating Temperature Range**:
  - **-10 – +60°C**

- **Humidity Measurement Range**:
  - **0 – 99%**

- **Humidity Measurement Resolution**:
  - **0.1%**

- **Operating Humidity Range**:
  - **0 – 95% (non-condensing)**

### Electrical Specifications

- **Communication Protocol**: RS485 via MODBUS (not strictly enforced)
- **Power Supply**: 12 V
- **Power Consumption**:
  - **Peak**: 1 A
  - **Normal**: 80 mA
- **Storage Temperature Range**:
  - **-20 – +80°C**
- **Mean Time Between Failures (MTBF)**:
  - **≥ 3 years**

### Dimensions

![Louvered Weather Station Housing Picture](https://github.com/livinghuang/sinica_louver/blob/main/picture/SQSNC001-L-P.png "Weather Station Sensor Housing Picture")
![Louvered Weather Station Housing Drawing](https://github.com/livinghuang/sinica_louver/blob/main/picture/SQSSNC001-L-D.png "Weather Station Sensor Housing Drawing")

## Sensor Data Structure

The `sensor_data` structure is used to hold the sensor readings from the device:

```c
struct sensor_data {
  float dsp310_temperature; // Temperature from the DSP310 sensor in Celsius
  float pressure;           // Atmospheric pressure in hPa
  uint16_t co2;             // CO2 levels in parts per million (ppm)
  uint16_t pm2p5;           // Particulate matter PM2.5 in micrograms per cubic meter (µg/m³)
  uint16_t pm10;            // Particulate matter PM10 in micrograms per cubic meter (µg/m³)
  int16_t temperature;  // Temperature from the G5T sensor in Celsius
  uint16_t humidity;        // Relative humidity in percentage (%)
  uint16_t reserved;        // 
};
```

## Modbus Communication Protocol

The device uses the following Modbus protocol format to response:

- **Device ID**: `0x10`
- **Function Code**: `0x20`
- **Length**: 1 byte (specifies the length of the payload)
- **Payload**: 20 bytes (contains the sensor data)
- **Version**: 1 byte (indicates the version of the protocol)
- **Checksum**: 2 bytes (used for error checking)

### Data Mapping

The sensor data is mapped into the 20-byte payload as follows:

1. `dsp310_temperature`: 4 bytes (float)
2. `pressure`: 4 bytes (float)
3. `co2`: 2 bytes (uint16_t)
4. `pm2p5`: 2 bytes (uint16_t)
5. `pm10`: 2 bytes (uint16_t)
6. `g5t_temperature`: 2 bytes (int16_t)
7. `humidity`: 2 bytes (uint16_t)

## Communication Example

### Request-Response Example

#### Sending a Request

To query the sensor data from the device, you send a data request as follows:

- **Request**: `10 20 02 00 00 4F C3`

This request breakdown is as follows:
- `10`: Device ID
- `20`: Function Code
- `02`: Specifies the length of the payload
- `00 00`: Starting address (not used in this example)
- `4F C3`: CRC

#### Expected Response

The device responds with the following data packet:

- **Response**: `10 20 15 34 DE DD 41 62 96 C5 47 08 0B 10 00 18 00 EE 00 AE 02 00 00 01 26 51`

This response breakdown is as follows:

- `10`: Device ID (echoed back)
- `20`: Function Code (echoed back)
- `15`: Length of the payload (21 bytes of data)
- Next 20 bytes are the sensor data:
  - `34 DE DD 41`: DSP310 temperature (float)
  - `62 96 C5 47`: Pressure (float)
  - `08 0B`: CO2 (uint16_t)
  - `10 00`: PM2.5 (uint16_t)
  - `18 00`: PM10 (uint16_t)
  - `EE 00`: G5T temperature (int16_t)
  - `AE 02`: Humidity (uint16_t)  
  - `00 00`: reserved  
  - `01`: Version (1 byte, here as part of payload due to alignment)
  - `26 51`: Checksum (last 2 bytes)

### Parsing the Response

Here's how you might parse this response to get the sensor data:

```c
#include <stdio.h>
#include <stdint.h>

int main()
{
  // Response buffer (fill with your actual response data)
  uint8_t response[] = {0x10, 0x20, 0x15, 0x34, 0xDE, 0xDD, 0x41, 0x62, 0x96, 0xC5, 0x47,
                        0x08, 0x0B, 0x10, 0x00, 0x18, 0x00, 0xEE, 0x00, 0xAE, 0x02, 0x00,
                        0x00, 0x01, 0x26, 0x51};

  // Interpret the response
  float dsp310_temperature = *(float *)&response[3];
  float pressure = *(float *)&response[7];
  uint16_t co2 = *(uint16_t *)&response[11];
  uint16_t pm2p5 = *(uint16_t *)&response[13];
  uint16_t pm10 = *(uint16_t *)&response[15];
  int16_t g5t_temperature = *(int16_t *)&response[17];
  uint16_t humidity = *(uint16_t *)&response[19];

  printf("Temperature (DSP310): %.2f°C\n", dsp310_temperature);
  printf("Pressure: %.2f hPa\n", pressure);
  printf("CO2: %d ppm\n", co2);
  printf("PM2.5: %d µg/m³\n", pm2p5);
  printf("PM10: %d µg/m³\n", pm10);
  printf("Temperature (G5T): %f°C\n", (float)g5t_temperature / 10);
  printf("Humidity: %f%%\n", (float)humidity / 10);

  return 0;
}
```

## Support and Contact

For more information or support, please contact our technical support team at [support@siliqs.net](mailto:support@siliqs.net)

## REFLASH 

```
esptool.py --chip esp32c3 --port /dev/cu.wchusbserial840 --baud 921600 write_flash \
  0x0 /Users/living/code/github/built_sinica_louver_v2/sinica_louver_v2.ino.bootloader.bin \
  0x8000 /Users/living/code/github/built_sinica_louver_v2/sinica_louver_v2.ino.partitions.bin \
  0x10000 /Users/living/code/github/built_sinica_louver_v2/sinica_louver_v2.ino.bin
```
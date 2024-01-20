# Peripheral Board: SQS280

## Air Pressure and Temperature Sensor Board

## Overview

The SQS280 is a dedicated peripheral board designed for accurate measurements of air pressure and temperature. It features the BMP280 sensor, renowned for its precision in measuring these environmental parameters.

## Key Features

- **BMP280 Sensor**: This sensor is known for its precise air pressure and temperature measurements, making it ideal for applications requiring accurate environmental data.

- **Compact Design**: The SQS280 offers a compact and user-friendly design, facilitating integration into your projects.

- **Versatile Applications**: The SQS280 can be used in various applications, including weather monitoring, altitude sensing, and any project requiring accurate air pressure and temperature data.

## Pinout

![SQS280 Pinout Diagram](Link_to_Your_SQS280_Pinout_Diagram)

## Resources

- [Datasheet for BMP280](Link_to_BMP280_Datasheet)

For more detailed specifications, please refer to the BMP280 datasheet linked above.

## Usage

1. **Power Supply**: Connect the VCC and GND pins to a 3.3V power source.

2. **I2C Communication**: Utilize the SDA and SCL pins for I2C communication with your microcontroller.

3. **Data Retrieval**: Retrieve air pressure and temperature data from the BMP280 sensor using your microcontroller.

4. **Optional Features**: The BMP280 sensor may offer additional features such as altitude calculations, which can be accessed through appropriate library functions.

## Example Code

Here's a sample Python code snippet to read air pressure and temperature data from the SQS280 using a Raspberry Pi:

```python
# Sample Python code to read data from SQS280 using a Raspberry Pi
import smbus2
import time

# Define the I2C bus number (usually 1 for Raspberry Pi)
bus = smbus2.SMBus(1)

# BMP280 address
bmp280_addr = 0x76

# Configure BMP280 for air pressure and temperature measurements
bus.write_byte_data(bmp280_addr, 0xF4, 0x27)

# Read data from BMP280
data = bus.read_i2c_block_data(bmp280_addr, 0xF7, 8)

# Convert data to temperature and air pressure
temperature = ((data[3] << 12) + (data[4] << 4) + (data[5] >> 4)) / 100.0
pressure = ((data[0] << 16) + (data[1] << 8) + data[2]) / 100.0

print(f"Temperature (BMP280): {temperature}°C")
print(f"Air Pressure (BMP280): {pressure} hPa")
```
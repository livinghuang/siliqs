# Peripheral Board: SQS1080

## Humidity and Temperature Sensor Board

## Overview

The SQS1080 is a specialized peripheral board designed for precise measurements of humidity and temperature. It incorporates the HDC1080 sensor, a high-precision sensor renowned for its accurate humidity and temperature readings.

## Key Features

- **HDC1080 Sensor**: This sensor offers exceptional accuracy in measuring humidity and temperature, making it ideal for various environmental monitoring applications.

- **Compact Design**: The SQS1080 is a compact and easy-to-use board, perfect for integration into your projects.

- **Wide Range of Applications**: The SQS1080 is suitable for applications like weather monitoring, indoor climate control, and any projects requiring precise humidity and temperature measurements.

## Pinout

![SQS1080 Pinout Diagram](Link_to_Your_SQS1080_Pinout_Diagram)

## Resources

- [Datasheet for HDC1080](Link_to_HDC1080_Datasheet)

For more detailed specifications, please refer to the HDC1080 datasheet linked above.

## Usage

1. **Power Supply**: Connect the VCC and GND pins to a 3.3V power source.

2. **I2C Communication**: Utilize the SDA and SCL pins for I2C communication with your microcontroller.

3. **Data Retrieval**: Retrieve humidity and temperature data from the HDC1080 sensor using your microcontroller.

4. **Alert Function (Optional)**: You can configure the ALERT pin to trigger an interrupt when specific conditions are met, providing additional control over your application.

## Example Code

Here's a sample Python code snippet to read data from the SQS1080 using a Raspberry Pi:

```python
# Sample Python code to read data from SQS1080 using a Raspberry Pi
import smbus2
import time

# Define the I2C bus number (usually 1 for Raspberry Pi)
bus = smbus2.SMBus(1)

# HDC1080 address
hdc1080_addr = 0x40

# Configure HDC1080 to measure temperature and humidity
bus.write_i2c_block_data(hdc1080_addr, 0x02, [0x10])

# Read data from HDC1080
data = bus.read_i2c_block_data(hdc1080_addr, 0x00, 4)

# Convert data to temperature and humidity
temperature = ((data[0] << 8) + data[1]) / 65536.0 * 165.0 - 40.0
humidity = ((data[2] << 8) + data[3]) / 65536.0 * 100.0

print(f"Temperature (HDC1080): {temperature}°C")
print(f"Humidity (HDC1080): {humidity}%")
```

# SQS5883P(RDY)

### Overview

**(Ready For M.P)** The **SQS5883P module** integrates the QMC5883P 3-axis magnetometer sensor to provide precise and reliable magnetic field measurements, enabling accurate heading determination and orientation sensing. Designed for applications such as e-compass, navigation, motion tracking, and smart devices, it features compact size, low power consumption, and I²C communication for seamless integration into your projects.

### Key Features

* **QMC5883P Sensor:** Incorporates a high-precision 3-axis magneto-resistive sensor with built-in signal conditioning and 16-bit ADC.
* **High Accuracy Compass Heading:** Achieves 1° to 2° heading accuracy, suitable for mapping, robotics, and wearable navigation.
* **Low Power Operation:** Consumes as little as 35μA in normal mode (ODR = 10Hz, low power setting), making it ideal for battery-powered systems.
* **Flexible Data Output:** Supports multiple data output rates (up to 200Hz in normal mode, 1500Hz in continuous mode).
* **Simple Integration:** Communicates via I²C with support for standard and fast modes (100kHz and 400kHz).

### Functionalities

* **3-Axis Magnetic Field Sensing**
* **Self-Test Mode**
* **Temperature Compensation**

### Application Scenarios

* E-Compass Applications
* Wearable Devices
* Robotics & Drones
* Gaming & AR Systems
* Smart Home & Industrial Sensing

### Technical Specifications

* **Sensor Chip:** QMC5883P
* **Field Range:** ±30 Gauss
* **Resolution:** 1 mGauss
* **Heading Accuracy:** 1–2°
* **Data Rate:** Up to 200Hz (Normal), 1500Hz (Continuous)
* **Voltage:** 2.5–3.6V
* **Power Consumption:** 22μA (Suspend), \~35–600μA (Normal), \~2.2mA (Continuous)
* **I²C Address:** 0x2C
* **Operating Temp:** -40°C to +85°C
* **Package:** LGA-16 (3.0mm x 3.0mm x 0.9mm)

### Usage Guide

#### Connecting the Module

* VDD to 3.3V
* GND to GND
* SDA to SDA (e.g., D21 on ESP32, A4 on Uno)
* SCL to SCL (e.g., D22 on ESP32, A5 on Uno)
* Pull-up resistors (2.7kΩ–4.7kΩ) on SDA and SCL lines

#### Initialization Sequence

1. Write `0x06` to register `0x29`
2. Write `0x08` to register `0x0B`
3. Write `0xCD` to register `0x0A`

#### Reading Data

* Check register `0x09` bit 0 for new data
* Read registers `0x01`–`0x06` for X, Y, Z axis values (16-bit, two’s complement)

***

## Additional Resources

### Pinout

<figure><img src="../.gitbook/assets/截圖 2025-04-13 下午5.16.59.png" alt=""><figcaption></figcaption></figure>

### Arduino Example Code

You can use the `DFRobot_QMC5883` library or write your own minimal driver:

```cpp
#include <Wire.h>

#define QMC5883_ADDRESS 0x2C

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Sensor config
  Wire.beginTransmission(QMC5883_ADDRESS);
  Wire.write(0x29);
  Wire.write(0x06); // Axis config
  Wire.endTransmission();

  Wire.beginTransmission(QMC5883_ADDRESS);
  Wire.write(0x0B);
  Wire.write(0x08); // Set/Reset ON, 8G
  Wire.endTransmission();

  Wire.beginTransmission(QMC5883_ADDRESS);
  Wire.write(0x0A);
  Wire.write(0xCD); // Normal mode, 200Hz ODR, OSR1=1, OSR2=1
  Wire.endTransmission();
}

void loop() {
  Wire.beginTransmission(QMC5883_ADDRESS);
  Wire.write(0x09); // Status
  Wire.endTransmission(false);
  Wire.requestFrom(QMC5883_ADDRESS, 1);
  if (Wire.read() & 0x01) { // DRDY
    Wire.beginTransmission(QMC5883_ADDRESS);
    Wire.write(0x01); // Start of data
    Wire.endTransmission(false);
    Wire.requestFrom(QMC5883_ADDRESS, 6);
    int16_t x = Wire.read() | (Wire.read() << 8);
    int16_t y = Wire.read() | (Wire.read() << 8);
    int16_t z = Wire.read() | (Wire.read() << 8);

    Serial.print("X: "); Serial.print(x);
    Serial.print(" Y: "); Serial.print(y);
    Serial.print(" Z: "); Serial.println(z);
  }
  delay(100);
}
```

***

### Python Example Code (using `smbus2` on Raspberry Pi)

Install library first:

```bash
pip install smbus2
```

Python script:

```python
from smbus2 import SMBus
import time
import struct

ADDRESS = 0x2C

bus = SMBus(1)

def write_register(reg, value):
    bus.write_byte_data(ADDRESS, reg, value)

def read_xyz():
    status = bus.read_byte_data(ADDRESS, 0x09)
    if not status & 0x01:
        return None
    data = bus.read_i2c_block_data(ADDRESS, 0x01, 6)
    x, y, z = struct.unpack('<hhh', bytes(data))
    return x, y, z

# Initialization
write_register(0x29, 0x06)
write_register(0x0B, 0x08)
write_register(0x0A, 0xCD)

while True:
    result = read_xyz()
    if result:
        print("X: %d, Y: %d, Z: %d" % result)
    time.sleep(0.1)
```


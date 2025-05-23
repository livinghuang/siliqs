# SQS002 (M.P)

## Humidity, Temperature, and Air Pressure Sensor Board

## Overview

(Mass Production) The SQS002 is a versatile peripheral board designed to provide precise measurements of humidity, temperature, and air pressure. It integrates two essential sensors, the HDC1080 and DPS310, to fulfill these critical functions.

## Key Features

* **HDC1080 Sensor**: This high-precision sensor measures humidity and temperature with exceptional accuracy.
* **DPS310 Sensor**: Renowned for its precise air pressure and temperature measurements. Note that temperature readings are derived from the internal temperature sensor of the DPS310 chip.
* **Integration**: By combining the HDC1080 and DPS310 sensors on a single board, the SQS00 simplifies data acquisition and offers a compact solution for environmental monitoring.
* **Versatile Applications**: The SQS002 can be used in various applications, including weather monitoring, indoor climate control, and environmental sensing.

## Usage

1. **Power Supply**: Connect the VCC and GND pins to a 3.3V power source.
2. **I2C Communication**: Utilize the SDA and SCL pins for I2C communication with your microcontroller.
3. **Data Retrieval**: Retrieve humidity, temperature, and air pressure data from the HDC1080 and BMP280 sensors using your microcontroller.
4. **Alert Function (Optional)**: You can configure the ALERT pin to trigger an interrupt when specific conditions are met, providing additional control over your application.

## Additional Resources

### Pinout

![SQS002 Pinout Diagram](pinout.png)

### Schematic

![SQS002 Schematic Diagram](schematic.png)

### Datasheet

* [HDC1080 Datasheet](hdc1080.pdf)
* [DPS310 Datasheet](dps310.pdf)

### connection

1. Directly connect 1.25-4 connector with SQ002S.
2. Directly connect 2.54-5 header with SQ002S.

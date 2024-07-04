#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Define the sensor data structure
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

// Define the Modbus response structure
struct modbus_response
{
  uint8_t modbus_id;
  uint8_t function_code;
  uint8_t byte_count;
  uint8_t register_data[sizeof(struct sensor_data)]; // Big-endian data
  uint8_t version;
  uint16_t crc; // This will be manually attached, not part of the initial structure
};

// Declare the global sensor data result
struct sensor_data sensor_result;

// Function to parse the Modbus response buffer into the sensor data structure
void parse_modbus_response(const struct modbus_response *response)
{
  memcpy(&sensor_result, response->register_data, sizeof(sensor_result));
}

int main()
{
  // Example response buffer (fill with your actual response data)
  uint8_t response_data[] = {0x10, 0x20, 0x15, 0xCC, 0x5B, 0xE6, 0x41, 0x41, 0x02, 0xC6, 0x47,
                             0x90, 0x08, 0x02, 0x00, 0x02, 0x00, 0x19, 0x01, 0xCC, 0x02,
                             0x00, 0x00, 0x01, 0x2D, 0xC7};

  // Map the response data to the Modbus response structure
  struct modbus_response response;
  memcpy(&response, response_data, sizeof(response));

  // Print the Modbus response data
  printf("Modbus ID: %u\n", response.modbus_id);
  printf("Function code: %u\n", response.function_code);
  printf("Byte count: %u\n", response.byte_count);
  printf("Version: %u\n", response.version);
  printf("CRC: %u\n", response.crc);

  // Parse the sensor data
  parse_modbus_response(&response);

  // Print the parsed sensor data
  printf("Temperature (DSP310): %.2f °C\n", sensor_result.dsp310_temperature);
  printf("Air Pressure: %.2f hPa\n", sensor_result.pressure);
  printf("CO2 Concentration: %u ppm\n", sensor_result.co2);
  printf("PM2.5 Concentration: %u µg/m³\n", sensor_result.pm2p5);
  printf("PM10 Concentration: %u µg/m³\n", sensor_result.pm10);
  printf("Temperature (HDC1080): %.1f °C\n", sensor_result.temperature / 10.0);
  printf("Humidity: %.1f %%\n", sensor_result.humidity / 10.0);

  return 0;
}
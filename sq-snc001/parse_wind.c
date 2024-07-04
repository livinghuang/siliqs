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
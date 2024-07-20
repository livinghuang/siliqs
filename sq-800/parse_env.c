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
  uint8_t response_v2[] = {0x01, 0x21, 0x25, 0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0xA1, 0x07, 0x9B, 0x02, 0x1C, 0xC6, 0x90, 0x43, 0xB8, 0xD6, 0xEB, 0x41, 0xBE, 0xDA, 0xC4, 0x47, 0x02, 0xFF, 0x47}; // Interpret the response

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
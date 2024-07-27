#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/global.cpp"
#include "global.h"

struct sensor_data global_sensor_result;

struct louver_data_struct_v2 global_louver_result;

void printHex(byte *data, int length)
{
  for (int i = 0; i < length; i++)
  {
    // Print each byte in hexadecimal format with leading zeros
    if (data[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println(); // Print a newline character at the end
}
void get_chip_id(uint8_t *chip_id)
{
  uint64_t chipid = ESP.getEfuseMac();
  // Extract the 48-bit MAC address from the 64-bit value
  chip_id[0] = (chipid >> 40) & 0xFF;
  chip_id[1] = (chipid >> 32) & 0xFF;
  chip_id[2] = (chipid >> 24) & 0xFF;
  chip_id[3] = (chipid >> 16) & 0xFF;
  chip_id[4] = (chipid >> 8) & 0xFF;
  chip_id[5] = chipid & 0xFF;

  printHex(chip_id, 6);
}

void swap_bytes(uint16_t *value)
{
  *value = (*value << 8) | (*value >> 8);
}

uint16_t raw_sum(uint8_t *buffer, int len)
{
  uint16_t sum = 0;
  for (int i = 0; i < len - 2; i++)
  {
    sum += buffer[i];
  }
  return sum;
}

#line 1 "/Users/living/code/github/siliqs/sq001/bat.cpp"

#include "global.h"

float getBatteryVoltage()
{
  uint32_t sum = 0;
  uint32_t test_min = 1970;
  uint32_t test_max = 2880;
  pinMode(2, ANALOG);
  for (size_t i = 0; i < 16; i++)
  {
    uint16_t analog = analogRead(2);
    // Serial.print("BAT:");
    // Serial.println(analog);
    sum += analog;
  }
  uint32_t raw = sum >> 4;
  Serial.print("adc raw voltage:");
  Serial.println(raw);
  float avg = (float)(raw);
  Serial.print("battery voltage:");
  Serial.println(avg);
  float result = ((avg - test_min) * (4.2 - 3) / (test_max - test_min) + 3);
  Serial.print("result=");
  Serial.print(result);
  return result;
}

uint8_t getBatteryLevel()
{
  const float maxBattery = 4.2;
  const float minBattery = 3.0;
  const float batVolt = getBatteryVoltage();
  const float batVoltage = fmax(minBattery, fmin(maxBattery, batVolt));
  uint8_t batLevel = BAT_LEVEL_EMPTY + ((batVoltage - minBattery) / (maxBattery - minBattery)) * (BAT_LEVEL_FULL - BAT_LEVEL_EMPTY);
  if (batVolt > 4.2)
  {
    batLevel = 255;
  }
  if (batVolt < 3.0)
  {
    batLevel = 0;
  }
  Serial.print("Battery Level: ");
  Serial.println(batLevel);
  return batLevel;
}
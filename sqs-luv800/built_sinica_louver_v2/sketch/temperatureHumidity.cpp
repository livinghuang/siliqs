#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/temperatureHumidity.cpp"
#include "global.h"
#include "temperatureHumidity.h"
HDC1080 hdc1080;
bool hdc1080_fetch_finished = false;
void temperatureHumidity_init()
{
  hdc1080.begin();
  vTaskDelay(10);
  hdc1080_fetch_finished = false;
}
void temperatureHumidity_loop()
{

  float temperature = hdc1080.readTemperature();
  float humidity = hdc1080.readHumidity();
  temperature = hdc1080.readTemperature();
  humidity = hdc1080.readHumidity();
  hdc1080.end();
  Serial.printf("T=%.2f degC, Humidity=%.2f %\n", temperature, humidity);
  global_sensor_result.temperature = temperature * 10;
  global_sensor_result.humidity = humidity * 10;

  global_louver_result.hdc1080_temperature = temperature;
  global_louver_result.humidity = humidity * 10;

  vTaskDelay(10);
  hdc1080_fetch_finished = true;
}
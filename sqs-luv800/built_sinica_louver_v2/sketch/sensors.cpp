#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sensors.cpp"
#include "sensors.h"

enum sensor_process
{
  SENSOR_START,
  HDC1080_INIT,
  HDC1080_LOOP,
  DS310_INIT,
  DS310_LOOP,
  G5T_INIT,
  G5T_LOOP,
  DSCO2_INIT,
  DSCO2_LOOP,
  SENSOR_END
};

enum sensor_process sensor_process = SENSOR_START;

void sensors_init()
{
  pinMode(pG5T_RST, OUTPUT);
  digitalWrite(pG5T_RST, HIGH);
  vTaskDelay(10000);
  sensor_process = SENSOR_START;
}
void sensors_loop()
{
  static uint8_t dsco2_trys = 0;
  static uint8_t g5t_trys = 0;
  static uint8_t buffer[64];
  uint8_t len;
  uint16_t crc;
  size_t messageSize;
  switch (sensor_process)
  {
  case SENSOR_START:
    Serial.println("Sensor Start");
    sensor_process = HDC1080_INIT;
    break;
  case HDC1080_INIT:
    Serial.println("HDC1080 Init");
    hdc1080_fetch_finished = false;
    temperatureHumidity_init();
    sensor_process = HDC1080_LOOP;
    break;
  case HDC1080_LOOP:
    temperatureHumidity_loop();
    if (hdc1080_fetch_finished)
    {
      sensor_process = DS310_INIT;
    }
    break;
  case DS310_INIT:
    Serial.println("DS310 Init");
    dsp310_fetch_finished = false;
    atmosphericPressure_init();
    sensor_process = DS310_LOOP;
    break;
  case DS310_LOOP:
    atmosphericPressure_loop();
    if (dsp310_fetch_finished)
    {
      Wire.end();
      sensor_process = G5T_INIT;
    }
    break;
  case G5T_INIT:
    Serial.println("G5T Init");
    g5t_trys++;
    g5t_fetch_finished = false;
    g5t_init_v2();
    sensor_process = G5T_LOOP;
    break;
  case G5T_LOOP:
    g5t_loop_v2();
    if ((g5t_fetch_finished) || (g5t_trys > 10))
    {
      g5t_trys = 0;
      sensor_process = DSCO2_INIT;
    }
    else
    {
      sensor_process = G5T_INIT;
    }
    break;
  case DSCO2_INIT:
    Serial.println("DSCO2 Init");
    dsco2_trys++;
    dsco2_fetch_finished = false;
    dsco2_init_v2();
    sensor_process = DSCO2_LOOP;
    break;
  case DSCO2_LOOP:
    dsco2_loop_v2();
    if ((dsco2_fetch_finished) || (dsco2_trys > 10))
    {
      dsco2_trys = 0;
      sensor_process = SENSOR_END;
    }
    else
    {
      sensor_process = DSCO2_INIT;
    }
    break;
  case SENSOR_END:
    Serial.print("global_louver_result: ");
    printHex((byte *)&global_louver_result, sizeof(global_louver_result));
    Serial.print("deviceId: ");
    printHex((byte *)&global_louver_result.deviceId, sizeof(global_louver_result.deviceId));
    Serial.printf("pm2p5: %d\n", global_louver_result.pm2p5);
    Serial.printf("pm10: %d\n", global_louver_result.pm10);
    Serial.printf("dsco2: %d\n", global_louver_result.co2);
    Serial.printf("humidity: %d\n", global_louver_result.humidity);
    Serial.printf("hdc1080_temperature: %f\n", global_louver_result.hdc1080_temperature);
    Serial.printf("dsp310_temperature: %f\n", global_louver_result.dsp310_temperature);
    Serial.printf("airPressure: %f\n", global_louver_result.airPressure);
    get_louver_v2_modbus_response();
    Serial.flush();

    Serial.print("global_sensor_result: ");
    printHex((byte *)&global_sensor_result, sizeof(global_sensor_result));
    Serial.printf("pm2p5: %d\n", global_sensor_result.pm2p5);
    Serial.printf("pm10: %d\n", global_sensor_result.pm10);
    Serial.printf("dsco2: %d\n", global_louver_result.co2);
    Serial.printf("temperature: %d\n", global_sensor_result.temperature);
    Serial.printf("humidity: %d\n", global_sensor_result.humidity);
    Serial.printf("dsp310_temperature: %f\n", global_sensor_result.dsp310_temperature);
    Serial.printf("pressure: %f\n", global_sensor_result.pressure);
    get_sensor_modbus_response();
    Serial.println("End");
    Serial.flush();
    vTaskDelay(30000);
    sensor_process = SENSOR_START;
    break;
  default:
    break;
  }
}
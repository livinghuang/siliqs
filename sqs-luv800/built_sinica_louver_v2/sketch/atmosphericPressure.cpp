#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/atmosphericPressure.cpp"
#include "atmosphericPressure.h"

bool dsp310_fetch_finished = false;
Dps310 Dps310PressureSensor;
void atmosphericPressure_init()
{
  Wire.setPins(pSDA, pSCL);
  // Call begin to initialize Dps310PressureSensor
  // The parameter 0x76 is the bus address. The default address is 0x77 and does not need to be given.
  // Dps310PressureSensor.begin(Wire, 0x76);
  // Use the commented line below instead of the one above to use the default I2C address.
  // if you are using the Pressure 3 click Board, you need 0x76
  Dps310PressureSensor.begin(Wire, 0x77);
  vTaskDelay(500);
  Serial.println("Init complete!");
}
void atmosphericPressure_loop()
{
  float temperature;
  float pressure;
  uint8_t oversampling = 7;
  int16_t ret;
  Serial.println();

  // lets the Dps310 perform a Single temperature measurement with the last (or standard) configuration
  // The result will be written to the paramerter temperature
  // ret = Dps310PressureSensor.measureTempOnce(temperature);
  // the commented line below does exactly the same as the one above, but you can also config the precision
  // oversampling can be a value from 0 to 7
  // the Dps 310 will perform 2^oversampling internal temperature measurements and combine them to one result with higher precision
  // measurements with higher precision take more time, consult datasheet for more information
  ret = Dps310PressureSensor.measureTempOnce(temperature, oversampling);

  if (ret != 0)
  {
    // Something went wrong.
    // Look at the library code for more information about return codes
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" degrees of Celsius");
    global_sensor_result.dsp310_temperature = temperature;
    global_louver_result.dsp310_temperature = temperature;
  }

  // Pressure measurement behaves like temperature measurement
  // ret = Dps310PressureSensor.measurePressureOnce(pressure);
  ret = Dps310PressureSensor.measurePressureOnce(pressure, oversampling);
  if (ret != 0)
  {
    // Something went wrong.
    // Look at the library code for more information about return codes
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pascal");
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      global_sensor_result.pressure = pressure;
      global_louver_result.airPressure = pressure;
      // 访问共享资源后解锁互斥量
      xSemaphoreGive(xMutex);
    }
  }
  dsp310_fetch_finished = true;
  Dps310PressureSensor.end();
  // Wait some time
}
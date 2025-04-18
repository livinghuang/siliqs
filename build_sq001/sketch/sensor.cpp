#line 1 "/Users/living/code/github/siliqs/sq001s/sensor.cpp"
#include "global.h"

struct hdc1080_data
{
  float temperature;
  float humidity;
} hdc1080_result;

struct bmp280_data
{
  float bmp280_internal_temperature;
  float pressure;
} bmp280_result;

BMP280 bmp;
HDC1080 hdc1080;
Dps310 Dps310PressureSensor = Dps310();

void SPI_RESET(void)
{
  bool rst = false;
  extern RadioEvents_t RadioEvents;
  Radio.Init(&RadioEvents);
  Radio.Sleep();
  SPI.end();

  extern RadioEvents_t RadioEvents;
  Radio.Init(&RadioEvents);
  Radio.Sleep();
  SPI.end();

  pinMode(RADIO_DIO_1, ANALOG);
  pinMode(RADIO_NSS, ANALOG);
  pinMode(RADIO_RESET, ANALOG);
  pinMode(RADIO_BUSY, ANALOG);
  pinMode(LORA_CLK, ANALOG);
  pinMode(LORA_MISO, ANALOG);
  pinMode(LORA_MOSI, ANALOG);

  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, HIGH);
  pinMode(RADIO_RESET, OUTPUT);
  digitalWrite(RADIO_RESET, LOW);
  delay(10);
}

// 从MAX31865读取寄存器的函数

byte readRegister(byte regAddress)
{
  digitalWrite(pCS, LOW);                                          // 选中MAX31865
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1)); // 根据MAX31865的要求设置SPI参数
  SPI.transfer(regAddress | 0x80);                                 // 发送寄存器地址，加上读取操作的标志
  byte regValue = SPI.transfer(0x00);                              // 发送哑元字节来读取寄存器的值
  SPI.endTransaction();
  digitalWrite(pCS, HIGH); // 取消选中MAX31865
  return regValue;         // 返回寄存器的值
}
void writeRegister(byte regAddress, byte value)
{
  digitalWrite(pCS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  SPI.transfer(regAddress & 0x7F); // The write operation address (clear MSB)
  SPI.transfer(value);
  SPI.endTransaction();
  digitalWrite(pCS, HIGH);
}

void pt100_init()
{
  max31865_init();
}

void pt100_fetch(void)
{
  max31865_process();
}

bool hdc1080_fetch(void)
{
  if (!hdc1080.begin())
  {
    return 0;
  }
  float temp = hdc1080.readTemperature();
  float Humidity = hdc1080.readHumidity();
  temp = hdc1080.readTemperature();
  Humidity = hdc1080.readHumidity();
  hdc1080.end();
  Serial.printf("T=%.2f degC, Humidity=%.2f %\n", temp, Humidity);
  hdc1080_result.temperature = temp;
  hdc1080_result.humidity = Humidity;
  global_sensor_result.humidity = Humidity;
  global_sensor_result.temperature = temp;
  return 1;
}

bool bmp280_fetch(void)
{
  if (!bmp.begin())
  {
    return 0;
  }
  bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                  BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  BMP280::FILTER_X16,      /* Filtering. */
                  BMP280::STANDBY_MS_500); /* Standby time. */
  delay(20);
  float temp = bmp.readTemperature();
  float Pressure = (float)bmp.readPressure() / 100.0;
  int c = 0;
  while ((temp < -50) || (Pressure > 1100) || (Pressure < 500))
  {
    bmp.putBMP280ToSleep();
    delay(10);
    bmp.end();
    Serial.println("BMP ERROR");
    Serial.flush();
    bmp.begin();
    delay(10);
    bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                    BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    BMP280::FILTER_X16,      /* Filtering. */
                    BMP280::STANDBY_MS_500); /* Standby time. */
    temp = bmp.readTemperature();
    Pressure = (float)bmp.readPressure() / 100.0;
    delay(10);
    c++;
    if (c > 3)
    {
      return false;
    }
  }
  bmp.putBMP280ToSleep();
  delay(10);
  bmp.end();
  Serial.printf("T=%.2f degC, Pressure=%.2f hPa\n", temp, Pressure);
  bmp280_result.bmp280_internal_temperature = temp;
  bmp280_result.pressure = Pressure;
  global_sensor_result.pressure = Pressure;
  return true;
}

void dsp310_fetch(void)
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
    global_sensor_result.pressure = pressure;
  }

  // Wait some time
  delay(500);
}

void fetchSensorData()
{
  power_On_Sensor_Bus();
  delay(5);
  while (1)
  {
    bmp280_fetch();
    hdc1080_fetch();
    delay(1000);
  }
  power_Off_Sensor_Bus();
}
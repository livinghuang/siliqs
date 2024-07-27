#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/global.h"
#ifndef GLOBAL_H
#define GLOBAL_H
#define v2
#include <WiFi.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "esp_system.h"
#include "bsp.h"
#include "Wire.h"
#include "esp_sleep.h"
#include <esp_timer.h>

#include "SPI.h"

#include "bsp.h"
#include "modbus.h"
#include "atmosphericPressure.h"
#include "temperatureHumidity.h"
#include "dsco2.h"
#include "g5t.h"
#include "sensors.h"

extern SemaphoreHandle_t xMutex;

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
  uint8_t reserved[10];      // 2 bytes
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
extern sensor_data_t global_sensor_result;
extern louver_data_struct_v2_t global_louver_result;
void printHex(byte *data, int length);
void get_chip_id(uint8_t *chip_id);
void swap_bytes(uint16_t *value);
uint16_t raw_sum(uint8_t *buffer, int len);

#endif
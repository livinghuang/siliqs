#ifndef GLOBAL_H
#define GLOBAL_H

#include "Arduino.h"
#include "esp_system.h"

#include "WiFi.h"
#include "Wire.h"
#include "esp_sleep.h"
#include <esp_timer.h>
#include <mbedtls/sha256.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Base64.h>
#include <time.h>
#include <sys/time.h>
#include <WiFiMulti.h>

struct louver_data_struct_v2
{
  uint8_t louver_mac[6];     // 6 bytes
  uint8_t reserved[10];      // 2 bytes
  uint16_t pm2p5;            // 2 bytes
  uint16_t pm10;             // 2 bytes
  uint16_t co2;              // 2 bytes
  uint16_t humidity;         // 2 bytes
  float hdc1080_temperature; // 4 bytes
  float dsp310_temperature;  // 4 bytes
  float air_pressure;        // 4 bytes
};

struct wind_data_struct_v2
{
  uint16_t wind_speed;
  uint16_t wind_direction;
};

struct gnss_data_struct_v2
{
  float latitude;
  float longitude;
  float altitude;
  uint8_t reserved[4];
};

struct station_data_v2
{
  uint16_t station_type;               // 2 bytes
  uint16_t station_id;                 // 2 bytes
  uint32_t timestamp;                  // 4 bytes
  uint8_t station_mac[16];             // 16 bytes
  float battery_voltage;               // 4 bytes
  uint8_t station_reserved[8];         // 8 bytes
  struct louver_data_struct_v2 louver; // 36 bytes
  struct wind_data_struct_v2 wind;     // 4 bytes
  struct gnss_data_struct_v2 gnss;     // 16 bytes
};
extern struct station_data_v2 global_station_v2_result; // 76 bytes
extern String mqtt_message_base64;
union UINT16
{
  uint16_t whole;
  struct
  {
    uint8_t low;
    uint8_t high;
  } bytes;
};

extern bool publish_finished;
void printHex(byte *data, int length);
void run_with_time_escape(uint64_t escape_period_ms, void (*callback)(), void (*stop_callback)());
esp_sleep_wakeup_cause_t print_wakeup_reason();
uint64_t get_chip_id(void);
uint16_t calculateCRC(uint8_t *buf, int length);
void print_sensor_data(void);
uint32_t get_time_stamp(void);
void generate_fixed_sensor_data(uint32_t sensor_id, char *encoded_data);
#endif
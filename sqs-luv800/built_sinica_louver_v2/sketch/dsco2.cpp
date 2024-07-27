#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/dsco2.cpp"
#include "global.h"

#include "SoftwareSerial.h"
bool dsco2_fetch_finished = false;

struct dsco2_data_v2
{
  uint16_t address;
  uint16_t length;
  uint16_t co2;
  uint16_t param1;
  uint16_t param2;
  uint16_t checksum;
};

union dsco2_data_union
{
  uint16_t rawBytes[sizeof(dsco2_data_v2) / 2];
  struct dsco2_data_v2 data;
};
union dsco2_data_union dsco2_data_v2;
SoftwareSerial softSerial1(pDSCO2_RX, pDSCO2_TX); // RX, TX

void dsco2_init_v2()
{
  // Initialize
  softSerial1.begin(9600);
  dsco2_fetch_finished = false;
  // for (int i = 0; i < 10; i++)
  // {
  //   Serial.printf("%d ", i);
  vTaskDelay(100);
  // }
  // Serial.println();
}

void dsco2_loop_v2(void)
{
  byte request[] = {0x42, 0x4d, 0xe3, 0x00, 0x00, 0x01, 0x72};
  softSerial1.write(request, sizeof(request)); // Send the request
  Serial.println("Start");
  softSerial1.flush();
  unsigned long startTime = millis();
  unsigned long timeout = 5000; // Timeout in milliseconds, e.g., 5000ms for 5 seconds

  while (!softSerial1.readBytes((byte *)&dsco2_data_v2, sizeof(dsco2_data_v2)))
  {
    if (millis() - startTime >= timeout)
    {
      Serial.println("Timeout occurred!");
      return;
    }
  }
  softSerial1.end();
  printHex((byte *)&dsco2_data_v2, sizeof(dsco2_data_v2));

  if (!memcmp((uint8_t *)&dsco2_data_v2.data.address, (uint8_t *)&request, sizeof(dsco2_data_v2.data.address)) != 0)
  {
    Serial.println("address okay");
    printHex((byte *)&dsco2_data_v2.data.address, sizeof(dsco2_data_v2.data.address));

    for (int i = 0; i < sizeof(dsco2_data_v2) / 2; i++)
    {
      swap_bytes((uint16_t *)(&dsco2_data_v2.rawBytes[i]));
    }
    // printHex((byte *)&dsco2_data_v2, sizeof(dsco2_data_v2));

    if ((dsco2_data_v2.data.checksum - raw_sum((byte *)&dsco2_data_v2, sizeof(dsco2_data_v2))) == 0)
    {
      Serial.println("checksum okay");
    }
    else
    {
      Serial.println("checksum error");
      return;
    }

    Serial.printf("length: %d\n", dsco2_data_v2.data.length);
    Serial.printf("co2: %d\n", dsco2_data_v2.data.co2);
    global_sensor_result.co2 = dsco2_data_v2.data.co2;
    global_louver_result.co2 = dsco2_data_v2.data.co2;
  }

  softSerial1.end();
  dsco2_fetch_finished = true;
}

#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/g5t.cpp"
#include "global.h"

#include "SoftwareSerial.h"
bool g5t_fetch_finished = false;

/*
数据1高八位
……
数据
1表示 PM1.0浓度（ CF=1，标准颗粒物）单位 μg/m³
数据1低八位
……
数据2高八位
……
数据
2表示 PM2.5浓度（ CF=1，标准颗粒物）单位 μg/m³
数据2低八位
……
数据3高八位
……
数据
3表示 PM10浓度（ CF=1，标准颗粒物）单位 μg/m³
数据3低八位
……
数据4高八位
……
数据
4表示 PM1.0浓度（大气环境下）单位： μg/m³
数据4低八位
……
数据5高八位
……
数据
5表示 PM2.5浓度（大气环境下）单位： μg/m³
数据5低八位
……
数据6高八位
……
数据
6表示 PM10浓度（大气环境下）单位： μg/m³
数据6低八位
……
数据7高八位
……
数据
7表示 0.1升空气中直径在 0.3um以上颗粒物个数
数据7低八位
……
数据8高八位
……
数据
8表示 0.1升空气中直径在 0.5um以上颗粒物个数
数据8低八位
……
数据9高八位
……
数据
9表示 0.1升空气中直径在 1.0um以上颗粒物个数
数据9低八位
……
数据10高八位
……
数据
10表示 0.1升空气中直径在 2.5um以上颗粒物个数
数据10低八位
……
数据11高八位
……
数据
11表示 温度
注：真实温度值
=本数值 /10 单位：℃ 数据11低八位
……
数据12高八位
……
数据
12表示 湿度
注：真实湿度值
=本数值 /10 单位： ：%

标准颗粒物质量浓度值是指用工业金属颗粒物作为等效颗粒进行密度换算得到的质量浓度值，适
用于工业生产车间等环境。大气环境颗粒物质量浓度值以空气中主要污染物为等效颗粒进行密度换
算，适用于普通室内外大气环境。
*/

SoftwareSerial pmsSerial(pG5T_RX, pG5T_TX); // 將Arduino Pin2設定為RX, Pin3設定為TX

struct g5t_data_v2
{
  uint16_t address;
  uint16_t length;
  uint16_t fpm1p0;
  uint16_t fpm2p5;
  uint16_t fpm10;
  uint16_t pm1p0;
  uint16_t pm2p5;
  uint16_t pm10;
  uint16_t um03;
  uint16_t um05;
  uint16_t um10;
  uint16_t um25;
  int16_t temperature;
  uint16_t humidity;
  uint8_t version;
  uint8_t errorCode;
  uint16_t checksum;
};

union g5t_data_union
{
  uint16_t rawBytes[sizeof(g5t_data_v2) / 2];
  struct g5t_data_v2 data;
};

union g5t_data_union g5t_data_v2;

void g5t_init_v2()
{

  // delay(100);
  pmsSerial.begin(9600);
  g5t_fetch_finished = false;
  // for (int i = 0; i < 10; i++)
  // {
  //   Serial.printf("%d ", i);
  vTaskDelay(100);
  // }
  // Serial.println();
}
void g5t_loop_v2()
{
  byte request[] = {0x42, 0x4d, 0xe3, 0x00, 0x00, 0x01, 0x72};
  pmsSerial.write(request, sizeof(request)); // Send the request
  Serial.println("Start");
  unsigned long startTime = millis();
  unsigned long timeout = 5000; // Timeout in milliseconds, e.g., 5000ms for 5 seconds

  while (!pmsSerial.readBytes((byte *)&g5t_data_v2, sizeof(g5t_data_v2)))
  {
    if (millis() - startTime >= timeout)
    {
      Serial.println("Timeout occurred!");
      return;
    }
  }
  pmsSerial.end();
  printHex((byte *)&g5t_data_v2, sizeof(g5t_data_v2));
  if (!memcmp((uint8_t *)&g5t_data_v2.data.address, (uint8_t *)&request, sizeof(g5t_data_v2.data.address)) != 0)
  {
    Serial.println("address okay");
    printHex((byte *)&g5t_data_v2.data.address, sizeof(g5t_data_v2.data.address));
    // Serial.printf("version=%d\n", g5t_data_v2.data.version);
    // Serial.printf("errorCode=%d\n", g5t_data_v2.data.errorCode);
    for (int i = 0; i < sizeof(g5t_data_v2) / 2; i++)
    {
      swap_bytes((uint16_t *)(&g5t_data_v2.rawBytes[i]));
    }
    // printHex((byte *)&g5t_data_v2, sizeof(g5t_data_v2));
    if ((g5t_data_v2.data.checksum - raw_sum((byte *)&g5t_data_v2, sizeof(g5t_data_v2))) == 0)
    {
      Serial.println("checksum okay");
    }
    else
    {
      Serial.println("checksum not okay");
      return;
    }
    // Serial.printf("fpm1p0=%d\n", g5t_data_v2.data.fpm1p0);
    // Serial.printf("fpm2p5=%d\n", g5t_data_v2.data.fpm2p5);
    // Serial.printf("fpm10=%d\n", g5t_data_v2.data.fpm10);
    // Serial.printf("pm1p0=%d\n", g5t_data_v2.data.pm1p0);
    Serial.printf("pm2p5=%d\n", g5t_data_v2.data.pm2p5);
    Serial.printf("pm10=%d\n", g5t_data_v2.data.pm10);
    // Serial.printf("um03=%d\n", g5t_data_v2.data.um03);
    // Serial.printf("um05=%d\n", g5t_data_v2.data.um05);
    // Serial.printf("um10=%d\n", g5t_data_v2.data.um10);
    // Serial.printf("um25=%d\n", g5t_data_v2.data.um25);
    // Serial.printf("temperature=%d\n", g5t_data_v2.data.temperature);
    // Serial.printf("humidity=%d\n", g5t_data_v2.data.humidity);
    global_sensor_result.pm2p5 = g5t_data_v2.data.pm2p5;
    global_sensor_result.pm10 = g5t_data_v2.data.pm10;
    global_louver_result.pm2p5 = g5t_data_v2.data.pm2p5;
    global_louver_result.pm10 = g5t_data_v2.data.pm10;
    g5t_fetch_finished = true;
  }
}

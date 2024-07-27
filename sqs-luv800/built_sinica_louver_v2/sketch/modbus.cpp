#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/modbus.cpp"
#include "global.h"

// Modbus RTU 常用设置
#define MODBUS_DEVICE_ID 0x10
#define FUNCTION_CODE_1 0x20
#define FUNCTION_CODE_2 0x21

// 串行通信设置
#define BAUD_RATE 4800
#define SERIAL_CONFIG SERIAL_8N1

struct modbus_response
{
  uint8_t modbus_id;
  uint8_t function_code;
  uint8_t byte_count;                                  // 随后字节数
  uint8_t register_data[sizeof(global_sensor_result)]; // 保持寄存器的值（大端）
  uint8_t version;
  uint16_t crc; // 这将手动附加，不是初始结构的一部分
};

struct modbus_response_v2
{
  uint8_t modbus_id;
  uint8_t function_code;
  uint8_t byte_count;                                  // 随后字节数
  uint8_t register_data[sizeof(global_louver_result)]; // 保持寄存器的值（大端）
  uint8_t version;
  uint16_t crc; // 这将手动附加，不是初始结构的一部分
};

// // 模拟保持寄存器
// #define HOLDING_REGISTERS_SIZE 10
// uint16_t holdingRegisters[HOLDING_REGISTERS_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// 计算 Modbus CRC-16 的函数
uint16_t modbusCRC(uint8_t *buffer, int len)
{
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buffer[pos];
    for (int i = 8; i > 0; i--)
    { // 使用 > 保证循环次数明确
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

// 设置 Modbus RTU 环境
void modbus_init()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Modbus RTU 从设备启动");

  Serial1.begin(BAUD_RATE, SERIAL_CONFIG, RO, DI);
  pinMode(RE_DE, OUTPUT);
  digitalWrite(RE_DE, LOW);
}
void get_sensor_modbus_response()
{
  struct modbus_response response;
  memset(&response, 0, sizeof(response));
  response.modbus_id = MODBUS_DEVICE_ID;
  response.byte_count = sizeof(global_sensor_result) + 1;
  memcpy(response.register_data, &global_sensor_result, sizeof(global_sensor_result));
  response.function_code = FUNCTION_CODE_1;
  response.version = 0x02;
  size_t messageSize = sizeof(response) - sizeof(response.crc);
  uint16_t crc = modbusCRC((uint8_t *)&response, messageSize);
  response.crc = crc; //  crc;
  printHex((byte *)&response, sizeof(response));
}
void get_louver_v2_modbus_response()
{
  struct modbus_response_v2 response_v2;
  memset(&response_v2, 0, sizeof(response_v2));
  response_v2.modbus_id = MODBUS_DEVICE_ID;
  response_v2.byte_count = sizeof(global_louver_result) + 1;
  get_chip_id(global_louver_result.deviceId);
  memcpy(response_v2.register_data, &global_louver_result, sizeof(global_louver_result));
  response_v2.function_code = FUNCTION_CODE_2;
  response_v2.version = 0x02;
  size_t messageSize = sizeof(response_v2) - sizeof(response_v2.crc);
  uint16_t crc = modbusCRC((uint8_t *)&response_v2, messageSize);
  response_v2.crc = crc; //  crc;
  printHex((byte *)&response_v2, sizeof(response_v2));
}

void send_modbus_response(uint8_t function_code)
{
  struct modbus_response response;
  struct modbus_response_v2 response_v2;
  switch (function_code)
  {
  case FUNCTION_CODE_1:
    response.modbus_id = MODBUS_DEVICE_ID;
    response.byte_count = sizeof(global_sensor_result) + 1;
    memcpy(response.register_data, &global_sensor_result, sizeof(global_sensor_result));
    response.function_code = function_code;
    response.version = 0x02;
    // Ensure mutex lock for accessing shared sensor data
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      xSemaphoreGive(xMutex);
      // Calculate the CRC on the response without the CRC field
      size_t messageSize = sizeof(response) - sizeof(response.crc);
      uint16_t crc = modbusCRC((uint8_t *)&response, messageSize);
      response.crc = crc; //  crc;
      // Send the response - assuming Serial1 is set up for Modbus RTU communication
      digitalWrite(RE_DE, HIGH);
      vTaskDelay(1);
      Serial1.write((uint8_t *)&response, sizeof(response));
      Serial1.flush();
      vTaskDelay(10);
      digitalWrite(RE_DE, LOW);
      printHex((byte *)&response, sizeof(response));
    }
    else
    {
      Serial.println("Failed to take the mutex.");
    }
    break;
  case FUNCTION_CODE_2:
    response_v2.modbus_id = MODBUS_DEVICE_ID;
    response_v2.byte_count = sizeof(global_louver_result) + 1;
    get_chip_id(global_louver_result.deviceId);
    memcpy(response_v2.register_data, &global_louver_result, sizeof(global_louver_result));
    response_v2.function_code = function_code;
    response_v2.version = 0x02;
    // Ensure mutex lock for accessing shared sensor data
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
      xSemaphoreGive(xMutex);
      // Calculate the CRC on the response without the CRC field
      size_t messageSize = sizeof(response_v2) - sizeof(response_v2.crc);
      uint16_t crc = modbusCRC((uint8_t *)&response_v2, messageSize);
      response_v2.crc = crc; //  crc;
      // Send the response - assuming Serial1 is set up for Modbus RTU communication
      digitalWrite(RE_DE, HIGH);
      vTaskDelay(1);
      Serial1.write((uint8_t *)&response_v2, sizeof(response_v2));
      Serial1.flush();
      vTaskDelay(10);
      digitalWrite(RE_DE, LOW);
      printHex((byte *)&response_v2, sizeof(response_v2));
    }
    else
    {
      Serial.println("Failed to take the mutex.");
    }
    break;
  default:
    Serial.println("Invalid function code");
    return;
    break;
  }
}

// Modbus RTU 服务器的主循环
void modbus_loop()
{
  if (Serial1.available())
  {
    uint8_t buffer[256];
    size_t length = 0;
    uint32_t lastTime = millis();
    while ((millis() - lastTime < 20) && length < sizeof(buffer))
    {
      if (Serial1.available())
      {
        buffer[length++] = Serial1.read();
        lastTime = millis();
      }
    }
    if (length > 6)
    {
      printHex(buffer, length);
      uint16_t receivedCrc = (buffer[length - 1] & 0xFF) << 8 | (buffer[length - 2] & 0xFF);
      uint16_t calculatedCrc = modbusCRC(buffer, length - 2);
      Serial.print("Received CRC: 0x");
      Serial.println(receivedCrc, HEX);
      Serial.print("Calculated CRC: 0x");
      Serial.println(calculatedCrc, HEX);
      if (calculatedCrc == receivedCrc)
      {
        if (buffer[0] == MODBUS_DEVICE_ID)
        {
          send_modbus_response(buffer[1]);
        }
      }
      else
      {
        Serial.println("CRC Error");
      }
    }
  }
  // send_periodic_message(); // 每秒定时发送消息
  vTaskDelay(1);
}

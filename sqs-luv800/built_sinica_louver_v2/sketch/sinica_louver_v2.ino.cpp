#include <Arduino.h>
#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
#include "global.h"
SemaphoreHandle_t xMutex;
void TaskModbus(void *pvParameters);
// void TaskAtmosphericPressure(void *pvParameters);
void TaskSensor(void *pvParameters);
#line 6 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
void runTasks(void);
#line 26 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
void setup();
#line 51 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
void loop();
#line 6 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
void runTasks(void)
{
  xTaskCreatePinnedToCore(
      TaskModbus, "TaskModbus" // A name just for humans
      ,
      2048 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,
      NULL, ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      TaskSensor, "TaskSensor", 2048 // Stack size
      ,
      NULL, 1 // Priority
      ,
      NULL, ARDUINO_RUNNING_CORE);
}

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  uint64_t chipid = ESP.getEfuseMac();
  memcpy(global_louver_result.deviceId, &chipid, 6);
  // Disconnect from any WiFi networks and stop the WiFi driver
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  Serial.println("WiFi RF function is turned off");

  // Now set up two tasks to run independently.
  runTasks();

  // 创建互斥量
  xMutex = xSemaphoreCreateMutex();

  if (xMutex == NULL)
  {
    Serial.println("无法创建互斥量");
  }
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskSensor(void *pvParameters) // This is a task.
{
  (void)pvParameters;
  // initialize
  sensors_init();

  for (;;) // A Task shall never return or exit.
  {
    sensors_loop();
  }
}

void TaskModbus(void *pvParameters) // This is a task.
{
  (void)pvParameters;
  // initialize
  modbus_init();

  for (;;) // A Task shall never return or exit.
  {
    modbus_loop();
  }
}

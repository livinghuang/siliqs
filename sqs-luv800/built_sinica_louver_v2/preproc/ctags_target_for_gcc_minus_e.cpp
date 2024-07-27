# 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
# 2 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 2
SemaphoreHandle_t xMutex;
void TaskModbus(void *pvParameters);
// void TaskAtmosphericPressure(void *pvParameters);
void TaskSensor(void *pvParameters);
void runTasks(void)
{
  xTaskCreatePinnedToCore(
      TaskModbus, "TaskModbus" // A name just for humans
      ,
      2048 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      
# 13 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
     __null
# 13 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
         , 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,
      
# 15 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
     __null
# 15 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
         , 
# 15 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
           0
# 15 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
                               );

  xTaskCreatePinnedToCore(
      TaskSensor, "TaskSensor", 2048 // Stack size
      ,
      
# 20 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
     __null
# 20 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
         , 1 // Priority
      ,
      
# 22 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
     __null
# 22 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
         , 
# 22 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
           0
# 22 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
                               );
}

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial0.begin(115200);
  uint64_t chipid = ESP.getEfuseMac();
  memcpy(global_louver_result.deviceId, &chipid, 6);
  // Disconnect from any WiFi networks and stop the WiFi driver
  WiFi.disconnect(true);
  WiFi.mode(WIFI_MODE_NULL);

  Serial0.println("WiFi RF function is turned off");

  // Now set up two tasks to run independently.
  runTasks();

  // 创建互斥量
  xMutex = xQueueCreateMutex( ( ( uint8_t ) 1U ) );

  if (xMutex == 
# 44 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino" 3 4
               __null
# 44 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/sinica_louver_v2.ino"
                   )
  {
    Serial0.println("无法创建互斥量");
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

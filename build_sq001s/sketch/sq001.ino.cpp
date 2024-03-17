#include <Arduino.h>
#line 1 "/Users/living/code/github/siliqs/sq001/sq001.ino"
#include "global.h"
// Dps310 Opject
bool resendflag = false;
bool deepsleepflag = false;

RTC_DATA_ATTR int bootCount = 0;
uint64_t chipid;
test_status_t test_status;

#line 10 "/Users/living/code/github/siliqs/sq001/sq001.ino"
void setup();
#line 28 "/Users/living/code/github/siliqs/sq001/sq001.ino"
void loop();
#line 10 "/Users/living/code/github/siliqs/sq001/sq001.ino"
void setup()
{
  Serial.begin(115200);
  Mcu.begin();

  print_wakeup_reason();
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  resendflag = false;
  deepsleepflag = false;

  get_chip_id();
  test_status = LORA_INTVAL_SEND_TEST;
}
bool lora_receive_test_init = true;
bool lora_to_lorawan_repeater_first_init = true;
bool pt100_test_init = true;
void loop()
{
  switch (test_status)
  {
  case WIFI_CONNECT_TEST_INIT:
  {
    wifi_connect_init();
    test_status = WIFI_CONNECT_TEST;
  }
  case WIFI_CONNECT_TEST:
  {
    if (wifi_connect_try(1) == true)
    {
      Serial.println("wifi connect OK");
    }
    wifi_connect_try_num--;
    break;
  }
  case WIFI_SCAN_TEST:
  {
    wifi_scan(1);
    break;
  }
  case LORAWAN_TEST_INIT:
  {
    lorawan_init();
    test_status = LORAWAN_COMMUNICATION_TEST;
    delay(1);
    break;
  }
  case LORAWAN_COMMUNICATION_TEST:
  {
    lorawan_process();
    delay(1);
    break;
  }
  case LORA_TO_LORAWAN_REPEATER_TEST:
  {
    if (lora_to_lorawan_repeater_first_init == true)
    {
      lora_to_lorawan_repeater_init();
      lora_to_lorawan_repeater_first_init = false;
    }
    lora_to_lorawan_repeater_process();
    break;
  }
  case LORA_TEST_INIT:
  {
    Serial.println("LORA_TEST_INIT");
    Serial.flush();
    lora_init();
    test_status = LORA_COMMUNICATION_TEST;
    delay(1);
    break;
  }
  case LORA_COMMUNICATION_TEST:
  {
    Serial.println("LORA_COMMUNICATION_TEST");
    Serial.flush();
    lora_status_handle();
    delay(1000);
    break;
  }
  case LORA_INTVAL_SEND_TEST:
  {
    lora_interval_send(60);
    break;
  }
  case LORA_CONTINUOUS_RECEIVE_TEST:
  {
    if (lora_receive_test_init == true)
    {
      lora_init();
      lora_receive_test_init = false;
    }
    lora_continuous_receive();
    break;
  }
  case HDC1080_TEST_INIT:
  {
    power_On_Sensor_Bus();
    delay(5);
    test_status = HDC1080_TEST;
    break;
  }
  case HDC1080_TEST:
  {
    hdc1080_fetch();
    delay(1000);
    break;
  }
  case BMP280_TEST_INIT:
  {
    power_On_Sensor_Bus();
    delay(5);
    test_status = BMP280_TEST;
    break;
  }
  case BMP280_TEST:
  {
    bmp280_fetch();
    delay(1000);
    break;
  }
  case DSP310_TEST_INIT:
  {
    power_On_Sensor_Bus();
    delay(5);
    Wire.setPins(pSDA, pSCL);
    // Call begin to initialize Dps310PressureSensor
    // The parameter 0x76 is the bus address. The default address is 0x77 and does not need to be given.
    // Dps310PressureSensor.begin(Wire, 0x76);
    // Use the commented line below instead of the one above to use the default I2C address.
    // if you are using the Pressure 3 click Board, you need 0x76
    Dps310PressureSensor.begin(Wire, 0x77);
    Serial.println("Init complete!");
    test_status = DSP310_TEST;
    break;
  }
  case DSP310_TEST:
  {
    dsp310_fetch();
    break;
  }
  case PT100_TEST:
  {
    if (pt100_test_init == true)
    {
      pt100_init();
      pt100_test_init = false;
    }
    pt100_fetch();
    delay(1000);
    test_status = PT100_TEST;
    break;
  }
  case SENSOR_TEST:
  {
    pinMode(pLED, OUTPUT);
    power_On_Sensor_Bus();
    delay(5);
    hdc1080_fetch();
    delay(5);
    hdc1080_fetch();
    delay(1000);
    Wire.setPins(pSDA, pSCL);
    Dps310PressureSensor.begin(Wire, 0x77);
    Serial.println("Init complete!");
    dsp310_fetch();
    Wire.end();
    power_Off_Sensor_Bus();
    delay(1000);
    break;
  }
  case DEEPSLEEP_BY_GPIO_TEST:
  {
    delay(1000);
    // #define DEEPSLEEP_BY_GPIO_TEST_PIN 4
    // #define SX1262_RST 5
    // pinMode(DEEPSLEEP_BY_GPIO_TEST_PIN, INPUT);
    // pinMode(SX1262_RST, OUTPUT);
    // digitalWrite(SX1262_RST, LOW);
    // esp_deep_sleep_enable_gpio_wakeup(1 << DEEPSLEEP_BY_GPIO_TEST_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
    esp_deep_sleep_start();
    break;
  }
  case DEEPSLEEP_BY_TIMER_TEST:
  {
    enter_deep_sleep(10);
    break;
  }

  case LED_INIT:
  {
    led_init();
    test_status = LED_TEST;
    break;
  }
  case LED_TEST:
  {
    digitalWrite(pLED, HIGH);
    delay(1000);
    digitalWrite(pLED, LOW);
    delay(1000);
    break;
  }
  case SPI_TEST:
  {
#define SX1262_RST 5
    pinMode(SX1262_RST, OUTPUT);
    digitalWrite(SX1262_RST, LOW);
    SPI.begin(pSCK, pMISO, pMOSI, pCS);
    SPI.transfer(0x00);
    delay(1000);
    break;
  }
  case SD_TEST:
  {
    extern bool sdInitialized;
    if (!sdInitialized)
    {
      sd_init();
    }
    else
    {
      sd_test();
    }
    delay(1000);
    break;
  }
  case RS485_INIT:
  {
    rs485_init();
    test_status = RS485_TEST;
    break;
  }
  case RS485_TEST:
  {
    rs485_status_handle();
    break;
  }
  case BAT_TEST:
  {
    float batteryVoltage = getBatteryVoltage();
    String battery_power = "BAT: " + (String)batteryVoltage;
    getBatteryLevel();
    delay(1000);
    break;
  }
  default:
    break;
  }
}


#line 1 "/Users/living/code/github/siliqs/sq001/bsp.cpp"
#include "global.h"
void power_On_Sensor_Bus()
{
  pinMode(pVext, OUTPUT);
  pinMode(pSDA, OUTPUT);
  pinMode(pSCL, OUTPUT);
  digitalWrite(pSDA, HIGH);
  digitalWrite(pSCL, HIGH);
  delay(5);
  digitalWrite(pVext, LOW);
}

void power_Off_Sensor_Bus()
{
  pinMode(pVext, OUTPUT);
  pinMode(pSDA, OUTPUT);
  pinMode(pSCL, OUTPUT);
  digitalWrite(pSDA, LOW);
  digitalWrite(pSCL, LOW);
  digitalWrite(pVext, HIGH);
}

void set_radio_off(void)
{
  Radio.Sleep();
  SPI.end();
  pinMode(RADIO_DIO_1, ANALOG);
  pinMode(RADIO_NSS, ANALOG);
  pinMode(RADIO_RESET, ANALOG);
  pinMode(RADIO_BUSY, ANALOG);
  pinMode(LORA_CLK, ANALOG);
  pinMode(LORA_MISO, ANALOG);
  pinMode(LORA_MOSI, ANALOG);
}

void enter_deep_sleep(uint64_t wake_up_after_secs)
{
  set_radio_off();
  esp_sleep_enable_timer_wakeup(wake_up_after_secs * 1000 * 1000);
  esp_deep_sleep_start();
}
#include "global.h"
static void prepareTxFrame(uint8_t port);

void lora_to_lorawan_repeater_init(void)
{
#if (LORAWAN_DEVEUI_AUTO)
  LoRaWAN.generateDeveuiByChipID();
#endif
  // Mcu.begin();
  static bool lora_receive_test_init = true;
  if (lora_receive_test_init == true)
  {
    appTxDutyCycle = 10000;
    lora_init();
    lora_receive_test_init = false;
  }
  while (1)
  {
    lora_continuous_receive();
    extern bool receiveflag;
    if (receiveflag)
    {
      break;
    }
  }

  deviceState = DEVICE_STATE_INIT;
}

void lora_to_lorawan_repeater_process(void)
{
  switch (deviceState)
  {
  case DEVICE_STATE_INIT:
  {
    LoRaWAN.init(loraWanClass, loraWanRegion);
    break;
  }
  case DEVICE_STATE_JOIN:
  {
    LoRaWAN.join();
    break;
  }
  case DEVICE_STATE_SEND:
  {
    prepareTxFrame(appPort);
    LoRaWAN.send();
    deviceState = DEVICE_STATE_CYCLE;
    break;
  }
  case DEVICE_STATE_CYCLE:
  {
    // Schedule next packet transmission
    txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
    LoRaWAN.cycle(txDutyCycleTime);
    deviceState = DEVICE_STATE_SLEEP;
    break;
  }
  case DEVICE_STATE_SLEEP:
  {
    LoRaWAN.sleep(loraWanClass);
    break;
  }
  default:
  {
    deviceState = DEVICE_STATE_INIT;
    break;
  }
  }
}

static void prepareTxFrame(uint8_t port)
{
  extern char rxpacket[];
  extern int16_t rxSize;
  memcpy(appData, rxpacket, rxSize);
  appDataSize = rxSize;
  printHex(appData, appDataSize);
}

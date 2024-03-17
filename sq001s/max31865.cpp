/*************************************************** 
This is a library is modify from the Adafruit 
PT100/P1000 RTD Sensor w/MAX31865 to use on Siliq platform
----------------------------------------------------------------
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865
  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328
  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
 ****************************************************/
#include "max31865.h"

Adafruit_MAX31865 thermo = Adafruit_MAX31865(pCS, pMOSI, pMISO, pSCK);

void max31865_init()
{
  extern RadioEvents_t RadioEvents;
  Radio.Init(&RadioEvents);
  Radio.Sleep();
  SPI.end();

  extern RadioEvents_t RadioEvents;
  Radio.Init(&RadioEvents);
  Radio.Sleep();
  SPI.end();

  pinMode(RADIO_DIO_1, ANALOG);
  pinMode(RADIO_NSS, ANALOG);
  pinMode(RADIO_RESET, ANALOG);
  pinMode(RADIO_BUSY, ANALOG);
  pinMode(LORA_CLK, ANALOG);
  pinMode(LORA_MISO, ANALOG);
  pinMode(LORA_MOSI, ANALOG);

  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, HIGH);
  pinMode(RADIO_RESET, OUTPUT);
  digitalWrite(RADIO_RESET, LOW);
  delay(10);
  thermo.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
}

void max31865_process()
{
  uint16_t rtd = thermo.readRTD();
  float temperature;
  Serial.print("RTD value: ");
  Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  Serial.print("Ratio = ");
  Serial.println(ratio, 8);
  Serial.print("Resistance = ");
  Serial.println(RREF * ratio, 8);
  Serial.print("Temperature = ");
  temperature=thermo.temperature(RNOMINAL, RREF);
  Serial.println(temperature);

  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault)
  {
    Serial.print("Fault 0x");
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH)
    {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH)
    {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW)
    {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH)
    {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW)
    {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV)
    {
      Serial.println("Under/Over voltage");
    }
    thermo.clearFault();
  }else{
    global_sensor_result.PT100 = temperature;
  }
  Serial.println();
  delay(1000);
}

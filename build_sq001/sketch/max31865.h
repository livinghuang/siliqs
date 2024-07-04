#line 1 "/Users/living/code/github/siliqs/sq001s/max31865.h"
#ifndef MAX31865_H
#define MAX31865_H
#include "global.h"
#include <Adafruit_MAX31865.h>


// use hardware SPI, just pass in the CS pin
// Adafruit_MAX31865 thermo = Adafruit_MAX31865(10);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF 400.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL 100.0
extern Adafruit_MAX31865 thermo;
void max31865_init(void);
void max31865_process(void);
#endif
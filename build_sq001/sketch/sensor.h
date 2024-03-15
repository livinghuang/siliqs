#line 1 "/Users/living/code/github/siliqs/sq001/sensor.h"
#define _SENSOR_H
#ifdef _SENSOR_H
#include "global.h"

extern BMP280 bmp;
extern HDC1080 hdc1080;
extern Dps310 Dps310PressureSensor;

bool hdc1080_fetch(void);
bool bmp280_fetch(void);
void dsp310_fetch(void);
void pt100_fetch(void);
void pt100_init(void);
void fetchSensorData(void);
#endif
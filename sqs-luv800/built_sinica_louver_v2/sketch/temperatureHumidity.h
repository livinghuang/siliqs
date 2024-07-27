#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/temperatureHumidity.h"
#ifndef TH_H
#define TH_H

#include "global.h"
#include "HDC1080.h"
extern HDC1080 hdc1080;
extern bool hdc1080_fetch_finished;
void temperatureHumidity_init();
void temperatureHumidity_loop();
#endif
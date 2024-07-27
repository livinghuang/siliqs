#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/atmosphericPressure.h"
#ifndef AP_H
#define AP_H

#include "global.h"
#include "Dps310.h"
#include "DpsClass.h"
extern Dps310 Dps310PressureSensor;
extern bool dsp310_fetch_finished;
void atmosphericPressure_init();
void atmosphericPressure_loop();
#endif
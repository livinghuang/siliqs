#line 1 "/Users/living/code/github/siliqs/sq001s/_wifi.h"
#ifndef WIFI_H
#define WIFI_H
#include "global.h"

extern uint16_t wifi_connect_try_num;

void wifi_connect_init(void);
bool wifi_connect_try(uint8_t try_num);
void wifi_scan(unsigned int value);
#endif
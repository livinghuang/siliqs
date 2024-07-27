#line 1 "/Users/living/code/github/siliqs/sqs-luv800/sinica_louver_v2/modbus.h"
#ifndef MODBUS_H
#define MODBUS_H

void modbus_init();
void modbus_loop();
uint16_t modbusCRC(uint8_t *buffer, int len);
void get_sensor_modbus_response();
void get_louver_v2_modbus_response();
#endif
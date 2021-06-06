#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <blehid.h>
#include "config.h"
#include "utils.h"
#include "esp32-hal.h"

#pragma pack(push, 1)
typedef struct
{
    uint16_t buttons;
    uint16_t triggers;
    int16_t axis_data[4];
} joy_data_t;
typedef struct
{
    int16_t mag_data[3];
    int16_t accel_data[3];
    int16_t gyro_data[3];
} sens_data_t;
typedef struct
{
    joy_data_t data;
    sens_data_t sens;
    uint32_t crc;
} report_t;
#pragma pack(pop)

void stm32_uart_init(report_t *preport);
int stm32_uart_process();

#endif
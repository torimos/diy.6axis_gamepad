#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <EEPROM.h>
#include <usb_hid.h>
#include "config.h"
#include "utils.h"

#include <gyro.h>
#include <magn.h>
#include <accel.h>

#pragma pack(push, 1)
typedef struct
{
    uint32_t buttons;
    int16_t axis_data[4];
    int16_t mag_data[3];
    int16_t accel_data[3];
    int16_t gyro_data[3];
} joy_data_t;
typedef struct
{
    uint8_t id;
    uint8_t status;
    joy_data_t data;
} usb_report_t;
typedef struct
{
    uint32_t header;
    uint16_t data_type;
    uint16_t data_size;
    joy_data_t data;
    uint32_t crc;
} uart_report_t;
typedef struct
{
    int16_t xmin, xmax, ymin, ymax, xoffs, yoffs;
} stick_calib_t;
typedef struct
{
    uint8_t scale_enabled;
    uint8_t calib_enabled;
    uint8_t uart_adapter_enabled;
    uint16_t scale_from;
    uint16_t scale_to;
    stick_calib_t calib_left;
    stick_calib_t calib_right;
} settings_t;

typedef struct
{
    uint8_t id;
    settings_t data;
} usb_params_report_t;
#pragma pack(pop)


extern ITG3200 itg;
extern HMC5883L mag;
extern ADXL345 acc;

extern settings_t settings;

void io_init();
bool sensors_init();
bool settings_init();

void eeprom_read_data(int pos, void* data, int len);
void eeprom_write_data(int pos, void* data, int len);

uint32_t get_CRC32(uint8_t *data, int16_t len);

void leds_init();
void leds_anim(uint8_t pattern1, uint8_t pattern2, int ms = 250, int timeout = 60000);
void leds_set(uint8_t state);
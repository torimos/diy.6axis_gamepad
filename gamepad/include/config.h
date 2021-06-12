#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

//MCU: STM32F103RBT6
// prog pins: Yellow Red Brown (L2R), Blue to GND

#define PWR_CTRL        PD2
#define LED1            PC0
#define LED2            PC1
#define LED3            PC2
#define LED4            PC3
#define MOTOR1          PB8
#define MOTOR2          PB9

#define CHRG            PC9
#define JADC_X_LEFT     PA0
#define JADC_Y_LEFT     PA1
#define JADC_X_RIGHT    PA2
#define JADC_Y_RIGHT    PA3
#define JSW_LEFT        PA4
#define JSW_RIGHT       PA5
#define MENU            PB12
#define SELECT          PB11
#define START           PB10
#define TRIANGLE        PC4
#define CIRCLE          PC5
#define FORK            PB0
#define SQUARE          PB1
#define LEFT_TUP        PC7
#define LEFT_TDOWN      PC8
#define RIGHT_TUP       PA7
#define RIGHT_TDOWN     PA6
#define UP              PC6
#define DOWN            PB14
#define LEFT            PB15
#define RIGHT           PB13

#define EXT1            PC12
#define EXT2            PA8

#define UART_REPORT_POOL_INTERVAL 5
#define SLEEP_TIMEOUT 5000

typedef struct
{
    int xmin, xmax, ymin, ymax, xoffs, yoffs;
} stick_calib_t;


#ifdef __cplusplus
}
#endif


#endif
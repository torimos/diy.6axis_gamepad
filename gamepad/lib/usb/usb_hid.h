#ifndef __USB_HID_H
#define __USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "usb.h"

#define REPORT_BUF_SIZE 64
#define USB_REPORT_POOL_INTERVAL 20

enum
{
	REPORT_ID_JOY = 1,
	REPORT_ID_PARAM,
	REPORT_ID_FIRMWARE,
};

typedef enum {
    HID_IDLE = 0,
    HID_BUSY,
}
HID_StateTypeDef;

typedef struct {
    uint8_t  Report_buf[REPORT_BUF_SIZE];
    uint8_t  OutReport_buf[REPORT_BUF_SIZE];
    uint32_t Protocol;
    uint32_t IdleState;
    uint32_t AltSetting;
    HID_StateTypeDef State;
    bool IsReportAvailable;
} HID_Class_DataTypeDef;

extern USBD_ClassTypeDef  USBD_HID_Class;

uint8_t HID_Device_SendCtrlReport(USBD_HandleTypeDef  *pdev, uint8_t *report, uint16_t len);
uint8_t HID_Device_SendReport(USBD_HandleTypeDef  *pdev, uint8_t *report, uint16_t len);
extern void HID_Device_ReceiveReport(uint8_t epnum, uint8_t *data);
extern bool HID_Device_GetReport(uint8_t id);
uint8_t HID_Device_PrepareReceive(USBD_HandleTypeDef  *pdev, uint8_t epnum);

#ifdef __cplusplus
}
#endif

#endif
#ifndef __USB_H
#define __USB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "usbd_def.h"


USBD_HandleTypeDef* USB_Init(USBD_ClassTypeDef *pclass);
void USB_DeInit();

#ifdef __cplusplus
}
#endif

#endif
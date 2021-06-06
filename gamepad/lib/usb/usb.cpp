#include "usb.h"
#include "usbd_def.h"
#include "usbd_ioreq.h"
#include "utils.h"

#define USB_DEV_VID                             0x0483
#define USB_DEV_PID                             0xBEAF
#define USB_DEVICE_ID1                          (UID_BASE)
#define USB_DEVICE_ID2                          (UID_BASE+0x4)
#define USB_DEVICE_ID3                          (UID_BASE+0x8)

#define USB_DEVICE_SIZ_STRING_SERIAL            0x1A
#define USB_DEVICE_LANGID_STRING                0x409   /* 1033 US.S English */
#define USB_DEVICE_MANUFACTURER_STRING          "STMicroelectronics"
#define USB_DEVICE_NAME_STRING                  "6Axis"
#define USB_DEVICE_CLASS_PRODUCT_STRING         CONCATS(USB_DEVICE_NAME_STRING, "Gamepad")
#define USB_DEVICE_CLASS_CONFIGURATION_STRING   CONCATS(USB_DEVICE_NAME_STRING, "Config")
#define USB_DEVICE_CLASS_INTERFACE_STRING       CONCATS(USB_DEVICE_NAME_STRING, "Interface")

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USB_Device_Class_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12,                       /* bLength */
    USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
    0x00, 0x02,                 /* bcdUSB 2.00*/
    0x00,                       /* bDeviceClass (Use class information in the Interface Descriptors) */
    0x00,                       /* bDeviceSubClass */
    0x00,                       /* bDeviceProtocol */
    USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
    LOBYTE(USB_DEV_VID),           /* idVendor */
    HIBYTE(USB_DEV_VID),           /* idVendor */
    LOBYTE(USB_DEV_PID),           /* idProduct */
    HIBYTE(USB_DEV_PID),           /* idProduct */
    0x00, 0x02,                 /* bcdDevice rel. 1.00 */
    USBD_IDX_MFC_STR,           /* Index of manufacturer string */
    USBD_IDX_PRODUCT_STR,       /* Index of product string */
    USBD_IDX_SERIAL_STR,        /* Index of serial number string */
    USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
__ALIGN_BEGIN   uint8_t USB_Device_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
    USB_LEN_LANGID_STR_DESC,
    USB_DESC_TYPE_STRING,
    LOBYTE(USB_DEVICE_LANGID_STRING),
    HIBYTE(USB_DEVICE_LANGID_STRING),
};

uint8_t USB_Device_StringSerial[USB_DEVICE_SIZ_STRING_SERIAL] = {
    USB_DEVICE_SIZ_STRING_SERIAL,
    USB_DESC_TYPE_STRING,
    0,
};

__ALIGN_BEGIN uint8_t USB_Device_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

uint8_t *USB_Dev_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USB_Dev_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USB_Dev_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USB_Dev_Class_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USB_Dev_Class_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USB_Dev_Class_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USB_Dev_Class_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

USBD_DescriptorsTypeDef USBD_HID_Desc = {
    USB_Dev_Class_DeviceDescriptor,
    USB_Dev_LangIDStrDescriptor,
    USB_Dev_ManufacturerStrDescriptor,
    USB_Dev_Class_ProductStrDescriptor,
    USB_Dev_SerialStrDescriptor,
    USB_Dev_Class_ConfigStrDescriptor,
    USB_Dev_Class_InterfaceStrDescriptor,
};

USBD_HandleTypeDef hUSBD_HID;

USBD_HandleTypeDef* USB_Init(USBD_ClassTypeDef *pclass)
{
    /* Init Device Library */
    if (USBD_Init(&hUSBD_HID, &USBD_HID_Desc, 0) == USBD_OK) {
        /* Add Supported Class */
        if (USBD_RegisterClass(&hUSBD_HID, pclass) == USBD_OK) {
        /* Start Device Process */
            USBD_Start(&hUSBD_HID);
            return &hUSBD_HID;
        }
    }
    return NULL;
}

void USB_DeInit()
{
    USBD_Stop(&hUSBD_HID);
    USBD_DeInit(&hUSBD_HID);
}

/* Private members */

static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);
static void Get_SerialNum(void);

uint8_t *USB_Dev_Class_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USB_Device_Class_DeviceDesc);
    return (uint8_t *)USB_Device_Class_DeviceDesc;
}

uint8_t *USB_Dev_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USB_Device_LangIDDesc);
    return (uint8_t *)USB_Device_LangIDDesc;
}

uint8_t *USB_Dev_Class_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString((uint8_t *)USB_DEVICE_CLASS_PRODUCT_STRING, USB_Device_StrDesc, length);
    return USB_Device_StrDesc;
}

uint8_t *USB_Dev_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString((uint8_t *)USB_DEVICE_MANUFACTURER_STRING, USB_Device_StrDesc, length);
    return USB_Device_StrDesc;
}

uint8_t *USB_Dev_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = USB_DEVICE_SIZ_STRING_SERIAL;
    Get_SerialNum(); /* Update the serial number string descriptor with the data from the unique ID*/
    return (uint8_t *)USB_Device_StringSerial;
}

uint8_t *USB_Dev_Class_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString((uint8_t *)USB_DEVICE_CLASS_CONFIGURATION_STRING, USB_Device_StrDesc, length);
    return USB_Device_StrDesc;
}

uint8_t *USB_Dev_Class_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString((uint8_t *)USB_DEVICE_CLASS_INTERFACE_STRING, USB_Device_StrDesc, length);
    return USB_Device_StrDesc;
}

static void Get_SerialNum(void)
{
    uint32_t deviceserial0, deviceserial1, deviceserial2;

    deviceserial0 = *(uint32_t *)USB_DEVICE_ID1;
    deviceserial1 = *(uint32_t *)USB_DEVICE_ID2;
    deviceserial2 = *(uint32_t *)USB_DEVICE_ID3;

    deviceserial0 += deviceserial2;

    if (deviceserial0 != 0U) {
        IntToUnicode(deviceserial0, &USB_Device_StringSerial[2], 8U);
        IntToUnicode(deviceserial1, &USB_Device_StringSerial[18], 4U);
    }
}

static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
    uint8_t idx = 0U;
    for (idx = 0U ; idx < len ; idx ++) {
        if (((value >> 28)) < 0xAU) {
            pbuf[ 2U * idx] = (value >> 28) + '0';
        } else {
            pbuf[2U * idx] = (value >> 28) + 'A' - 10U;
        }
        value = value << 4;
        pbuf[2U * idx + 1] = 0U;
    }
}
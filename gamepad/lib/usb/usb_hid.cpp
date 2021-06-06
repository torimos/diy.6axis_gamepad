#include "usb_hid.h"
#include "usbd_desc.h"
#include "usbd_ioreq.h"

#define HID_DEVICE_INTERFACE_INDEX          0x00U
#define HID_DEVICE_INTERFACE_SIZ            0x09U
#define HID_DEVICE_DESC_SIZ                 0x09U
#define HID_DEVICE_DESCRIPTOR_TYPE          0x21U
#define HID_DEVICE_REPORT_DESCRIPTOR_TYPE   0x22U
#define HID_DEVICE_EPIN_ADDR                0x81U
#define HID_DEVICE_EPOUT_ADDR               0x01U
#define HID_DEVICE_EPIN_SIZE                64U
#define HID_DEVICE_EPOUT_SIZE               64U
#define HID_REQ_SET_PROTOCOL                0x0BU
#define HID_REQ_GET_PROTOCOL                0x03U
#define HID_REQ_SET_IDLE                    0x0AU
#define HID_REQ_GET_IDLE                    0x02U
#define HID_REQ_SET_REPORT                  0x09U
#define HID_REQ_GET_REPORT                  0x01U

/* USB HID device FS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t HID_Device_CfgFSDesc[]  __ALIGN_END = {
    /************** Descriptor of Configuration ****************/
    /* 00 */
    0x09,                                       /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION,                /* bDescriptorType: Configuration */
    LOBYTE(0),                                  /* wTotalLength: Bytes returned */
    HIBYTE(0),
    0x01,                                       /*bNumInterfaces: 1 interface*/
    0x01,                                       /*bConfigurationValue: Configuration value*/
    0x00,                                       /*iConfiguration: Index of string descriptor describing the configuration*/
    0x80,                                       /*bmAttributes: 0x80 - bus powered, 0xC0 bus powered and no Support Remote Wake-up */
    0x32,                                       /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick HID Device interface ****************/
    /* 09 */
    0x09,                                       /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE,                    /*bDescriptorType: Interface descriptor type*/
    HID_DEVICE_INTERFACE_INDEX,                 /*bInterfaceNumber: Number of Interface*/
    0x00,                                       /*bAlternateSetting: Alternate setting*/
    0x01,                                       /*bNumEndpoints*/
    0x03,                                       /*bInterfaceClass: HID*/
    0x00,                                       /*bInterfaceSubClass : 0=no boot, 1=BOOT*/
    0x00,                                       /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0x00,                                       /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick HID Device ********************/
    /* 18 */
    HID_DEVICE_DESC_SIZ,                        /*bLength: HID Descriptor size*/
    HID_DEVICE_DESCRIPTOR_TYPE,                 /*bDescriptorType: HID*/
    0x11, 0x01,                                 /*bcdHID 1.11: HID Class Spec release number*/
    0x00,                                       /*bCountryCode: Hardware target country*/
    0x01,                                       /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,                                       /*bDescriptorType*/
    LOBYTE(0),                                  /*wItemLength: Total length of Report descriptor*/
    HIBYTE(0),
    /******************** Descriptor of Joystick HID Device endpoint ********************/
    /* 27 */
    0x07,                                       /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT,                     /*bDescriptorType:*/
    HID_DEVICE_EPIN_ADDR,                       /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                                       /*bmAttributes: Interrupt endpoint*/
    LOBYTE(HID_DEVICE_EPIN_SIZE),               /*wMaxPacketSize*/
    HIBYTE(HID_DEVICE_EPIN_SIZE), 
    USB_REPORT_POOL_INTERVAL,                       /*bInterval: Polling Interval T, where (2^(T-1)=Tx micro-frames*/
    /* 34 */
    // 0x07,                                       /*bLength: Endpoint Descriptor size*/
    // USB_DESC_TYPE_ENDPOINT,                     /*bDescriptorType:*/
    // HID_DEVICE_EPOUT_ADDR,	                    /*bEndpointAddress: Endpoint Address (OUT)*/
    // 0x03,                                       /*bmAttributes: Interrupt endpoint*/
    // LOBYTE(HID_DEVICE_EPOUT_SIZE),               /*wMaxPacketSize*/
    // HIBYTE(HID_DEVICE_EPOUT_SIZE), 
    // 0x5,	                                    /* bInterval: Polling Interval */
    // /* 41 */
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t HID_DeviceQualifierDesc[]  __ALIGN_END = {
    USB_LEN_DEV_QUALIFIER_DESC,         //   bLength
    USB_DESC_TYPE_DEVICE_QUALIFIER,     //   bDescriptorType (Device Qualifier)
    0x00, 0x02,                         //   bcdUSB 2.00
    0x00,                               //   bDeviceClass (Use class information in the Interface Descriptors)
    0x00,                               //   bDeviceSubClass 
    0x00,                               //   bDeviceProtocol
    0x40,                               //   bMaxPacketSize0 64
    0x01,                               //   bNumConfigurations 1
    0x00,                               //   bReserved
};

__ALIGN_BEGIN static uint8_t HID_Device_ReportDesc[]  __ALIGN_END = {
    0x05, 0x01,             // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,             // USAGE (Joystick)
    0xA1, 0x01,             // COLLECTION (Application)
    0x85, REPORT_ID_JOY,	//   REPORT_ID	(usb_report_ID)	
    // Buttons section
    0x05, 0x09,             //   USAGE_PAGE (Button)		
    0x19, 0x01,             //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,             //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x25, 0x01,             //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,             //   REPORT_SIZE (1bit)
    0x95, 0x10,             //   REPORT_COUNT (16)
    0x81, 0x02,             //   INPUT (Data, Var, Abs)
    // Triggers section
    0x75, 0x08,             //   REPORT_SIZE (8bit)
    0x15, 0x00,             //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,       //   LOGICAL_MAXIMUM (255)
    0x35, 0x00,             //   PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,       //   PHYSICAL_MAXIMUM (255)
    0x95, 0x02,             //   REPORT_COUNT (2)
    0x05, 0x01,             //   USAGE_PAGE (Generic Desktop)
    0x09, 0x32,             //   USAGE (Z)
    0x09, 0x35,             //   USAGE (Rz)
    0x81, 0x02,             //   INPUT (Data,Var,Abs)
    // Left pointer section
    0x75, 0x10,             //   REPORT_SIZE (16bit)
    0x16, 0x00, 0x80,       //   LOGICAL_MINIMUM (-32768)
    0x26, 0xff, 0x7f,       //   LOGICAL_MAXIMUM (32767)
    0x36, 0x00, 0x80,       //   PHYSICAL_MINIMUM (-32768)
    0x46, 0xff, 0x7f,       //   PHYSICAL_MAXIMUM (32767)
    0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
    0x09, 0x01,             //   USAGE (Pointer)
    0xa1, 0x00,             //   COLLECTION (Physical)
    0x95, 0x02,             //     REPORT_COUNT (2)
    0x09, 0x30,             //     USAGE (X)
    0x09, 0x31,             //     USAGE (Y)
    0x81, 0x02,             //     INPUT (Data,Var,Abs)
    0xC0,                   //   END COLLECTION
    // Right pointer section
    0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
    0x09, 0x01,             //   USAGE (Pointer)
    0xa1, 0x00,             //   COLLECTION (Physical)
    0x95, 0x02,             //     REPORT_COUNT (2)
    0x09, 0x33,             //     USAGE (Rx)
    0x09, 0x34,             //     USAGE (Ry)
    0x81, 0x02,             //     INPUT (Data,Var,Abs)
    0xC0,                   //   END COLLECTION
    // Magn section
    0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
    0x09, 0x01,             //   USAGE (Pointer)
    0xa1, 0x00,             //   COLLECTION (--)
    0x95, 0x03,             //     REPORT_COUNT (3)
    0x09, 0x40,             //     USAGE (Vx)
    0x09, 0x41,             //     USAGE (Vy)
    0x09, 0x42,             //     USAGE (Vz)
    0x81, 0x02,             //     INPUT (Data,Var,Abs)
    0xC0,                   //   END COLLECTION
     // Accel section
    0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
    0x09, 0x01,             //   USAGE (Pointer)
    0xa1, 0x00,             //   COLLECTION (--)
    0x95, 0x03,             //     REPORT_COUNT (3)
    0x09, 0x40,             //     USAGE (Vx)
    0x09, 0x41,             //     USAGE (Vy)
    0x09, 0x42,             //     USAGE (Vz)
    0x81, 0x02,             //     INPUT (Data,Var,Abs)
    0xC0,                   //   END COLLECTION
     // Gyro section
    0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
    0x09, 0x01,             //   USAGE (Pointer)
    0xa1, 0x00,             //   COLLECTION (--)
    0x95, 0x03,             //     REPORT_COUNT (3)
    0x09, 0x40,             //     USAGE (Vx)
    0x09, 0x41,             //     USAGE (Vy)
    0x09, 0x42,             //     USAGE (Vz)
    0x81, 0x02,             //     INPUT (Data,Var,Abs)
    0xC0,                   //   END COLLECTION
    // Control report from host to device section
    0x09, 0x02,             //   USAGE (Vendor Usage 2)
    0x85, REPORT_ID_PARAM,  //   REPORT_ID	(--)
    0x75, 0x08,             //   REPORT_SIZE (8)
    0x95, 0x3F,             //   REPORT_COUNT (63+1)
    0x91, 0x00,             //   OUTPUT (Data,Ary,Abs)
    0xC0                    // END COLLECTION
};

static uint8_t  HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  *HID_GetFSCfgDesc(uint16_t *length);
static uint8_t  *HID_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t  HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t HID_EP0_RxReady(USBD_HandleTypeDef *pdev);

USBD_ClassTypeDef  USBD_HID_Class = {
    HID_Init,
    HID_DeInit,
    HID_Setup,
    NULL, /*EP0_TxSent*/
    HID_EP0_RxReady, /*EP0_RxReady*/
    HID_DataIn,
    HID_DataOut,
    NULL, /*SOF*/
    NULL, /*IsoINIncomplete*/
    NULL, /*IsoOUTIncomplete*/
    NULL, /*GetHSConfigDescriptor*/
    HID_GetFSCfgDesc,
    NULL, /*GetHSDeviceQualifierDescriptor*/
    HID_GetDeviceQualifierDesc,
};

static uint8_t HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    UNUSED(cfgidx);
    uint8_t ret = USBD_OK;

    /* Open EP IN */
    USBD_LL_OpenEP(pdev,
                    HID_DEVICE_EPIN_ADDR,
                    USBD_EP_TYPE_INTR,
                    HID_DEVICE_EPIN_SIZE);
    pdev->ep_in[HID_DEVICE_EPIN_ADDR & 0xFU].is_used = 1U;

    /* Open EP OUT */
    USBD_LL_OpenEP(pdev, 
                    HID_DEVICE_EPOUT_ADDR, 
                    USBD_EP_TYPE_INTR,
                    HID_DEVICE_EPOUT_SIZE);
    pdev->ep_out[HID_DEVICE_EPOUT_ADDR & 0xFU].is_used = 1U;


    pdev->pClassData = USBD_malloc(sizeof(HID_Class_DataTypeDef));

    if (pdev->pClassData == NULL) {
     ret = USBD_FAIL;
    } else {
        ((HID_Class_DataTypeDef *)pdev->pClassData)->State = HID_IDLE;
    }

    HID_Device_PrepareReceive(pdev, HID_DEVICE_EPOUT_ADDR);

    return ret;
}

static uint8_t HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    UNUSED(cfgidx);
    /* Close HID EPs */
    USBD_LL_CloseEP(pdev, HID_DEVICE_EPIN_ADDR);
    pdev->ep_in[HID_DEVICE_EPIN_ADDR & 0xFU].is_used = 0U;
    USBD_LL_CloseEP(pdev, HID_DEVICE_EPOUT_ADDR);
    pdev->ep_out[HID_DEVICE_EPOUT_ADDR & 0xFU].is_used = 0U;

    /* Free allocated memory */
    if (pdev->pClassData != NULL) {
        USBD_free(pdev->pClassData);
        pdev->pClassData = NULL;
    }

    return USBD_OK;
}

static uint8_t HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    HID_Class_DataTypeDef *hhid = (HID_Class_DataTypeDef *) pdev->pClassData;
    uint16_t len = 0U;
    uint8_t *pbuf = NULL;
    uint16_t status_info = 0U;
    USBD_StatusTypeDef ret = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_CLASS :
            switch (req->bRequest) {
            case HID_REQ_SET_PROTOCOL:
                hhid->Protocol = (uint8_t)(req->wValue);
                break;
            case HID_REQ_GET_PROTOCOL:
                USBD_CtlSendData(pdev, (uint8_t *)&hhid->Protocol, 1U);
                break;
            case HID_REQ_SET_IDLE:
                hhid->IdleState = (uint8_t)(req->wValue >> 8);
                break;
            case HID_REQ_GET_IDLE:
                USBD_CtlSendData(pdev, (uint8_t *)&hhid->IdleState, 1U);
                break;
            case HID_REQ_SET_REPORT:
                hhid->IsReportAvailable = 1U;
                USBD_CtlPrepareRx(pdev, hhid->Report_buf, req->wLength);
          break;
            default:
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
                break;
            }
            break;
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest) {
            case USB_REQ_GET_STATUS:
                if (pdev->dev_state == USBD_STATE_CONFIGURED) {
                    USBD_CtlSendData(pdev, (uint8_t *)(void *)&status_info, 2U);
                } else {
                    USBD_CtlError(pdev, req);
                    ret = USBD_FAIL;
                }
                break;
            case USB_REQ_GET_DESCRIPTOR:
                if (req->wValue >> 8 == HID_DEVICE_REPORT_DESCRIPTOR_TYPE) {
                    len = MIN(sizeof(HID_Device_ReportDesc), req->wLength);
                    pbuf = HID_Device_ReportDesc;
                } else if (req->wValue >> 8 == HID_DEVICE_DESCRIPTOR_TYPE) {
                    len = MIN(HID_DEVICE_DESC_SIZ, req->wLength);
                    pbuf = HID_Device_CfgFSDesc+18;
                } else {
                    USBD_CtlError(pdev, req);
                    ret = USBD_FAIL;
                break;
                }
                USBD_CtlSendData(pdev, pbuf, len);
                break;
            case USB_REQ_GET_INTERFACE :
                if (pdev->dev_state == USBD_STATE_CONFIGURED) {
                USBD_CtlSendData(pdev, (uint8_t *)&hhid->AltSetting, 1U);
                } else {
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
                }
                break;
            case USB_REQ_SET_INTERFACE :
                if (pdev->dev_state == USBD_STATE_CONFIGURED) {
                    hhid->AltSetting = (uint8_t)(req->wValue);
                } else {
                    USBD_CtlError(pdev, req);
                    ret = USBD_FAIL;
                }
                break;
            default:
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
                break;
            }
            break;

        default:
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
    }

  return ret;
}

static uint8_t HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    /* Ensure that the FIFO is empty before a new transfer, this condition could
    be caused by  a new transfer before the end of the previous transfer */
    if (epnum == (HID_DEVICE_EPIN_ADDR & 0x7F)) {
        ((HID_Class_DataTypeDef *)pdev->pClassData)->State = HID_IDLE;
    }
    return USBD_OK;
}

static uint8_t HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    HID_Device_ReceiveReport(epnum, ((HID_Class_DataTypeDef *)pdev->pClassData)->Report_buf);
    return USBD_OK;
}

static uint8_t HID_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
    HID_Class_DataTypeDef *hhid = (HID_Class_DataTypeDef *)pdev->pClassData;
    if (hhid == NULL)
    {
        return (uint8_t)USBD_FAIL;
    }
    if (hhid->IsReportAvailable == 1U)
    {
        HID_Device_ReceiveReport(0, ((HID_Class_DataTypeDef *)pdev->pClassData)->Report_buf);
        hhid->IsReportAvailable = 0U;
    }

    return (uint8_t)USBD_OK;
}

static uint8_t *HID_GetFSCfgDesc(uint16_t *length)
{
    *length = sizeof(HID_Device_CfgFSDesc);
    // Setup Total length of Config descriptor
    uint32_t descSize = sizeof(HID_Device_ReportDesc);
    // Setup Total length of Report descriptors
    HID_Device_CfgFSDesc[2] = LOBYTE(*length);
    HID_Device_CfgFSDesc[3] = HIBYTE(*length);
    HID_Device_CfgFSDesc[25] = LOBYTE(descSize);
    HID_Device_CfgFSDesc[26] = HIBYTE(descSize);

    return HID_Device_CfgFSDesc;
}

static uint8_t  *HID_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = sizeof(HID_DeviceQualifierDesc);
  return HID_DeviceQualifierDesc;
}

uint8_t HID_Device_SendReport(USBD_HandleTypeDef  *pdev, uint8_t *report, uint16_t len)
{
    HID_Class_DataTypeDef* hhid = (HID_Class_DataTypeDef *)pdev->pClassData;
    if (pdev->dev_state == USBD_STATE_CONFIGURED) {
        if (hhid->State == HID_IDLE) {
            hhid->State = HID_BUSY;
            USBD_LL_Transmit(pdev, HID_DEVICE_EPIN_ADDR, report, len);
        }
    }
    return USBD_OK;
}

uint8_t HID_Device_PrepareReceive(USBD_HandleTypeDef  *pdev, uint8_t epnum)
{
    HID_Class_DataTypeDef* hhid = (HID_Class_DataTypeDef *)pdev->pClassData;
    /* Prepare Out endpoint to receive next packet */
    USBD_LL_PrepareReceive(pdev, epnum, hhid->Report_buf, HID_DEVICE_EPOUT_SIZE);
    return USBD_OK;
}
#ifndef __CONFIG_H
#define __CONFIG_H

#include <Arduino.h>

#define DEVICE_NAME "6Axis Gamepad (BLE)"
#define REPORT_POOL_INTERVAL 20
#define REPORT_BUF_SIZE 64

#define EXT1            25
#define EXT2            32

static const uint8_t REPORT_MAP[] = {
  0x05, 0x01,             // USAGE_PAGE (Generic Desktop)
  0x09, 0x04,             // USAGE (Joystick)
  0xa1, 0x01,             // COLLECTION (Application)

  0x85, 0x01,	            //   REPORT_ID	(1)	
  // State
  0x75, 0x08,             //   REPORT_SIZE (8bit)
  0x95, 0x01,             //   REPORT_COUNT (1)
  0x15, 0x00,             //   LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,       //   LOGICAL_MAXIMUM (255)
  0x81, 0x03,             //   INPUT (Data, Var, Abs)
  // Buttons section 0..16 bits
  0x75, 0x01,             //   REPORT_SIZE (1bit)
  0x95, 0x11,             //   REPORT_COUNT (17)	
  0x15, 0x00,             //   LOGICAL_MINIMUM (0)
  0x25, 0x01,             //   LOGICAL_MAXIMUM (1)
  0x35, 0x00,             //   Physical Minimum (0)
  0x45, 0x01,             //   Physical Maximum (1)
  0x05, 0x09,             //   USAGE_PAGE (Button)
  0x19, 0x01,             //   USAGE_MINIMUM (Button 1)
  0x29, 0x11,             //   USAGE_MAXIMUM (Button 17)
  0x81, 0x02,             //   INPUT (Data, Var, Abs)
  // reserved 17..31 bits
  0x75, 0x01,             //   REPORT_SIZE (1bit)
  0x95, 0x0F,             //   REPORT_COUNT (15)
  0x06, 0x00, 0xFF,       //   Usage Page (Vendor Defined 0xFF00)
  0x81, 0x03,             //   INPUT (Const, Var, Abs)
  // Left,Right pointer section
  0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
  0x09, 0x01,             //   USAGE (Pointer)
  0xA1, 0x00,             //   COLLECTION (Physical)
  0x75, 0x10,             //     REPORT_SIZE (16bit)
  0x16, 0xFC, 0x00,       //     LOGICAL_MINIMUM (-1024)
  0x26, 0x04, 0x00,       //     LOGICAL_MAXIMUM (1024)
  0x36, 0xFC, 0x00,       //     PHYSICAL_MINIMUM (-1024)
  0x46, 0x04, 0x00,       //     PHYSICAL_MAXIMUM (1024)
  0x95, 0x04,             //     REPORT_COUNT (4)
  0x09, 0x30,             //     USAGE (X)
  0x09, 0x31,             //     USAGE (Y)
  0x09, 0x33,             //     USAGE (Rx)
  0x09, 0x34,             //     USAGE (Ry)
  0x81, 0x02,             //     INPUT (Data,Var,Abs)
  0xC0,                   //   END COLLECTION
  
  0x85, 0x02,	            //   REPORT_ID	(2)	
  // Magn section
  0x05, 0x01,             // 	 USAGE_PAGE (Generic Desktop)
  0x09, 0x01,             //   USAGE (Pointer)
  0xa1, 0x00,             //   COLLECTION (--)
  0x75, 0x10,             //     REPORT_SIZE (16bit)
  0x16, 0xFC, 0x00,       //     LOGICAL_MINIMUM (-1024)
  0x26, 0x04, 0x00,       //     LOGICAL_MAXIMUM (1024)
  0x36, 0xFC, 0x00,       //     PHYSICAL_MINIMUM (-1024)
  0x46, 0x04, 0x00,       //     PHYSICAL_MAXIMUM (1024)
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
  0x75, 0x10,             //     REPORT_SIZE (16bit)
  0x16, 0xFC, 0x00,       //     LOGICAL_MINIMUM (-1024)
  0x26, 0x04, 0x00,       //     LOGICAL_MAXIMUM (1024)
  0x36, 0xFC, 0x00,       //     PHYSICAL_MINIMUM (-1024)
  0x46, 0x04, 0x00,       //     PHYSICAL_MAXIMUM (1024)
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
  0x75, 0x10,             //     REPORT_SIZE (16bit)
  0x16, 0x0E, 0x00,       //     LOGICAL_MINIMUM (-8,192)
  0x26, 0x02, 0x00,       //     LOGICAL_MAXIMUM (8,192)
  0x36, 0x0E, 0x00,       //     PHYSICAL_MINIMUM (-8,192)
  0x46, 0x02, 0x00,       //     PHYSICAL_MAXIMUM (8,192)
  0x95, 0x03,             //     REPORT_COUNT (3)
  0x09, 0x40,             //     USAGE (Vx)
  0x09, 0x41,             //     USAGE (Vy)
  0x09, 0x42,             //     USAGE (Vz)
  0x81, 0x02,             //     INPUT (Data,Var,Abs)
  0xC0,                   //   END COLLECTION

  // Control report from host to device section
  0x09, 0x03,             //   USAGE (Vendor Usage 3)
  0x85, 0x02,	            //   REPORT_ID	(2)	
  0x75, 0x08,             //   REPORT_SIZE (8)
  0x95, 0x3F,             //   REPORT_COUNT (63+1)
  0x91, 0x01,             //   OUTPUT (Data,Ary,Abs)
  0xC0                    // END COLLECTION
};


#endif
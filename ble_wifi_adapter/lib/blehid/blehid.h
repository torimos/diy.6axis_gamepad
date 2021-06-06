#ifndef __BLEHID_H
#define __BLEHID_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>

#define VENDOR_ID 0x02E5
#define PRODUCT_ID 0xABBB
#define PRODUCT_VER 0x0110

typedef void (*OutputCallbackEvent)(uint8_t* data);

class BleHid : 
    public BLEServerCallbacks, 
    public BLECharacteristicCallbacks
{
public:
    std::string deviceName;
    uint8_t* reportMap;
    uint16_t reportMapSize;
    BLEHIDDevice* hid;
    BLECharacteristic* input;
    BLECharacteristic* output;
    BLEAdvertising* advertising;
    bool isBleConnected;

    BleHid(const char* name, uint8_t* report, uint16_t reportSize)
    {
        this->deviceName = std::string(name);
        this->reportMap = report;
        this->reportMapSize = reportSize;
    }

    void begin();
    bool isConnected(void);
    void onConnect(BLEServer* server);
    void onDisconnect(BLEServer* server);
    void onWrite(BLECharacteristic* characteristic);
    static void taskServer(void* p);
    void notify(uint8_t *data, size_t length);
};

#endif
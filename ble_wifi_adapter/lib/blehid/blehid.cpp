#include "BleHid.h"

void BleHid::begin()
{
    xTaskCreate(BleHid::taskServer, "server", 20000, this, 5, NULL);
}

bool BleHid::isConnected(void)
{
    return isBleConnected;
}

void BleHid::onConnect(BLEServer* server) {
    // Allow notifications for characteristics
    BLE2902* cccDesc = (BLE2902*)inputs[0]->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    cccDesc->setNotifications(true);
    cccDesc = (BLE2902*)inputs[1]->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    cccDesc->setNotifications(true);
    #if DEBUG_LVL >= 2
    Serial.println("Client has connected");
    #endif
    isBleConnected = true;
}

void BleHid::onDisconnect(BLEServer* server) {
    // Disallow notifications for characteristics
    BLE2902* cccDesc = (BLE2902*)inputs[0]->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    cccDesc->setNotifications(false);
    cccDesc = (BLE2902*)inputs[1]->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    cccDesc->setNotifications(false);
    #if DEBUG_LVL >= 2
    Serial.println("Client has disconnected");
    #endif
    isBleConnected = false;
    advertising->start();
}

void BleHid::onWrite(BLECharacteristic* characteristic) {
    #if DEBUG_LVL >= 2
    uint8_t* data = characteristic->getData();
    Serial.printf("OutputCallbacks onWrite: %02X %02X %02X %02X %02X\n\r", data[0], data[1], data[2], data[3], data[4]);
    #endif
}

void BleHid::taskServer(void* p)
{
    BleHid* dev = (BleHid*)p;
    // initialize the device
    BLEDevice::init(dev->deviceName);
    BLEServer* server = BLEDevice::createServer();
    server->setCallbacks(dev);

    // create an HID device
    dev->hid = new BLEHIDDevice(server);
    dev->inputs[0]= dev->hid->inputReport(1);
    dev->inputs[1] = dev->hid->inputReport(2);
    dev->output = dev->hid->outputReport(3); 
    dev->output->setCallbacks(dev);

    // set manufacturer name
    dev->hid->manufacturer()->setValue("Maker Community");
    /*
    * Set pnp parameters (MANDATORY)
    * https://github.com/oesmith/gatt-xml/blob/master/org.bluetooth.characteristic.pnp_id.xml
    */


    dev->hid->pnp(0x01,VENDOR_ID,PRODUCT_ID,PRODUCT_VER);
    /*
    * Set hid informations (MANDATORY)
    * https://github.com/oesmith/gatt-xml/blob/master/org.bluetooth.characteristic.hid_information.xml
    * information about HID device: device is not localized, device can not be connected
    */
    dev->hid->hidInfo(0x00,0x01);


    // Security: device requires bonding
    BLESecurity* security = new BLESecurity();
    security->setAuthenticationMode(ESP_LE_AUTH_BOND);
    //security->setStaticPIN(147896);

    // set report map
    dev->hid->reportMap((uint8_t*)dev->reportMap, dev->reportMapSize);
    dev->hid->startServices();

    // set battery level to 100%
    dev->hid->setBatteryLevel(100);

    // advertise the services
    dev->advertising = server->getAdvertising();
    dev->advertising->setAppearance(HID_JOYSTICK);
    dev->advertising->addServiceUUID(dev->hid->hidService()->getUUID());
    dev->advertising->addServiceUUID(dev->hid->deviceInfo()->getUUID());
    dev->advertising->addServiceUUID(dev->hid->batteryService()->getUUID());
    dev->advertising->start();

    #if DEBUG_LVL >= 1
    Serial.println("BLE ready");
    #endif
    #if DEBUG_LVL >= 3
    Serial.print("hidService UUID: ");Serial.print(dev->hid->hidService()->getUUID().toString().c_str()); Serial.println();
    Serial.print("deviceInfo UUID: ");Serial.print(dev->hid->deviceInfo()->getUUID().toString().c_str()); Serial.println();
    Serial.print("batteryService UUID: ");Serial.print(dev->hid->batteryService()->getUUID().toString().c_str()); Serial.println();
    Serial.printf("input UUID: ");Serial.printf(dev->input->getUUID().toString().c_str()); Serial.println();
    Serial.printf("output UUID: ");Serial.printf(dev->output->getUUID().toString().c_str()); Serial.println();
    #endif
    vTaskDelay(portMAX_DELAY);
}

void BleHid::notify(uint8_t id, uint8_t *data, size_t length)
{
    inputs[id]->setValue(data, length);
    inputs[id]->notify();
}
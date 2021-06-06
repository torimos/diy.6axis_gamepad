#include <Arduino.h>
#include <BLEDevice.h>
#include <esp_err.h>
#include "esp32-hal-log.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("00001812-0000-1000-8000-00805f9b34fb");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("00002a4d-0000-1000-8000-00805f9b34fb");
// 00002a4d-0000-1000-8000-00805f9b34fb
// batteryService UUID: 0000180f-0000-1000-8000-00805f9b34fb

static boolean connected = false;
static boolean doConnect = false;
static boolean doScan = false;
static BLERemoteCharacteristic* input;
static BLERemoteCharacteristic* output;
static BLEAdvertisedDevice* myDevice;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.println("onConnect");
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    for (int i=0;i<length;i++)
      Serial.printf("%02x ", pData[i]);
    Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}
uint8_t tx_buf[64];
bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the in/out characteristics in the service of the remote BLE server.
	  auto charsMap = pRemoteService->getCharacteristicsByHandle();
    for (auto it = charsMap->begin(); it != charsMap->end(); ++it)
    {
      if (it->second->getUUID().equals(charUUID))
      {
        if (it->second->canWrite())
        {
          output = it->second;
        }
        else {
          input = it->second;
        }
        Serial.printf("Found %s h: %02X r:%d, w:%d, n:%d", it->second->getUUID().toString().c_str(), it->first, it->second->canRead(), it->second->canWrite(), it->second->canNotify());
        Serial.println();
      }
    }

    if (!input->canRead() || !output->canWrite()) {
      Serial.println("No input or output characteristics detected");
      return false;
    }

    input->setAuth(esp_gatt_auth_req_t::ESP_GATT_AUTH_REQ_NO_MITM);
    output->setAuth(esp_gatt_auth_req_t::ESP_GATT_AUTH_REQ_NO_MITM);

    if(input->canNotify())
      input->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

void loop() {
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  if (connected) {
      tx_buf[0]++;
      tx_buf[1] = 0xDE;
      tx_buf[2] = 0xAD;
      output->writeValue(tx_buf, 3);
  } else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
   
  delay(1000); // Delay a second between loops.
}
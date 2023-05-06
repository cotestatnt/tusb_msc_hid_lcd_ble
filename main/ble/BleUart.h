
#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include "sdkconfig.h"
// #if defined(CONFIG_BT_ENABLED)

#include <esp_err.h>

#include "nimconfig.h"
#include "NimBLEDevice.h"
#include "NimBLECharacteristic.h"

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class BleUART
{

public:
    esp_err_t init();
    BleUART(NimBLEServer *pSrv);
    esp_err_t startAdvertising(void);

    void setRxCallback(BLECharacteristicCallbacks* cb);
    void print(const char* msg);
private:
    NimBLEServer* m_pServer;
    NimBLEService *pService = nullptr;
    NimBLECharacteristic* pRxCharacteristic = nullptr;
    NimBLECharacteristic* pTxCharacteristic = nullptr;
    BLECharacteristicCallbacks * rxCallback = nullptr;
    BLEServerCallbacks * serverCallback = nullptr;
};

// #endif
#endif
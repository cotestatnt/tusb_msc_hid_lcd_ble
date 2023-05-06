
#include "BleHidKeybaord.h"
#include "BleUart.h"

NimBLEServer* pServer;

BleHidKeyboard* bleKeyboard;
BleUART* bleUart ;

bool canSendReport = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer, BLEConnInfo& connInfo) {
        printf("Device connected\n");
    };

    void onDisconnect(BLEServer* pServer, BLEConnInfo& connInfo, int reason) {
        printf("Device disconnected\n");
        canSendReport = false;
        pServer->startAdvertising(); // restart advertising
        printf("Restart HID advertising\n");
    }

    void onAuthenticationComplete(BLEConnInfo& connInfo){
        printf("Starting BLE report!\n");
        canSendReport = true;
    }
};

class MyRxCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo& connInfo) {
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            printf("Received: %s\n", rxValue.c_str());
            char txBuf[64];
            snprintf((char*)txBuf, sizeof(txBuf), "You sent: %s", rxValue.c_str());
            bleUart->print(txBuf);

            if (strstr(rxValue.c_str(), "play") != nullptr) {
                    printf("Sending Play/Pause media key...\n");
                    bleKeyboard->write(KEY_MEDIA_PLAY_PAUSE);
                }
                else if (strstr(rxValue.c_str(), "vol_up") != nullptr) {
                    printf("Sending Volume Up media key...\n");
                    bleKeyboard->write(KEY_MEDIA_VOLUME_UP);
                }
                else if (strstr(rxValue.c_str(), "vol_down") != nullptr){
                    printf("Sending Volume Up media key...\n");
                    bleKeyboard->write(KEY_MEDIA_VOLUME_DOWN);
                }
                vTaskDelay(100/portTICK_PERIOD_MS);
                bleKeyboard->releaseAll();
        }
    }
};


// void HidConnectedTask (void * parameter){
//     vTaskDelay(1000/portTICK_PERIOD_MS);
//     for(;;) {
//         if(canSendReport) {
//             // printf("Sending 'Hello world'...\n");
//             // const char* msg = "Hello World";
//             // bleKeyboard->write((uint8_t*)msg, strlen(msg));
//             printf("Sending Play/Pause media key...\n");
//             bleKeyboard->write(KEY_MEDIA_PLAY_PAUSE);
//             vTaskDelay(100/portTICK_PERIOD_MS);
//             bleKeyboard->releaseAll();
//             vTaskDelay(10000/portTICK_PERIOD_MS);
//         }
//         vTaskDelay(10/portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer
//     }
//     vTaskDelete(nullptr);
// }

void hid_main(void) {
    // Init NimBLE stack
    NimBLEDevice::init("NimBLE ESP32");
    NimBLEDevice::setSecurityAuth(true, false, false);
    NimBLEDevice::setMTU(512);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Start BLE keyboard HID service
    bleKeyboard = new BleHidKeyboard(pServer) ;
    bleKeyboard->setBatteryLevel(75);
    bleKeyboard->init();

    // Start BLE UART service
    bleUart = new BleUART(pServer);
    bleUart->setRxCallback(new MyRxCallbacks());
    bleUart->init();

    // Start advertising
    pServer->getAdvertising()->start();
    ESP_LOGD("ESP32", "Advertising started! Waiting for client tp notify");

    // xTaskCreate(HidConnectedTask, "HidConnectedTask", 5000, nullptr, 1, nullptr);
}









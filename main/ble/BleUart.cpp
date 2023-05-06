#include "BleUart.h"

static const char* LOG_TAG = "BLE UART";

BleUART::BleUART(NimBLEServer *pSrv) {
    m_pServer = pSrv;
}

void BleUART::setRxCallback(BLECharacteristicCallbacks* cb) {
    rxCallback = cb;
}


esp_err_t BleUART::init() {

    // Create the BLE Service
    pService = m_pServer->createService(SERVICE_UUID);
    if (pService == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    // Create a BLE Characteristic
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);
    pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
    pRxCharacteristic->setCallbacks(rxCallback);

    NimBLEAdvertising *pAdvertising = m_pServer->getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());

    // Start the service
    if (!pService->start()) {
        ESP_LOGE(LOG_TAG, "Failed to start BLE GATT service");
        return ESP_FAIL;
    }

    ESP_LOGI(LOG_TAG, "GATT service started");
    return ESP_OK;
}


esp_err_t BleUART::startAdvertising(void) {
    // Start advertising
    if (!m_pServer->getAdvertising()->start()) {
        ESP_LOGE(LOG_TAG, "Failed to start advertising");
        return ESP_FAIL;
    }
    ESP_LOGD(LOG_TAG, "Advertising started! Waiting for client");
    return ESP_OK;
}

void BleUART::print(const char* msg) {
    pTxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
    pTxCharacteristic->notify();
}

#include "BleHidKeybaord.h"

static const char* LOG_TAG = "BLE HID";

BleHidKeyboard::BleHidKeyboard( NimBLEServer *pSrv ) {
    m_pServer = pSrv;
}

esp_err_t BleHidKeyboard::init() {
    hid = new NimBLEHIDDevice(m_pServer);
    dataHid = new NimBLEHIDDevice(m_pServer);
    if (hid == nullptr || dataHid == nullptr) {
        return ESP_ERR_NO_MEM;
    }
    inputKeyboard = hid->inputReport(KEYBOARD_ID); // <-- input REPORTID from report map
    outputKeyboard = hid->outputReport(KEYBOARD_ID);
    inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID);
    inputData = dataHid->inputReport(0);
    outputData = dataHid->outputReport(0);

    hid->manufacturer()->setValue("Espressif");
    dataHid->manufacturer()->setValue("Espressif");

    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    hid->hidInfo(0x00,0x01);
    dataHid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    dataHid->hidInfo(0x00,0x01);

    hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
    dataHid->reportMap((uint8_t*)_hidDataReportDescriptor, sizeof(_hidDataReportDescriptor));
    hid->startServices();
    dataHid->startServices();

    NimBLEAdvertising *pAdvertising = m_pServer->getAdvertising();
    pAdvertising->setAppearance(HID_KEYBOARD);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    return ESP_OK;
}


esp_err_t BleHidKeyboard::startAdvertising(void) {
    // Start advertising
    if (!m_pServer->getAdvertising()->start()) {
        ESP_LOGE(LOG_TAG, "Failed to start advertising");
        return ESP_FAIL;
    }
    hid->setBatteryLevel(batteryLevel);
    ESP_LOGD(LOG_TAG, "Advertising started! Waiting for client");
    return ESP_OK;
}

void BleHidKeyboard::setBatteryLevel(uint8_t level) {
  this->batteryLevel = level;
  if (this->hid != 0)
    this->hid->setBatteryLevel(this->batteryLevel);
}

void BleHidKeyboard::sendReport(KeyReport *keys)
{
    inputKeyboard->setValue((uint8_t *)keys, sizeof(KeyReport));
    inputKeyboard->notify();
}

void BleHidKeyboard::sendReport(MediaKeyReport *keys)
{
    inputMediaKeys->setValue((uint8_t *)keys, sizeof(MediaKeyReport));
    inputMediaKeys->notify();
}

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t BleHidKeyboard::press(uint8_t k)
{
    uint8_t i;
    uint8_t pressed_key = 0;
    if (k >= 136) {
        // it's a non-printing key (not a modifier)
        pressed_key = k - 136;
    }
    else if (k >= 128) {
        // it's a modifier key
        keyReport.modifiers |= (1 << (k - 128));
        pressed_key = 0;
    }
    else {
        // it's a printing key
        pressed_key = ascii_map[k];
        ESP_LOGI(LOG_TAG, "pressed %d -> %d???\n", k, pressed_key);
        if (!pressed_key) {
            ESP_LOGI(LOG_TAG, "press: not in the ascii map!\n");
            return 0;
        }
        if (pressed_key & 0x80) {
            // it's a capital letter or other character reached with shift
            keyReport.modifiers |= 0x02; // the left shift modifier
            pressed_key &= 0x7F;
        }
    }

    // Add k to the key report only if it's not already present and if there is an empty slot.
    if (keyReport.keys[0] != pressed_key && keyReport.keys[1] != pressed_key &&
        keyReport.keys[2] != pressed_key && keyReport.keys[3] != pressed_key &&
        keyReport.keys[4] != pressed_key && keyReport.keys[5] != pressed_key)
    {

        for (i = 0; i < 6; i++) {
            if (keyReport.keys[i] == 0x00) {
                keyReport.keys[i] = pressed_key;
                break;
            }
        }
        if (i == 6) {
            ESP_LOGI(LOG_TAG, "no empty wtf???\n");
            return 0;
        }
        for (i = 0; i < 6; i++) {
            ESP_LOGI(LOG_TAG, "reporting keys: %d\n", keyReport.keys[i]);
        }
    }
    sendReport(&keyReport);
    return 1;
}

size_t BleHidKeyboard::press(const MediaKeyReport k)
{
    uint16_t k_16 = k[1] | (k[0] << 8);
    uint16_t mediaKeyReport_16 = mediaKeyReport[1] | (mediaKeyReport[0] << 8);
    mediaKeyReport_16 |= k_16;
    mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
    mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);
    sendReport(&mediaKeyReport);
    return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t BleHidKeyboard::release(uint8_t k)
{
    uint8_t i;
    if (k >= 136) {
        // it's a non-printing key (not a modifier)
        k = k - 136;
    }
    else if (k >= 128) {
        // it's a modifier key
        keyReport.modifiers &= ~(1 << (k - 128));
        k = 0;
    }
    else
    { // it's a printing key
        k = ascii_map[k];
        if (!k) {
            ESP_LOGI(LOG_TAG, "release: not in the ascii map!\n");
            return 0;
        }
        if (k & 0x80) {
            // it's a capital letter or other character reached with shift
            keyReport.modifiers &= ~(0x02); // the left shift modifier
            k &= 0x7F;
        }
    }

    // Test the key report to see if k is present.  Clear it if it exists.
    // Check all positions in case the key is present more than once (which it shouldn't be)
    for (i = 0; i < 6; i++) {
        if (0 != k && keyReport.keys[i] == k) {
            keyReport.keys[i] = 0x00;
        }
    }
    sendReport(&keyReport);
    return 1;
}

size_t BleHidKeyboard::release(const MediaKeyReport k)
{
    uint16_t k_16 = k[1] | (k[0] << 8);
    uint16_t mediaKeyReport_16 = mediaKeyReport[1] | (mediaKeyReport[0] << 8);
    mediaKeyReport_16 &= ~k_16;
    mediaKeyReport[0] = (uint8_t)((mediaKeyReport_16 & 0xFF00) >> 8);
    mediaKeyReport[1] = (uint8_t)(mediaKeyReport_16 & 0x00FF);
    sendReport(&mediaKeyReport);
    return 1;
}

void BleHidKeyboard::clearAll(void)
{
    keyReport.keys[0] = 0;
    keyReport.keys[1] = 0;
    keyReport.keys[2] = 0;
    keyReport.keys[3] = 0;
    keyReport.keys[4] = 0;
    keyReport.keys[5] = 0;
    keyReport.modifiers = 0;
    mediaKeyReport[0] = 0;
    mediaKeyReport[1] = 0;
}

void BleHidKeyboard::releaseAll(void)
{
    clearAll();
    sendReport(&keyReport);
}

size_t BleHidKeyboard::write(uint8_t c)
{
    uint8_t p = press(c); // Keydown
    vTaskDelay(3);
    release(c); // Keyup
    return p;   // just return the result of press() since release() almost always returns 1
}

size_t BleHidKeyboard::write(const MediaKeyReport c)
{
    uint16_t p = press(c); // Keydown
    vTaskDelay(3);
    release(c); // Keyup
    return p;   // just return the result of press() since release() almost always returns 1
}

size_t BleHidKeyboard::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (size--) {
        if (*buffer != '\r') {
            if (write(*buffer))
                n++;
            else
                break;
        }
        buffer++;
    }
    return n;
}

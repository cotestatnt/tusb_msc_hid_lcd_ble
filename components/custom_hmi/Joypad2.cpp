
#include "Joypad2.h"

esp_err_t gpio_init(void *param)
{
    button_gpio_config_t *cfg = (button_gpio_config_t *)param;
    return button_gpio_init(cfg);
}

uint8_t gpio_get_key_value(void *param)
{
    button_gpio_config_t *cfg = (button_gpio_config_t *)param;
    return button_gpio_get_key_level((void *)cfg->gpio_num);
}

esp_err_t gpio_deinit(void *param)
{
    button_gpio_config_t *cfg = (button_gpio_config_t *)param;
    return button_gpio_deinit(cfg->gpio_num);
}



button_handle_t Joypad::initButton(joypad_button_t& btn) {
    button_gpio_config_t *gpio_cfg = (button_gpio_config_t *)calloc(1, sizeof(button_gpio_config_t));
    gpio_cfg->active_level = 0;
    gpio_cfg->gpio_num = btn.gpio;

    button_config_t cfg = {
        .type = BUTTON_TYPE_CUSTOM,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
        .custom_button_config = {
            .active_level = 0,
            .button_custom_init = gpio_init,
            .button_custom_get_key_value = gpio_get_key_value,
            .button_custom_deinit = gpio_deinit,
            .priv = gpio_cfg,
        },
    };
    btn.handle = iot_button_create(&cfg);
    return btn.handle;
}

button_handle_t Joypad::getHandle(uint8_t gpio_num) {
    for(const joypad_button_t& bt: buttons) {
        if (bt.gpio == gpio_num)
            return bt.handle;
    }
    return nullptr;
}

int Joypad::getGPIO(button_handle_t handle) {
    for(const joypad_button_t& bt: buttons) {
        if (bt.handle == handle)
            return bt.gpio;
    }
    return -1;
}

void Joypad::addButton( uint8_t gpio_num, uint8_t active_level) {
    joypad_button_t button;
    button.gpio = gpio_num;
    button.level = active_level;
    initButton(button);
    buttons.push_back(button);
}

esp_err_t Joypad::addCallback( uint8_t gpio_num, button_event_t event, button_cb_t cb, void *usr_data) {
    button_handle_t btn = getHandle(gpio_num);
    return iot_button_register_cb(btn, event, cb, usr_data);
}


esp_err_t Joypad::addCallbackToAll(button_event_t event, button_cb_t cb, void *usr_data) {
    esp_err_t err = ESP_OK;
    for(const joypad_button_t& bt: buttons) {
        button_handle_t btn = getHandle(bt.gpio);
        err = iot_button_register_cb(btn, event, cb, usr_data);
        // printf("Button %d, active level %d\n", bt.gpio, bt.level);
    }
    return err;
}

void Joypad::listButtons() {
    for(const joypad_button_t& bt: buttons) {
        printf("Button %d, active level %d\n", bt.gpio, bt.level);
    }
}
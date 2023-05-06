#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "sdkconfig.h"
#include "esp_log.h"
#include "iot_button.h"


typedef struct {
  uint8_t gpio;
  uint8_t level;
  button_handle_t handle;
}
joypad_button_t;

/***************************************/
/*      JOYPAD CLASS DEDFINITION       */
/***************************************/

class Joypad {

  private:
    std::vector<joypad_button_t> buttons;

    button_handle_t initButton(joypad_button_t& btn) ;

  public:
    Joypad() {;}

    button_handle_t getHandle(uint8_t gpio_num) ;

    int getGPIO(button_handle_t handle);

    void addButton( uint8_t gpio_num, uint8_t active_level) ;

    esp_err_t addCallback( uint8_t gpio_num, button_event_t event, button_cb_t cb, void *usr_data = nullptr) ;

    esp_err_t addCallbackToAll(button_event_t event, button_cb_t cb, void *usr_data = nullptr);

    void listButtons() ;

};

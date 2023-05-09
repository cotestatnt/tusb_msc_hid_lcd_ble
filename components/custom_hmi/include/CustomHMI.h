#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "limits.h"
#include "float.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "iot_button.h"

#include "esp_board.h"

#include "lvgl.h"
#include "../ui/ui.h"
#include "../ui/ui_helpers.h"

#ifndef HMI_HANDLE_H_
#define HMI_HANDLE_H_


/***************************************/
/*         HMI CLASS DEDFINITION       */
/***************************************/

#define APP_DISP_DEFAULT_BRIGHTNESS 100 // 0-100%
#define TAG "HMI"

// The format to be printed float value on menu (1 decimal point)
#define FLOAT_FORMAT "%3.1f"
#define _OPEN_SYS_ITOA_EXT

#define typeof(X) typeid(X).hash_code()

#define TO_BOOL(x) *(bool*)x
#define TO_FLOAT(x) *(float*)x
#define TO_DOUBLE(x) *(double*)x
#define TO_INT(x) *(int*)x
#define TO_UNSIGNED_INT(x) *(unsigned int*)x
#define TO_LONG(x) *(long*)x
#define TO_UNSIGNED_LONG(x) *(unsigned long*)x
#define TO_CHAR(x) *(char*)(x)
#define TO_UNSIGNED_CHAR(x) *(unsigned char*)(x)

typedef void (*object_callback)(lv_obj_t *obj);

typedef struct
{
    lv_obj_t *obj;          // The item
    lv_obj_t *screen;       // The screen to be selected on MID button
    lv_obj_t *value_label;  // The value object (label) to be updated and shown
    object_callback cb_function;
    void *variable;         // The variable linked to the object
    size_t var_code;        // The hash code of variable type linked to the object
    double min;
    double max;
} menu_item_t;



class CustomHMI {

  private:
    std::vector<menu_item_t> items;

    lv_obj_t *current_obj = NULL;  // The object actual selected
    lv_obj_t *next_obj = NULL;     // The object to be selected
    lv_obj_t *last_screen = NULL;
    lv_group_t * group;

    bool isAnotherScreenChild(lv_obj_t *obj, lv_obj_t *current_screen) ;

  public:
    CustomHMI() {}

    void init() {
      group = lv_group_create();
    }

    menu_item_t * getItemFromObj(lv_obj_t *obj) ;

    void addMenuItem(lv_obj_t *obj, lv_obj_t *screen) ;

    template <class T>
    inline void bindVariable(lv_obj_t *obj, lv_obj_t *value_lbl, T& var) {
      menu_item_t *item = getItemFromObj(obj);
      if (item != NULL) {
          item->value_label = value_lbl;
          item->variable = &var;
          item->var_code = typeof(var);
          // printf("Var code: %d\n", item->var_code);
      }
      updateVariable(item, 0);
    }

    void bindCallback(lv_obj_t *obj, object_callback cb) ;

    void setVariableRange(lv_obj_t *obj, double min, double max) ;

    void setFirstMenuObject(lv_obj_t *obj) ;

    void setMenuScreenActive(lv_obj_t *target) ;

    void setItemActive(menu_item_t *item, bool next = true) ;

    void updateVariable(menu_item_t *item, float step) ;

    void updateItem(lv_obj_t *target) ;

    inline lv_obj_t * getCurrentObj() {return current_obj; }


    inline lv_obj_t * getNextObj() {return next_obj; }
    inline lv_obj_t * getLastScreen() {return last_screen; }
};


#endif
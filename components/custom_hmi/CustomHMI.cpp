#include "CustomHMI.h"

menu_item_t * CustomHMI::getItemFromObj(lv_obj_t *obj) {
    if (obj != nullptr) {
        for(menu_item_t &it: items) {
            if (it.obj == obj) {
                menu_item_t * pt = &it;
                return pt;
            }
        }
    }
    return nullptr;
}

void CustomHMI::updateVariable(menu_item_t *item, float step) {
    // If this item has a linked variable, edit the value
    if (item != nullptr) {
        if (nullptr != item->variable) {
            // printf("Update obj %d\n", item->obj);
            char valStr[16];

            if (item->var_code == typeof(int) || item->var_code == typeof(long) ) {
                // Do a copy of variable and add the step value
                long value = TO_LONG(item->variable);
                value += step;
                // Check for min/max limits
                if (value > item->max)   value = item->max;
                if (value < item->min)  value = item->min;
                // Save back new value to original variables
                TO_LONG(item->variable) = value;
                // Update screen
                itoa(value, valStr, 10);
                _ui_label_set_property(item->value_label, _UI_LABEL_PROPERTY_TEXT, valStr);
            }
            else if (item->var_code == typeof(bool)) {
                bool value = TO_BOOL(item->variable);
                value = !value;
                TO_BOOL(item->variable) = value;
                if (value ) strcpy(valStr, "true"); else strcpy(valStr, "false");
                _ui_label_set_property(item->value_label, _UI_LABEL_PROPERTY_TEXT, valStr);
            }
            else if (item->var_code == typeof(unsigned int) || item->var_code == typeof(unsigned long ) ) {
                unsigned long value = TO_UNSIGNED_LONG(item->variable);
                value += step;
                if (value > item->max)   value = item->max;
                if (value < item->min)  value = item->min;
                TO_UNSIGNED_LONG(item->variable) = value;
                utoa(value, valStr, 10);
                _ui_label_set_property(item->value_label, _UI_LABEL_PROPERTY_TEXT, valStr);
            }
            else if (item->var_code == typeof(double)) {
                double value = TO_DOUBLE(item->variable);
                value += step/10.0;
                if (value > item->max)   value = item->max;
                if (value < item->min)  value = item->min;
                TO_DOUBLE(item->variable) = value;
                snprintf(valStr, sizeof(valStr), FLOAT_FORMAT, value);
                _ui_label_set_property(item->value_label, _UI_LABEL_PROPERTY_TEXT, valStr);
            }
            else if (item->var_code == typeof(float)) {
                double value = TO_FLOAT(item->variable);
                value += step/10.0;
                if (value > item->max)   value = item->max;
                if (value < item->min)  value = item->min;
                TO_FLOAT(item->variable) = value;
                snprintf(valStr, sizeof(valStr), FLOAT_FORMAT, value);
                _ui_label_set_property(item->value_label, _UI_LABEL_PROPERTY_TEXT, valStr);
            }
        }
    }
}

void CustomHMI::updateItem(lv_obj_t *obj) {
    menu_item_t *item = getItemFromObj(obj);
    updateVariable(item, 0);
}

void CustomHMI::addMenuItem(lv_obj_t *obj, lv_obj_t *screen) {
    menu_item_t item;
    item.obj = obj;
    item.screen = screen;
    item.min = DBL_MIN;
    item.max = DBL_MAX;
    item.cb_function = nullptr;
    item.variable = nullptr;
    item.var_code = 0;

    items.push_back(item);
    lv_group_add_obj(group, obj);
    // printf("Obj item %d\n", item.obj);
}

// void CustomHMI::bindVariable(lv_obj_t *obj, lv_obj_t *value_lbl, void *var, int v_code) {
//     menu_item_t *item = getItemFromObj(obj);
//     if (item != nullptr) {
//         item->value_label = value_lbl;
//         item->variable = var;
//         item->var_code = v_code;
//         printf("Var code: %d\n", v_code);
//     }

//     updateVariable(item, 0);
// }

void CustomHMI::bindCallback(lv_obj_t *obj, object_callback cb) {
    menu_item_t *item = getItemFromObj(obj);
    if (item != nullptr) {
        item->cb_function = cb;
    }
}

void CustomHMI::setVariableRange(lv_obj_t *obj, double min, double max) {
    menu_item_t *item = getItemFromObj(obj);
    if (item != nullptr) {
        item->min = min;
        item->max = max;
    }
}


void CustomHMI::setMenuScreenActive(lv_obj_t *target) {
    lv_obj_t *current = lv_scr_act();
    if (nullptr != target && target != current)
    {
        last_screen = current;
        _ui_screen_change(target, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 0);

        // Set first child for current screen and clear focused state
        lv_obj_t *first_child = lv_obj_get_child(target, 0);
        for (int i = 0; i < lv_obj_get_child_cnt(target); i++)
        {
            lv_obj_clear_state(lv_obj_get_child(target, i), LV_STATE_FOCUSED);
        }
        current_obj = nullptr;
        next_obj = first_child;
        ESP_LOGI("HMI", "Set new screen. First select obj %d\n ", next_obj);
    }
}



bool CustomHMI::isAnotherScreenChild(lv_obj_t *obj, lv_obj_t *current_screen) {
    lv_obj_t * target_screen = nullptr;
    if (obj != nullptr)
        target_screen = lv_obj_get_screen(obj);
    if (current_screen != target_screen) {
        return true;
    }
    return false;
}

void CustomHMI::setItemActive(menu_item_t *item, bool next) {
    if (item != nullptr) {
        lv_obj_t *setObj = nullptr;
        if (nullptr == current_obj) {
            setObj = next_obj;
        }
        else {
            // Find next obj from items vector
            std::vector<menu_item_t>::iterator it;
            for(it = items.begin(); it != items.end(); it++ ) {
                if(item->obj == it->obj) {
                    if (next)
                        setObj = std::next(it, 1)->obj;
                    else
                        setObj = std::prev(it, 1)->obj;
                    break;
                }
            }
            // Clear actual selected object
            ESP_LOGI("HMI", "Clear actual selected item %d\n", current_obj);
            lv_obj_clear_state(current_obj, LV_STATE_FOCUSED);

            // If next object is in another screen, go back to first
            if (isAnotherScreenChild(setObj, lv_obj_get_screen(current_obj))){
                ESP_LOGI("HMI", "Obj %d is last screen child. Go back to first\n", current_obj);
                setObj = lv_obj_get_child(lv_obj_get_screen(current_obj), 0);
            }
        }

        if (nullptr != setObj) {
            ESP_LOGI("HMI", "Focus next item active %d\n", setObj);
            lv_group_focus_obj(setObj);
            lv_obj_add_state(setObj, LV_STATE_FOCUSED);
            current_obj = setObj;
        }
    }
}



#include "CustomHMI.h"
#include "Joypad2.h"

Joypad myJoypad;
CustomHMI myHMI;

extern "C"{void hmi_main(void);}

extern int counter;
extern int value1;
extern int value2;
extern double value3;
extern float value4;
extern bool value5;
extern int red;
extern int green;
extern int blue;

extern void save_option(lv_obj_t *);
extern void set_HID_callback(lv_obj_t *);
extern void set_mount_callback(lv_obj_t *);
extern void set_color_callback(lv_obj_t *);
extern void set_white_callback(lv_obj_t *);



/* This function will build items relation for properly navigate menu using joypad buttons
 *  For each menu item, some properties need to be setted:
 *   - object to be highlighted
 *   - target screen to be shown on MID BUTTON click (can be NULL)
 *
 *  Is also possible bind a variable to a a specific menu item ù
 *  in order to edit it's value using LEFT and RIGHT BUTTON click
 */
void link_menu_items() {
    myHMI.init();
    myHMI.addMenuItem(ui_PanelEdit, ui_ScreenEdit);
    myHMI.addMenuItem(ui_PanelOptions, ui_ScreenOptions);
    myHMI.addMenuItem(ui_PanelEditRGB, ui_ScreenEditRGB);
    myHMI.addMenuItem(ui_ButtonSave, nullptr);
    myHMI.bindCallback(ui_ButtonSave, save_option);

    myHMI.addMenuItem(ui_Panel1, nullptr);
    myHMI.addMenuItem(ui_Panel2, nullptr);
    myHMI.addMenuItem(ui_Panel3, nullptr);
    myHMI.addMenuItem(ui_Panel4, nullptr);
    myHMI.addMenuItem(ui_Panel5, nullptr);
    myHMI.addMenuItem(ui_Panel6, nullptr);

    // // Options screen
    myHMI.addMenuItem(ui_PanelEditHID, nullptr);
    myHMI.addMenuItem(ui_PanelEditMount, nullptr);
    myHMI.bindCallback(ui_PanelEditHID, set_HID_callback);
    myHMI.bindCallback(ui_PanelEditMount, set_mount_callback);

    // // RGB led screen
    myHMI.addMenuItem(ui_PanelRed, nullptr);
    myHMI.addMenuItem(ui_PanelGreen, nullptr);
    myHMI.addMenuItem(ui_PanelBlue, nullptr);
    myHMI.addMenuItem(ui_ButtonOK, nullptr);
    myHMI.addMenuItem(ui_ButtonWHITE, nullptr);

    // Attach the function to be called (on MID button click) to the menu objects (Buttons)
    myHMI.bindCallback(ui_ButtonOK, set_color_callback);
    myHMI.bindCallback(ui_ButtonWHITE, set_white_callback);

    // Attach the variable to be edited to the menu objects (text label)
    // object to be highlighted, target text obj for value, pointer to variable, type of variable
    myHMI.bindVariable(ui_Panel1, ui_value1, value1);
    myHMI.bindVariable(ui_Panel2, ui_value2, value2);
    myHMI.bindVariable(ui_Panel3, ui_value3, value3);
    myHMI.bindVariable(ui_Panel4, ui_value4, value4);
    myHMI.bindVariable(ui_Panel5, ui_value5, value5);
    myHMI.bindVariable(ui_PanelRed, ui_valueRed, red);
    myHMI.bindVariable(ui_PanelGreen, ui_valueGreen, green);
    myHMI.bindVariable(ui_PanelBlue, ui_valueBlue, blue);

    // Set the range for this variables
    myHMI.setVariableRange(ui_PanelRed, 0, 255);
    myHMI.setVariableRange(ui_PanelGreen, 0, 255);
    myHMI.setVariableRange(ui_PanelBlue, 0, 255);

    // Bind variable to the main screen
    myHMI.addMenuItem(ui_Counter, nullptr);
    myHMI.bindVariable(ui_Counter, ui_Counter, counter);
}

 menu_item_t * getItem() {
    menu_item_t * item = nullptr;
    lv_obj_t * current = myHMI.getCurrentObj();
    if (current != nullptr) {
        item = myHMI.getItemFromObj(current);
        ESP_LOGV(TAG, "Current item obj %d\n", current);
    }
    else {
        item = new menu_item_t();
        item->obj = myHMI.getNextObj();
        item->screen = lv_scr_act();
        ESP_LOGV(TAG, "Set next obj %d\n", item->obj);
    }
    return item;
 }

void ButtonSingleClick(void *arg, void *data) {
    int gpio = myJoypad.getGPIO((button_handle_t)arg);
    ESP_LOGI(TAG, "BTN%d: BUTTON_SINGLE_CLICK", gpio);

    menu_item_t * item = getItem();
    if (lv_scr_act() == ui_ScreenMain) {
        item = nullptr;
        ESP_LOGW(TAG, "Navigation buttons disabled on main screen\n");
    }
    switch (gpio)
    {
    case BTN_LEFT:
        myHMI.updateVariable(item, -1);
        break;
    case BTN_RIGHT:
        myHMI.updateVariable(item, +1);
        break;
    case BTN_UP:
        myHMI.setItemActive(item, false);
        break;
    case BTN_DOWN:
        myHMI.setItemActive(item, true);
        break;
    case BTN_MID:
        if (item != nullptr) {
            if (item->screen != nullptr) {
                myHMI.setMenuScreenActive(item->screen);
                ESP_LOGI(TAG, "Set a new screen\n");
            }
            if (item->cb_function != nullptr)
                item->cb_function(item->obj);
        }
        break;
    case BTN_RES:
        ESP_LOGI(TAG, "Go back to previous active screen\n");
        myHMI.setMenuScreenActive(myHMI.getLastScreen());
        break;
    case BTN_SET:
        if (lv_scr_act() == ui_ScreenMain) {
            ESP_LOGI(TAG, "Enter setup menu\n");
            // Set the first object to be highlighted on edit menu
            myHMI.setMenuScreenActive(ui_ScreenSetup);
        }
        else {
            ESP_LOGI(TAG, "Exit setup menu\n");
            myHMI.setMenuScreenActive(ui_ScreenMain);
        }
        break;
    default:
        break;
    }
}


void ButtonLongPressHold(void *arg, void *data)
{
    int gpio = myJoypad.getGPIO((button_handle_t)arg);
    ESP_LOGI(TAG, "BTN%d: BUTTON_SINGLE_CLICK", gpio);
    int step = 10;
    menu_item_t * item = getItem();
    switch (gpio)
    {
    case BTN_LEFT:
        myHMI.updateVariable(item, -1 * step);
        break;
    case BTN_RIGHT:
        myHMI.updateVariable(item, step);
        break;
    default:
        break;
    }
}


void joypad_init() {
    myJoypad.addButton(BTN_UP, 0);
    myJoypad.addButton(BTN_DOWN, 0);
    myJoypad.addButton(BTN_LEFT, 0);
    myJoypad.addButton(BTN_RIGHT, 0);
    myJoypad.addButton(BTN_MID, 0);
    myJoypad.addButton(BTN_SET, 0);
    myJoypad.addButton(BTN_RES, 0);
    myJoypad.addCallbackToAll(BUTTON_SINGLE_CLICK, ButtonSingleClick);
    myJoypad.addCallback(BTN_LEFT, BUTTON_LONG_PRESS_HOLD, ButtonLongPressHold);
    myJoypad.addCallback(BTN_RIGHT, BUTTON_LONG_PRESS_HOLD, ButtonLongPressHold);
}



// Show objects on display
void app_lvgl_display(void)
{
    bsp_display_lock(0);

    ui_init();

    bsp_display_unlock();
}



// Init HMI (joypad + LCD)
void hmi_main()
{
    // Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Initialize display and LVGL */
    bsp_display_start();

    /* Set default display brightness */
    bsp_display_brightness_set(APP_DISP_DEFAULT_BRIGHTNESS);

    /* Add and show objects on display */
    app_lvgl_display();
    ESP_LOGI(TAG, "Example initialization done.");

    // Call this function to initialize gamepad buttons
    joypad_init();

    link_menu_items();
}

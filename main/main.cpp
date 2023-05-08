/*
 * GPIOs definitions inside
 *    components/esp_custom_board/include/esp_board.h
 */
#include <esp_board.h>

// USB included as file
#include "usb/usb_main.c"

// BLE HID/UART emulation
#include "main_ble.cpp"

extern "C"{void app_main(void);}

// RGB led stuffs
#include <led_strip.h>                  // RGB NeoPixel LED library
static led_strip_handle_t led_strip;
#include "rgb/rgb_led.c"

const char *var_filename = "/var_tracing.csv";

// Some variables tied to menu items
int value1 = 100;
int value2 = 1500;
double value3 = 25.7;
float value4 = 12.1;
bool value5 = true;
int red = 5;
int green = 5;
int blue = 5;
int counter = 0;
#include "hmi/hmi_handle.cpp"

/* Load variables last values from var_tracing.csv file (last line)*/
static int load_last_values()
{
    if (tinyusb_msc_storage_in_use_by_usb_host())
    {
        ESP_LOGE("READ", "storage exposed over USB. Application can't read from storage.");
        return -1;
    }
    ESP_LOGD("READ", "read from storage:");
    char path[32];
    strcpy(path, BASE_PATH);
    strcat(path, var_filename);
    FILE *pFile = fopen(path, "r");
    if (!pFile)
    {
        ESP_LOGE("READ", "Filename not present - %s", var_filename);
        return -1;
    }
    int num_rows = 0;
    int last_pos = 0;
    char ch;
    // Count the rows number and set last-1 position in order to get last row content
    while (!feof(pFile))
    {
        ch = fgetc(pFile);
        if (ch == '\n')
        {
            num_rows++;
            // If this is not the last row, save position
            ch = fgetc(pFile);
            if (!feof(pFile))
            {
                last_pos = ftell(pFile);
            }
        }
    }
    fseek(pFile, last_pos - 1, SEEK_SET);

    // char buf[64];
    // while (fgets(buf, sizeof(buf), pFile) != NULL) {
    //     printf("%s", buf);
    // }
    fscanf(pFile, "%d", &value1);
    fscanf(pFile, "%d", &value2);
    fscanf(pFile, "%f", &value3);
    fscanf(pFile, "%f", &value4);
    fscanf(pFile, "%d", &value5);
    fscanf(pFile, "%d", &red);
    fscanf(pFile, "%d", &green);
    fscanf(pFile, "%d", &blue);
    fclose(pFile);

    printf("Last saved values: %d\t %d\t %3.1f\t %3.1f\t %d\t %d\t %d\t %d\n",
           value1, value2, value3, value4, value5, red, green, blue);
    set_led(red, green, blue);
    return 1;
}

/* Append variables values to file. Create if don't exists*/
static int append_to_file(const char *str)
{
    if (tinyusb_msc_storage_in_use_by_usb_host())
    {
        ESP_LOGE("WRITE", "storage exposed over USB. Application can't write to storage.");
        return 0;
    }
    ESP_LOGD("WRITE", "write to storage:");
    char path[32];
    strcpy(path, BASE_PATH);
    strcat(path, var_filename);
    FILE *pFile = fopen(path, "r");
    if (!pFile)
    {
        ESP_LOGW("WRITE", "%s doesn't exist yet, creating", var_filename);
        pFile = fopen(path, "w");
        if (pFile) {
            fprintf(pFile, "v1\t v2\t v3\t v4\t v5\t R\t G\t B\n");
            fclose(pFile);
        }
        else
            ESP_LOGE("WRITE", "Create file %s error.", var_filename);

    }
    pFile = fopen(path, "a");
    if (pFile) {
        fprintf(pFile, str);
        fclose(pFile);
        return 1;
    }
    else {
        // If error on write, unmount and mount then call again save_option() functions
        msc_unmount();
        msc_mount();
        save_option(nullptr);
    }
    return 0;
}


void set_HID_callback(lv_obj_t *obj){
    arrow_keypad_active = !arrow_keypad_active;
    if (arrow_keypad_active)
        lv_obj_add_state(ui_CheckboxHID, LV_STATE_CHECKED);
    else
        lv_obj_clear_state(ui_CheckboxHID, LV_STATE_CHECKED);
    printf("%s\n",arrow_keypad_active ? "HID arrrow keypad enabled\nTry to use joypad." : "HID arrrow keypad disabled");
}


void set_mount_callback(lv_obj_t *obj){
    static bool mount_drive = false;
    mount_drive = !mount_drive;
    if (mount_drive) {
        // Mount partition in the app
        msc_mount();
        lv_obj_add_state(ui_CheckboxMount, LV_STATE_CHECKED);
    }
    else {
        msc_unmount();
        lv_obj_clear_state(ui_CheckboxMount, LV_STATE_CHECKED);
    }
    printf("%s\n", mount_drive ? "Partition mounted (App can write)" : "Partition unmounted (connected to host)");
}

/* Called when "OK" HMI button was "pressed" (using joypad MID button)*/
void set_color_callback(lv_obj_t *obj)
{
    printf("Object %d trigger this callback\n", obj);
    set_led(red, green, blue);
}

/* Called when "OK" HMI button was "pressed" (using joypad MID button)*/
void save_option(lv_obj_t *obj)
{
    char var_str[128];
    snprintf(
        var_str, sizeof(var_str),
        "%d\t %d\t %3.1f\t %3.1f\t %d\t %d\t %d\t %d\n",
        value1, value2, value3, value4, value5, red, green, blue);
    if (append_to_file(var_str)) {
        printf("Variables file saved!\n");
    }
}

/* Called when "WHITE" HMI button was "pressed" (using joypad MID button)*/
void set_white_callback(lv_obj_t *obj)
{
    printf("Object %d trigger this callback\n", obj);
    red = 15;
    green = 15;
    blue = 15;
    myHMI.updateItem(ui_valueRed);
    myHMI.updateItem(ui_PanelGreen);
    myHMI.updateItem(ui_PanelBlue);
    set_led(red, green, blue);
}



// A very simple task which increment a variable once a second
// In the same task, update also RGB led when values change
static void counter_task(void *arg)
{
    while (1)
    {
        counter++;
        if (lv_scr_act() == ui_ScreenMain)
            myHMI.updateItem(ui_Counter);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void app_main(void) {
    // Configure RGB led
    configure_led();

    // Start USB stuff
    usb_main();

    // Load variable values from file
    msc_mount(); // Mount partition in the app
    load_last_values();
    set_led(red, green, blue);

    // Init hmi (lcd + joypad)
    hmi_main();

    // Start counter task
    xTaskCreatePinnedToCore(counter_task, "Counter Task", 4096, NULL, 5, NULL, tskNO_AFFINITY);


    hid_main();
}

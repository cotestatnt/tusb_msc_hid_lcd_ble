
idf_component_register(
    SRCS
        "main.cpp"
        "main_ble.cpp"
        "hmi/hmi_handle.cpp"
        "ble/BleHidKeybaord.cpp"
        "ble/BleUart.cpp"

    INCLUDE_DIRS
        "."
        "ble"
        "usb"
        "rgb"
        "hmi"
)

#LVGL framework
target_compile_options(${COMPONENT_LIB} PRIVATE -Wunused-function)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wmissing-field-initializers)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wdeprecated-enum-enum-conversion)


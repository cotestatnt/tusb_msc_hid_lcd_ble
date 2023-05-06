
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

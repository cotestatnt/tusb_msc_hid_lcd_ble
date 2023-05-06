#Add sources from ui directory
file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "ui/*.c")

idf_component_register(
  SRCS "CustomHMI.cpp" "Joypad2.cpp" ${SRC_UI}
  INCLUDE_DIRS "include" "ui"
)

#LVGL framework
target_compile_options(${COMPONENT_LIB} PRIVATE -Wunused-function)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wmissing-field-initializers)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wdeprecated-enum-enum-conversion)
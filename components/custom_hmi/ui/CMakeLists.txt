SET(SOURCES screens/ui_ScreenMain.c
    screens/ui_ScreenSetup.c
    screens/ui_ScreenEdit.c
    screens/ui_ScreenOptions.c
    screens/ui_ScreenEditRGB.c
    ui.c
    ui_helpers.c
    images/ui_img_749525677.c)

add_library(ui ${SOURCES})

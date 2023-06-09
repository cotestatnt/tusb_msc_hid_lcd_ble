// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.4
// Project name: SquareLine_SimpleMenu

#include "../ui.h"

void ui_ScreenOptions_screen_init(void)
{
    ui_ScreenOptions = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenOptions, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_flex_flow(ui_ScreenOptions, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_ScreenOptions, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_left(ui_ScreenOptions, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_ScreenOptions, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_ScreenOptions, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_ScreenOptions, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_ScreenOptions, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_ScreenOptions, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelEditHID = lv_obj_create(ui_ScreenOptions);
    lv_obj_set_width(ui_PanelEditHID, lv_pct(100));
    lv_obj_set_height(ui_PanelEditHID, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PanelEditHID, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_PanelEditHID, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_PanelEditHID, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_PanelEditHID, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_width(ui_PanelEditHID, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelEditHID, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_PanelEditHID, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_PanelEditHID, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_PanelEditHID, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_PanelEditHID, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_PanelEditHID, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_PanelEditHID, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_PanelEditHID, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_PanelEditHID, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui_PanelEditHID, 1, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_CheckboxHID = lv_checkbox_create(ui_PanelEditHID);
    lv_checkbox_set_text(ui_CheckboxHID, "USB KEYPAD");
    lv_obj_set_width(ui_CheckboxHID, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_CheckboxHID, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_CheckboxHID, 387);
    lv_obj_set_y(ui_CheckboxHID, -44);
    lv_obj_set_align(ui_CheckboxHID, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_CheckboxHID, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags

    lv_obj_set_style_border_color(ui_CheckboxHID, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_CheckboxHID, 255, LV_PART_INDICATOR | LV_STATE_FOCUSED);

    ui_PanelEditMount = lv_obj_create(ui_ScreenOptions);
    lv_obj_set_width(ui_PanelEditMount, lv_pct(100));
    lv_obj_set_height(ui_PanelEditMount, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PanelEditMount, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_PanelEditMount, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_PanelEditMount, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_PanelEditMount, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_width(ui_PanelEditMount, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelEditMount, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_PanelEditMount, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_PanelEditMount, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_PanelEditMount, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_PanelEditMount, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_PanelEditMount, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_PanelEditMount, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_PanelEditMount, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_PanelEditMount, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui_PanelEditMount, 1, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_CheckboxMount = lv_checkbox_create(ui_PanelEditMount);
    lv_checkbox_set_text(ui_CheckboxMount, "Mount Device");
    lv_obj_set_width(ui_CheckboxMount, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_CheckboxMount, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_CheckboxMount, 387);
    lv_obj_set_y(ui_CheckboxMount, -44);
    lv_obj_set_align(ui_CheckboxMount, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_CheckboxMount, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags

    lv_obj_set_style_border_color(ui_CheckboxMount, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_CheckboxMount, 255, LV_PART_INDICATOR | LV_STATE_FOCUSED);

}

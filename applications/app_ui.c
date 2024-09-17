/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-09-18     starry       the first version
 */
#include <lvgl.h>
int32_t MAX_VALUE = 100;
int32_t MIN_VALUE = 0;
int32_t SCALE_LEFT_BOUND_TICKS = 200;
int32_t SCALE_ANGLE_RANGE = 140;
int32_t SCALE_RIGHT_BOUND_TICKS = 340;
int32_t ARC_START_ROTATION = 120;
typedef enum {
    SUPER_DIAL_NULL = 0,
    SUPER_DIAL_LEFT = 1,
    SUPER_DIAL_RIGHT= 2,
} SuperDialMotion;

typedef struct
{
    int32_t xkonb_value;
    int32_t motor_pos;
    int32_t angle_offset;
    SuperDialMotion konb_direction;
} PlaygroundInfo;
struct
{
    lv_obj_t* meter;
    lv_meter_scale_t* scale_pot;   // 圆点的刻度
    lv_obj_t* label_value;
    lv_meter_indicator_t* nd_img_circle;
    lv_meter_scale_t* scale_arc;   // arc 的刻度
    lv_meter_indicator_t* arc;
    lv_group_t* group;
} ui;

struct
{
    lv_style_t meter;
    lv_style_t ticks;
} style;
static lv_timer_t* timer;
void Create(lv_obj_t* root)
{
    LV_IMG_DECLARE(img_src_dot_blue)
    lv_obj_remove_style_all(root);
    lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(root, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);

    //Write codes meter
    ui.meter = lv_meter_create(root);
    lv_obj_set_pos(ui.meter, 1, 0);
    lv_obj_set_size(ui.meter, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_scrollbar_mode(ui.meter, LV_SCROLLBAR_MODE_OFF);

    //add scale ui.scale_circle
    ui.scale_pot = lv_meter_add_scale(ui.meter);
    lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 41, 2, 0, lv_color_make(0xff, 0x00, 0x00));
    lv_meter_set_scale_range(ui.meter, ui.scale_pot, 0, 360, 360, 270);

    //add needle img for ui.scale_circle
    ui.nd_img_circle = lv_meter_add_needle_img(ui.meter, ui.scale_pot,(void*)&img_src_dot_blue, -100, 8);
    lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, 0);

    //add scale ui.scale_value
    ui.scale_arc = lv_meter_add_scale(ui.meter);
    lv_meter_set_scale_ticks(ui.meter, ui.scale_arc, 41, 0, 0, lv_color_make(0xff, 0x00, 0x00));
    lv_meter_set_scale_range(ui.meter, ui.scale_arc, 0, 100, 360, ARC_START_ROTATION);

    //add arc for ui.scale_value
    ui.arc = lv_meter_add_arc(ui.meter, ui.scale_arc, 6, lv_color_make(0xff, 0x00, 0x00), 5);
    lv_meter_set_indicator_start_value(ui.meter, ui.arc, 0);
    lv_meter_set_indicator_end_value(ui.meter, ui.arc, 0);

    //Write style state: LV_STATE_DEFAULT for meter_style
    lv_style_init(&style.meter);
    lv_style_set_radius(&style.meter, 240);
    lv_style_set_bg_color(&style.meter, lv_color_black());
    lv_style_set_bg_grad_color(&style.meter, lv_color_make(128,0,128));
    lv_style_set_bg_grad_dir(&style.meter, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style.meter, 255);
    lv_style_set_bg_main_stop(&style.meter, 255);
    lv_style_set_bg_grad_stop(&style.meter, 255);
    // lv_style_set_border_color(&style.meter, lv_color_make(0x12, 0x11, 0x12));
    lv_style_set_border_width(&style.meter, 0);
    lv_style_set_border_opa(&style.meter, 255);
    lv_obj_add_style(ui.meter, &style.meter, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for style_screen_meter_1_main_ticks_default
    lv_style_init(&style.ticks);
    lv_style_set_text_color(&style.ticks, lv_color_white());
    lv_style_set_text_font(&style.ticks, &lv_font_montserrat_14);
    lv_obj_add_style(ui.meter, &style.ticks, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write codes screen_label_1

    ui.label_value = lv_label_create(root);
    lv_obj_set_pos(ui.label_value, 70, 67);
    lv_obj_set_size(ui.label_value, 100, 32);
    lv_obj_set_scrollbar_mode(ui.label_value, LV_SCROLLBAR_MODE_OFF);
    lv_label_set_text(ui.label_value, "0");
    lv_label_set_long_mode(ui.label_value, LV_LABEL_LONG_WRAP);

    //Write style state: LV_STATE_DEFAULT for label_style
    static lv_style_t label_style;
    lv_style_init(&label_style);
    lv_style_set_radius(&label_style, 0);
    lv_style_set_bg_color(&label_style, lv_color_make(0x0f, 0x0f, 0x0f));
    lv_style_set_bg_grad_color(&label_style, lv_color_make(0x21, 0x95, 0xf6));
    lv_style_set_bg_grad_dir(&label_style, LV_GRAD_DIR_NONE);
    lv_style_set_bg_opa(&label_style, 0);  // 完全透明
    lv_style_set_text_color(&label_style, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&label_style, &lv_font_montserrat_26);
    lv_style_set_text_letter_space(&label_style, 2);
    lv_style_set_text_line_space(&label_style, 0);
    lv_style_set_text_align(&label_style, LV_TEXT_ALIGN_CENTER);
    lv_style_set_pad_left(&label_style, 0);
    lv_style_set_pad_right(&label_style, 0);
    lv_style_set_pad_top(&label_style, 8);
    lv_style_set_pad_bottom(&label_style, 0);
    lv_obj_add_style(ui.label_value, &label_style, LV_PART_MAIN|LV_STATE_DEFAULT);


    ui.group = lv_group_create();
//    lv_indev_set_group(lv_get_indev(LV_INDEV_TYPE_ENCODER), ui.group);

    lv_group_add_obj(ui.group, ui.meter);
    lv_group_focus_obj(ui.meter);

}
void BoundZeroView()
{
    MIN_VALUE = 0;
    MAX_VALUE = 8;
    SCALE_LEFT_BOUND_TICKS = 200;
    SCALE_ANGLE_RANGE = 140;

    SCALE_RIGHT_BOUND_TICKS = SCALE_LEFT_BOUND_TICKS + SCALE_ANGLE_RANGE;
    lv_meter_set_scale_ticks(ui.meter, ui.scale_pot, 13, 2, 0, lv_color_make(0xff, 0xff, 0xff));
    lv_meter_set_scale_major_ticks(ui.meter, ui.scale_pot, 12, 4, 20, lv_color_make(0xff, 0xff, 0xff), 10);
    lv_meter_set_scale_range(ui.meter, ui.scale_pot, MIN_VALUE , MAX_VALUE, SCALE_ANGLE_RANGE, SCALE_LEFT_BOUND_TICKS);

    // display out of bounds
    lv_meter_set_scale_ticks(ui.meter, ui.scale_arc, 40, 0, 0, lv_color_make(0xff, 0x00, 0x00));
    lv_meter_set_scale_range(ui.meter, ui.scale_arc, 0, 360, 360, ARC_START_ROTATION);
    lv_meter_set_indicator_start_value(ui.meter, ui.arc, 0);
    lv_meter_set_indicator_end_value(ui.meter, ui.arc, 0);
}
static long map(long x, long in_min, long in_max, long out_min, long out_max) {
    const long run = in_max - in_min;
    if(run == 0){
//        log_e("map(): Invalid input range, min == max");
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;
    return (delta * rise) / run + out_min;
}
void UpdateBackgroundView(PlaygroundInfo *info)
{
    // lv_style_set_bg_main_stop(&style.meter, 255-xkonb_value);
    // lv_style_set_bg_grad_stop(&style.meter, 255-xkonb_value);
    int value_map = map(info->xkonb_value, 0, MAX_VALUE, 0, 255);
    lv_obj_set_style_bg_main_stop(ui.meter, 255 - value_map, 0);
    lv_obj_set_style_bg_grad_stop(ui.meter, 255 - value_map, 0);
}

extern uint16_t read_raw_angel;

void UpdateView(PlaygroundInfo *info)
{
    int _value = 0;
    int32_t motor_pos = info->motor_pos;//info->motor_pos;   (int32_t)(read_raw_angel*360/4096.0f)
    motor_pos = motor_pos % 360;
    if (motor_pos < 0) {
        motor_pos = 360 + motor_pos;
    }
    lv_meter_set_indicator_value(ui.meter, ui.nd_img_circle, motor_pos);
    _value = info->xkonb_value;
    UpdateBackgroundView(info);

    if (info->angle_offset != 0) {
        if (info->xkonb_value == MIN_VALUE) {
            lv_meter_set_indicator_start_value(ui.meter, ui.arc, SCALE_LEFT_BOUND_TICKS - ARC_START_ROTATION - info->angle_offset);
            lv_meter_set_indicator_end_value(ui.meter, ui.arc, SCALE_LEFT_BOUND_TICKS - ARC_START_ROTATION);
        } else {
            lv_meter_set_indicator_start_value(ui.meter, ui.arc, SCALE_RIGHT_BOUND_TICKS - ARC_START_ROTATION);
            lv_meter_set_indicator_end_value(ui.meter, ui.arc, SCALE_RIGHT_BOUND_TICKS - ARC_START_ROTATION - info->angle_offset);
        }

    } else {
        lv_meter_set_indicator_start_value(ui.meter, ui.arc, 0);
        lv_meter_set_indicator_end_value(ui.meter, ui.arc, 0);
    }



    lv_label_set_text_fmt(
        ui.label_value,
        "%d",
        _value
    );
}
bool is_outbound = false;
int32_t arc_offset = 0;   // 超出界限时显示的 arch 长度
SuperDialMotion konb_direction = SUPER_DIAL_NULL;
int now_pos = 0;
int last_pos = 0;
int knob_value = 50;

void GetKnobStatus(PlaygroundInfo *info)
{
//    info->xkonb_value = knob_value;
    info->xkonb_value = knob_value;
    info->motor_pos = now_pos;
    info->angle_offset = arc_offset;
    info->konb_direction = konb_direction;
    konb_direction = SUPER_DIAL_NULL;
}
void onTimerUpdate(lv_timer_t* timer)
{
    PlaygroundInfo info;
    GetKnobStatus(&info);
    UpdateView(&info);
}
void main_screen_ui(void)
{

    /*Change the active screen's background color*/
//       lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xf03a57), LV_PART_MAIN);
//
//       /*Create a white label, set its text and align it to the center*/
//       lv_obj_t * label = lv_label_create(lv_scr_act());
//       lv_label_set_text(label, "smart knob");
//       lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
//       lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    Create(lv_scr_act());
    BoundZeroView();
   timer = lv_timer_create(onTimerUpdate, 10,NULL);
}

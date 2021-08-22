/**
 * @file 100ask_modify_picture.c
 *
 */

#include "../../lv_100ask_demo_theme/lv_100ask_demo_theme.h"

/**********************
 *	STATIC VARIABLES
 **********************/
static lv_obj_t * scan_img;
static uint16_t hue_act;
static int16_t lightness_act;


/**********************
 *  STATIC PROTOTYPES
 **********************/
LV_EVENT_CB_DECLARE(modify_picture_save_event_cb);
LV_EVENT_CB_DECLARE(back_to_home_event_cb);
LV_EVENT_CB_DECLARE(lightness_slider_event_cb);
LV_EVENT_CB_DECLARE(hue_slider_event_cb);

//static void pic_open(void);
static void scan_img_color_refr(void);
static void scan_anim_ready(lv_anim_t * a);

//LV_FONT_DECLARE();





void lv_100ask_modify_picture(void)
{

    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GRAY, LV_100ASK_DEMO_BG_NORMAL);  // 调整页面背景



    uint32_t delay = 100;

    lv_obj_t * back = add_back(back_to_home_event_cb);
    //lv_100ask_demo_anim_top_to_bottom(back, delay);

    lv_obj_t * title = add_title("ADJUST IMAGE");
    //lv_100ask_demo_anim_top_to_bottom(title, delay);
    //delay += LV_100ASK_DEMO_ANIM_DELAY;

    LV_IMG_DECLARE(lv_100ask_demo_img_scan_example);
    scan_img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(scan_img, &lv_100ask_demo_img_scan_example);
    lv_obj_align(scan_img, NULL, LV_ALIGN_IN_TOP_LEFT, 40, 100);
    lv_obj_set_style_local_radius(scan_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_clip_corner(scan_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, true);
    lv_obj_set_style_local_image_recolor_opa(scan_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 80);

    lv_coord_t box_w = 160;
    lv_obj_t * settings_box = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(settings_box, box_w, 245);
    lv_obj_align(settings_box, scan_img, LV_ALIGN_OUT_RIGHT_TOP, 40, 0);

    lightness_act = 0;
    hue_act = 180;
    LV_IMG_DECLARE(lv_100ask_demo_icon_bright);
    LV_IMG_DECLARE(lv_100ask_demo_icon_hue);

    lv_obj_t * slider = lv_slider_create(settings_box, NULL);
    lv_obj_set_size(slider, 8, 160);
    lv_obj_align(slider, NULL, LV_ALIGN_IN_TOP_MID, - 35, 65);
    lv_obj_set_event_cb(slider, lightness_slider_event_cb);
    lv_slider_set_range(slider, -80, 80);
    lv_slider_set_value(slider, 0, LV_ANIM_OFF);
    lv_obj_set_ext_click_area(slider, 30, 30, 30, 30);

    lv_obj_t * icon = lv_img_create(settings_box, NULL);
    lv_img_set_src(icon, &lv_100ask_demo_icon_bright);
    lv_obj_align_origo(icon, slider, LV_ALIGN_OUT_TOP_MID, 0, -30);

    slider = lv_slider_create(settings_box, slider);
    lv_obj_align(slider, NULL, LV_ALIGN_IN_TOP_MID, 35, 65);
    lv_obj_set_event_cb(slider, hue_slider_event_cb);
    lv_slider_set_range(slider, 0, 359);
    lv_slider_set_value(slider, 180, LV_ANIM_OFF);

    icon = lv_img_create(settings_box, NULL);
    lv_img_set_src(icon, &lv_100ask_demo_icon_hue);
    lv_obj_align_origo(icon, slider, LV_ALIGN_OUT_TOP_MID, 0, -30);

    scan_img_color_refr();

    lv_obj_t * next_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_theme_apply(next_btn, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_CIRCLE);
    lv_obj_set_size(next_btn, box_w, 60);
    lv_obj_align(next_btn, scan_img, LV_ALIGN_OUT_RIGHT_BOTTOM, 40, 0);

    lv_obj_set_event_cb(next_btn, modify_picture_save_event_cb);
    lv_obj_set_style_local_value_str(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "SAVE");
    lv_obj_set_style_local_value_font(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());
    lv_obj_set_style_local_bg_color(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_DEMO_GREEN);
    lv_obj_set_style_local_bg_color(next_btn, LV_OBJ_PART_MAIN, LV_STATE_PRESSED, lv_color_darken(LV_100ASK_DEMO_GREEN, LV_OPA_20));


    lv_100ask_demo_anim_top_to_bottom(scan_img, delay);
    delay += LV_100ASK_DEMO_ANIM_DELAY;

    lv_100ask_demo_anim_top_to_bottom(settings_box, delay);
    delay += 7;

    lv_100ask_demo_anim_top_to_bottom(next_btn, delay);
    delay += LV_100ASK_DEMO_ANIM_DELAY;

}


static void scan_img_color_refr(void)
{
    if(scan_img) {
        uint8_t s = lightness_act > 0 ? 100 - lightness_act : 100;
        uint8_t v = lightness_act < 0 ? 100 + lightness_act : 100;
        lv_color_t c = lv_color_hsv_to_rgb(hue_act, s, v);
        lv_obj_set_style_local_image_recolor(scan_img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, c);

    }
}



LV_EVENT_CB_DECLARE(lightness_slider_event_cb)
{
    if (e == LV_EVENT_VALUE_CHANGED) {
        lightness_act = lv_slider_get_value(obj);
        scan_img_color_refr();
    }
}


LV_EVENT_CB_DECLARE(hue_slider_event_cb)
{
    if (e == LV_EVENT_VALUE_CHANGED) {
        hue_act = lv_slider_get_value(obj);
        scan_img_color_refr();
    }
}




LV_EVENT_CB_DECLARE(modify_picture_save_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        scan_img = NULL;

        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GREEN, LV_100ASK_DEMO_BG_FULL);

        uint32_t delay = 200;

        LV_IMG_DECLARE(lv_100ask_demo_img_ready);
        lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_ready);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -40);

        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_100ask_demo_anim_top_to_bottom(img, delay);

        info_bottom_create("File saved", "CONTINUE", back_to_home_event_cb, delay);
    }
}



LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_WHITE, LV_100ASK_DEMO_BG_FULL);
        home_open(0);
    }
}





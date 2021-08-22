/**
 * @file 100ask_create_qrcode.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_100ask_demo_theme/lv_100ask_demo_theme.h"



/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *keyboard_data;
static lv_obj_t * qrcode_create;
static char * qrcode_data[128];


/**********************
 *  STATIC PROTOTYPES
 **********************/
LV_EVENT_CB_DECLARE(qrcode_icon_event_cb);
LV_EVENT_CB_DECLARE(creat_qrcode_event_cb);
LV_EVENT_CB_DECLARE(back_to_home_event_cb);

static void qrcode_open(void);
static void qrcode_anim_ready(lv_anim_t * a);





void lv_100ask_create_qrcode(void)
{
	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
	uint32_t delay = 200;
	lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GREEN, LV_100ASK_DEMO_BG_FULL);

	lv_obj_t * arc = add_loader(qrcode_anim_ready);
	lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, -40);

	lv_obj_t * txt = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(txt, "Opening, please wait...");
	lv_theme_apply(txt, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
	lv_obj_align(txt, arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 60);

	lv_100ask_demo_anim_top_to_bottom(arc, delay);
	delay += LV_100ASK_DEMO_ANIM_DELAY;
	lv_100ask_demo_anim_top_to_bottom(txt, delay);


}


static void qrcode_anim_ready(lv_anim_t * a)
{

    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    qrcode_open();  // 页面处理
}


static void qrcode_open(void)
{
	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GRAY, LV_100ASK_DEMO_BG_SMALL);  // 调整页面背景

    uint32_t delay = 200;

    lv_obj_t * back = add_back(back_to_home_event_cb);
    lv_100ask_demo_anim_top_to_bottom(back, delay);

    lv_obj_t * title = add_title("CREAT QRCODE");
    lv_100ask_demo_anim_top_to_bottom(title, delay);

	/* 二维码 */
	//strcpy(qrcode_data, "https://www.100ask.net");
	/*Create a 100x100 QR code*/
	qrcode_create = lv_qrcode_create(lv_scr_act(), 200, lv_color_hex3(0x33f), lv_color_hex3(0xeef));
	lv_obj_align(qrcode_create, NULL, LV_ALIGN_CENTER, -300, 0);
	lv_qrcode_update(qrcode_create, "https://www.100ask.net", strlen("https://www.100ask.net"));


#if 1
	/*Create a keyboard and apply the styles*/
    lv_obj_t *kb = lv_keyboard_create(lv_scr_act(), NULL);
    lv_keyboard_set_cursor_manage(kb, true);

    /*Create a text area. The keyboard will write here*/
    keyboard_data = lv_textarea_create(lv_scr_act(), NULL);
	lv_obj_align(keyboard_data, NULL, LV_ALIGN_IN_TOP_MID, 100, 150);
    lv_textarea_set_text(keyboard_data, "https://www.100ask.net");
    lv_coord_t max_h = LV_VER_RES / 2 - LV_DPI / 8;
    if(lv_obj_get_height(keyboard_data) > max_h) lv_obj_set_height(keyboard_data, max_h);

    /*Assign the text area to the keyboard*/
    lv_keyboard_set_textarea(kb, keyboard_data);

#endif


	lv_obj_t * next_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_theme_apply(next_btn, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_CIRCLE);
    lv_obj_set_size(next_btn, 160, 60);
    lv_obj_align(next_btn, keyboard_data, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_obj_set_event_cb(next_btn, creat_qrcode_event_cb);
    lv_obj_set_style_local_value_str(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "CREAT");
    lv_obj_set_style_local_value_font(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());

}

LV_EVENT_CB_DECLARE(creat_qrcode_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
			lv_qrcode_update(qrcode_create, lv_textarea_get_text(keyboard_data), strlen(lv_textarea_get_text(keyboard_data)));
            printf("Hello\n");
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





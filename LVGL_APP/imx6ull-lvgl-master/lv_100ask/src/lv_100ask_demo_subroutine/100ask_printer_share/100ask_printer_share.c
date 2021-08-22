/**
 * @file 100ask_priner_share.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
 #include "../../lv_100ask_demo_theme/lv_100ask_demo_theme.h"


 /*********************
 *      DEFINES
 *********************/



 /**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * print_cnt_label;
static lv_color_t bg_color_act;
static lv_color_t bg_color_act;
static lv_color_t bg_color_prev;

static uint16_t print_cnt;


 /**********************
 *  STATIC PROTOTYPES
 **********************/
LV_EVENT_CB_DECLARE(usb_icon_event_cb);
LV_EVENT_CB_DECLARE(mobile_icon_event_cb);
LV_EVENT_CB_DECLARE(internet_icon_event_cb);
LV_EVENT_CB_DECLARE(print_cnt_bnt_event_cb);
LV_EVENT_CB_DECLARE(print_start_event_cb);
LV_EVENT_CB_DECLARE(back_to_print_event_cb);


LV_EVENT_CB_DECLARE(back_to_home_event_cb);

static void print_start_ready(lv_anim_t * a);
static void lv_demo_printer_anim_in(lv_obj_t * obj, uint32_t delay);
static void lv_demo_printer_anim_bg(uint32_t delay, lv_color_t color, int32_t y_new);
static lv_obj_t * add_icon(lv_obj_t * parent, const void * src_bg, const void * src_icon, const char * txt);
static lv_anim_value_t anim_path_triangle(const lv_anim_path_t * path, const lv_anim_t * a);
static void anim_bg_color_cb(lv_anim_t * a, lv_anim_value_t v);



void lv_100ask_printer_share(uint32_t delay)
{
    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_NORMAL); // µ÷ÕûÒ³Ãæ±³¾°

    lv_obj_t * back = add_back(back_to_home_event_cb);
    lv_demo_printer_anim_in(back, delay);


    lv_obj_t * title = add_title("PRINT MENU");
    lv_demo_printer_anim_in(title, delay);

    lv_coord_t box_w = 720;
    lv_obj_t * box = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(box, box_w, 260);
    lv_obj_align(box, NULL, LV_ALIGN_IN_TOP_MID, 0, 100);
    //lv_obj_align(box, NULL, 0, 0, 800);

    delay += LV_100ASK_DEMO_ANIM_DELAY;
    lv_demo_printer_anim_in(box, delay);

    LV_IMG_DECLARE(lv_100ask_demo_img_usb);
    LV_IMG_DECLARE(lv_100ask_demo_img_mobile);
    LV_IMG_DECLARE(lv_100ask_demo_img_internet);
    LV_IMG_DECLARE(lv_100ask_demo_img_btn_bg_2);
    LV_IMG_DECLARE(lv_100ask_demo_img_btn_bg_3);
    LV_IMG_DECLARE(lv_100ask_demo_img_btn_bg_4);

    lv_obj_t * icon;
    icon = add_icon(box, &lv_100ask_demo_img_btn_bg_2, &lv_100ask_demo_img_usb, "USB");
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_LEFT_MID, 1 * box_w / 6, -15);
    lv_obj_set_event_cb(icon, usb_icon_event_cb);
    lv_obj_fade_in(icon, LV_100ASK_DEMO_ANIM_TIME * 2, delay + LV_100ASK_DEMO_ANIM_TIME + 50);

    icon = add_icon(box, &lv_100ask_demo_img_btn_bg_3, &lv_100ask_demo_img_mobile, "MOBILE");
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_LEFT_MID, 3 * box_w / 6, -15);
    lv_obj_set_event_cb(icon, mobile_icon_event_cb);
    lv_obj_fade_in(icon, LV_100ASK_DEMO_ANIM_TIME * 2, delay + LV_100ASK_DEMO_ANIM_TIME + 50);

    icon = add_icon(box, &lv_100ask_demo_img_btn_bg_4, &lv_100ask_demo_img_internet, "INTERNET");
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_LEFT_MID, 5 * box_w / 6, -15);
    lv_obj_set_event_cb(icon, internet_icon_event_cb);
    lv_obj_fade_in(icon, LV_100ASK_DEMO_ANIM_TIME * 2, delay + LV_100ASK_DEMO_ANIM_TIME + 50);

    box = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(box, box_w, 80);
    lv_obj_align(box, NULL, LV_ALIGN_IN_BOTTOM_MID, 0,
            - LV_HOR_RES / 15);
    lv_obj_set_style_local_value_str(box, LV_CONT_PART_MAIN, LV_STATE_DEFAULT,
            "From where do you want to print?");

    delay += LV_100ASK_DEMO_ANIM_DELAY;
    lv_demo_printer_anim_in(box, delay);

}



static void print_start_ready(lv_anim_t * a)
{
    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GREEN, LV_100ASK_DEMO_BG_FULL);
    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

    LV_IMG_DECLARE(lv_100ask_demo_img_ready);
    lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img, &lv_100ask_demo_img_ready);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -40);

    uint32_t delay = 200;
    lv_demo_printer_anim_in(img, delay);
    delay += LV_100ASK_DEMO_ANIM_DELAY;

    info_bottom_create("Printing finished", "CONTINUE", back_to_print_event_cb, delay);
}


static void lv_demo_printer_anim_in(lv_obj_t * obj, uint32_t delay)
{
    if (obj != lv_scr_act()) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(obj) - LV_100ASK_DEMO_ANIM_Y,
                lv_obj_get_y(obj));
        lv_anim_start(&a);

        lv_obj_fade_in(obj, LV_100ASK_DEMO_ANIM_TIME - 50, delay);
    }
}



static lv_obj_t * add_icon(lv_obj_t * parent, const void * src_bg, const void * src_icon, const char * txt)
{

    lv_obj_t * bg = lv_img_create(parent, NULL);
    lv_obj_set_click(bg, true);
    lv_theme_apply(bg, (lv_theme_style_t)LV_100ASK_DEMO_THEME_ICON);
    lv_img_set_src(bg, src_bg);
    lv_img_set_antialias(bg, false);

    lv_obj_t * icon = lv_img_create(bg, NULL);
    lv_img_set_src(icon, src_icon);
    lv_obj_set_style_local_image_recolor_opa(icon, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
    lv_obj_align(icon, NULL, LV_ALIGN_IN_TOP_RIGHT, -30, 30);

    lv_obj_t * label = lv_label_create(bg, NULL);
    lv_label_set_text(label, txt);
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 30, -30);


    return bg;
}




static void lv_demo_printer_anim_bg(uint32_t delay, lv_color_t color, int32_t y_new)
{
    lv_coord_t y_act = lv_obj_get_y(lv_scr_act());
    lv_color_t act_color = lv_obj_get_style_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN);
    if(y_new != LV_100ASK_DEMO_BG_NORMAL && y_new == y_act && act_color.full == color.full) return;


    lv_anim_t a;
    if((y_new == LV_100ASK_DEMO_BG_NORMAL && y_new == y_act) ||
            (y_new == LV_100ASK_DEMO_BG_NORMAL && y_act == LV_100ASK_DEMO_BG_FULL)) {
        lv_anim_path_t path;
        lv_anim_path_init(&path);
        lv_anim_path_set_cb(&path, anim_path_triangle);

        lv_anim_init(&a);
        lv_anim_set_var(&a, lv_scr_act());
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_BG + 200);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, y_act, y_new);
        lv_anim_set_path(&a, &path);
        lv_anim_start(&a);
    } else {
        lv_anim_init(&a);
        lv_anim_set_var(&a, lv_scr_act());
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_BG);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(lv_scr_act()), y_new);
        lv_anim_start(&a);
    }

    bg_color_prev = bg_color_act;
    bg_color_act = color;

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) anim_bg_color_cb);
    lv_anim_set_values(&a, 0, 255);
    lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_BG);
    lv_anim_set_path(&a, &lv_anim_path_def);
    lv_anim_start(&a);

}



/**
 * Calculate the current value of an animation applying linear characteristic
 * @param a pointer to an animation
 * @return the current value to set
 */
static lv_anim_value_t anim_path_triangle(const lv_anim_path_t * path, const lv_anim_t * a)
{
    /*Calculate the current step*/
    uint32_t step;
    lv_anim_value_t ret = 0;
    if(a->time == a->act_time) {
        ret = (lv_anim_value_t)a->end;
    }
    else {
        if(a->act_time < a->time / 2) {
            step = ((int32_t)a->act_time * 1024) / (a->time / 2);
            int32_t new_value;
            new_value = (int32_t)step * (LV_100ASK_DEMO_BG_SMALL - a->start);
            new_value = new_value >> 10;
            new_value += a->start;

            ret = (lv_anim_value_t)new_value;
        } else {
            uint32_t t = a->act_time - a->time / 2;
            step = ((int32_t)t * 1024) / (a->time / 2);
            int32_t new_value;
            new_value = (int32_t)step * (a->end - LV_100ASK_DEMO_BG_SMALL);
            new_value = new_value >> 10;
            new_value += LV_100ASK_DEMO_BG_SMALL;

            ret = (lv_anim_value_t)new_value;
        }
    }

    return ret;
}




LV_EVENT_CB_DECLARE(usb_icon_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_NORMAL);

        uint32_t delay = 200;

        lv_obj_t * back = add_back(back_to_print_event_cb);
        lv_demo_printer_anim_in(back, delay);

        lv_obj_t * title = add_title("PRINTING FROM USB DRIVE");
        lv_demo_printer_anim_in(title, delay);

        lv_coord_t box_w = (LV_HOR_RES * 5) / 10;
        lv_obj_t * list = lv_list_create(lv_scr_act(), NULL);
        lv_obj_set_size(list, box_w, LV_VER_RES / 2);
        lv_obj_align(list, NULL, LV_ALIGN_IN_TOP_LEFT, LV_HOR_RES / 20, LV_VER_RES / 5);

        const char * dummy_file_list[] = { "Contract 12.pdf", "Scanned_05_21.pdf", "Photo_132210.jpg", "Photo_232141.jpg",
             "Photo_091640.jpg", "Photo_124019.jpg", "Photo_232032.jpg", "Photo_232033.jpg", "Photo_232034.jpg",
             "Monday schedule.pdf", "Email from John.txt", "New file.txt", "Untitled.txt", "Untitled (1).txt", "Gallery_40.jpg",
             "Gallery_41.jpg", "Gallery_42.jpg", "Gallery_43.jpg", "Gallery_44.jpg" };


        uint32_t i;
        for(i = 0; i < sizeof(dummy_file_list) / sizeof(dummy_file_list[0]); i++) {
         lv_obj_t * btn = lv_list_add_btn(list, LV_SYMBOL_FILE, dummy_file_list[i]);
         lv_btn_set_checkable(btn, true);
        }

        lv_obj_t * dropdown_box = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_set_size(dropdown_box, box_w, LV_VER_RES / 5);
        lv_obj_align(dropdown_box, list, LV_ALIGN_OUT_BOTTOM_MID, 0, LV_HOR_RES / 30);

        lv_obj_t * dropdown = lv_dropdown_create(dropdown_box, NULL);
        lv_obj_align(dropdown, NULL, LV_ALIGN_IN_LEFT_MID, LV_HOR_RES / 60, 0);
        lv_dropdown_set_max_height(dropdown, LV_VER_RES / 3);
        lv_dropdown_set_options_static(dropdown, "Best\nNormal\nDraft");
        lv_obj_set_width(dropdown, (box_w - 3 * LV_HOR_RES / 60) / 2);
        lv_obj_set_ext_click_area(dropdown, 5, 5, 5, 5);

        dropdown = lv_dropdown_create(dropdown_box, dropdown);
        lv_obj_align(dropdown, NULL, LV_ALIGN_IN_RIGHT_MID, - LV_HOR_RES / 60, 0);
        lv_dropdown_set_options_static(dropdown, "100 DPI\n200 DPI\n300 DPI\n400 DPI\n500 DPI\n1500 DPI");

        box_w = 320 - 40;
        lv_obj_t * settings_box = lv_obj_create(lv_scr_act(), NULL);
        lv_obj_set_size(settings_box, box_w, LV_VER_RES / 2);
        lv_obj_align(settings_box, list, LV_ALIGN_OUT_RIGHT_TOP, LV_HOR_RES / 20, 0);

        print_cnt = 1;
        lv_obj_t * numbox = lv_cont_create(settings_box, NULL);
        lv_theme_apply(numbox, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BOX_BORDER);
        lv_obj_set_size(numbox, LV_HOR_RES / 7, LV_HOR_RES / 13);
        lv_obj_align(numbox, settings_box, LV_ALIGN_IN_TOP_MID, 0, LV_VER_RES / 10);
        lv_obj_set_style_local_value_str(numbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "Copies");
        lv_obj_set_style_local_value_align(numbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_MID);
        lv_obj_set_style_local_value_ofs_y(numbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, - LV_VER_RES / 50);
        lv_obj_set_style_local_value_font(numbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());
        lv_cont_set_layout(numbox, LV_LAYOUT_CENTER);

        print_cnt_label = lv_label_create(numbox, NULL);
        lv_label_set_text(print_cnt_label, "1");
        lv_obj_set_style_local_text_font(print_cnt_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_title());

        lv_obj_t * btn = lv_btn_create(settings_box, NULL);
        lv_obj_set_size(btn, LV_HOR_RES / 13, LV_HOR_RES / 13);
        lv_obj_align(btn, numbox, LV_ALIGN_OUT_LEFT_MID, - LV_VER_RES / 60, 0);
        lv_obj_set_style_local_value_str(btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_DOWN);
        lv_obj_set_event_cb(btn, print_cnt_bnt_event_cb);
        lv_obj_set_ext_click_area(btn, 10, 10, 10, 10);

        lv_obj_t * sw = lv_switch_create(settings_box, NULL);
        lv_obj_set_size(sw, LV_HOR_RES / 10, LV_VER_RES / 12);
        lv_obj_align(sw, btn, LV_ALIGN_OUT_BOTTOM_LEFT, LV_HOR_RES / 50, LV_VER_RES / 7);
        lv_obj_set_style_local_value_ofs_y(sw, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, - LV_VER_RES / 50);
        lv_obj_set_style_local_value_align(sw, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_MID);
        lv_obj_set_style_local_value_str(sw, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "Color");
        lv_obj_set_style_local_value_font(sw, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());

        btn = lv_btn_create(settings_box, btn);
        lv_obj_align(btn, numbox, LV_ALIGN_OUT_RIGHT_MID, LV_VER_RES / 60, 0);
        lv_obj_set_style_local_value_str(btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_UP);

        sw = lv_switch_create(settings_box, sw);
        lv_obj_align(sw, btn, LV_ALIGN_OUT_BOTTOM_RIGHT, - LV_HOR_RES / 50, LV_VER_RES / 7);
        lv_obj_set_style_local_value_str(sw, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "Vertical");

        lv_obj_t * print_btn = lv_btn_create(lv_scr_act(), NULL);
        lv_theme_apply(print_btn, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_CIRCLE);
        lv_obj_set_size(print_btn, box_w, 60);

        lv_coord_t btn_ofs_y = (lv_obj_get_height(dropdown_box) - lv_obj_get_height(print_btn)) / 2;
        lv_obj_align(print_btn, settings_box, LV_ALIGN_OUT_BOTTOM_MID, 0, LV_HOR_RES / 30 + btn_ofs_y);
        lv_obj_set_style_local_value_str(print_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "PRINT");
        lv_obj_set_style_local_value_font(print_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());
        lv_obj_set_style_local_bg_color(print_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_DEMO_GREEN);
        lv_obj_set_style_local_bg_color(print_btn, LV_OBJ_PART_MAIN, LV_STATE_PRESSED, lv_color_darken(LV_100ASK_DEMO_GREEN, LV_OPA_20));
        lv_obj_set_event_cb(print_btn, print_start_event_cb);

        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_demo_printer_anim_in(list, delay);

        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_demo_printer_anim_in(settings_box, delay);

        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_demo_printer_anim_in(dropdown_box, delay);

        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_demo_printer_anim_in(print_btn, delay);

        //lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_NORMAL);

    }
}




LV_EVENT_CB_DECLARE(internet_icon_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_RED, LV_100ASK_DEMO_BG_FULL);

        uint32_t delay = 200;

        LV_IMG_DECLARE(lv_100ask_demo_img_printer2);

        lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_printer2);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, -90, 0);

        lv_demo_printer_anim_in(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_no_internet);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_no_internet);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -40);

        lv_demo_printer_anim_in(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_cloud);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_cloud);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 80, -80);

        lv_demo_printer_anim_in(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        info_bottom_create("No internet connection", "BACK", back_to_print_event_cb, delay);
    }

    //icon_generic_event_cb(obj, e);
}


LV_EVENT_CB_DECLARE(mobile_icon_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_FULL);

        uint32_t delay = 200;

        LV_IMG_DECLARE(lv_100ask_demo_img_printer2);

        lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_printer2);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, -90, 0);

        lv_demo_printer_anim_in(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_wave);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_wave);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

        lv_demo_printer_anim_in(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_phone);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_phone);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 80, 0);

        lv_demo_printer_anim_in(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        info_bottom_create("Put you phone near to the printer", "BACK", back_to_print_event_cb, delay);
    }

    //icon_generic_event_cb(obj, e);
}



LV_EVENT_CB_DECLARE(print_cnt_bnt_event_cb)
{
    if (e == LV_EVENT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        const char * txt = lv_obj_get_style_value_str(obj, LV_BTN_PART_MAIN);
        if(strcmp(txt, LV_SYMBOL_DOWN) == 0) {
            if(print_cnt > 1) print_cnt--;
        } else {
            if(print_cnt < 1000) print_cnt ++;
        }
        lv_label_set_text_fmt(print_cnt_label, "%d", print_cnt);
    }
}


LV_EVENT_CB_DECLARE(print_start_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        uint32_t delay = 200;
        lv_100ask_demo_anim_bg(150, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_FULL);

        lv_obj_t * arc = add_loader(print_start_ready);
        lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, -40);

        lv_obj_t * txt = lv_label_create(lv_scr_act(), NULL);
        lv_label_set_text(txt, "Printing, please wait...");
        lv_theme_apply(txt, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
        lv_obj_align(txt, arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 60);

        lv_demo_printer_anim_in(arc, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_demo_printer_anim_in(txt, delay);
    }
}

LV_EVENT_CB_DECLARE(back_to_print_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_printer_share(150);
    }
}


static void anim_bg_color_cb(lv_anim_t * a, lv_anim_value_t v)
{
    lv_color_t c = lv_color_mix(bg_color_act, bg_color_prev, v);
    lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, c);
}



LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
		//play_game = false;
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_WHITE, LV_100ASK_DEMO_BG_FULL);
        home_open(0);
    }
}









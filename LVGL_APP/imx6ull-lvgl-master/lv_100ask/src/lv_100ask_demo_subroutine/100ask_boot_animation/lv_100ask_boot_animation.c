/**
 * @file lv_100ask_boot_animation.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_100ask_boot_animation.h"

//#if LV_USE_100ASK_BOOT_ANIAMTION
#if 1

/*********************
 *      DEFINES
 *********************/
#define ACTIVE_TRACK_CNT       3
#define INTRO_TIME      2000
#define BAR_COLOR1      lv_color_hex(0xe9dbfc)
#define BAR_COLOR2      lv_color_hex(0x6f8af6)
#define BAR_COLOR3      LV_COLOR_WHITE
#define BAR_COLOR1_STOP     80
#define BAR_COLOR2_STOP     100
#define BAR_COLOR3_STOP     (2 * LV_HOR_RES / 3)
#define BAR_CNT          20
#define DEG_STEP         (180/BAR_CNT)
#define BAND_CNT         4
#define BAR_PER_BAND_CNT  (BAR_CNT / BAND_CNT)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/


 /**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * main_cont;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/



void lv_100ask_boot_animation(void)
{
#if 0
    /*Create an empty white main container*/
    main_cont = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(main_cont, LV_HOR_RES, LV_VER_RES + 30);
    lv_obj_set_style_local_bg_color(main_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_radius(main_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 15);
    lv_obj_set_style_local_border_width(main_cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_add_protect(main_cont, LV_PROTECT_PRESS_LOST);
    lv_obj_set_drag(main_cont, true);
    lv_obj_set_drag_throw(main_cont, true);
    lv_obj_set_drag_dir(main_cont, LV_DRAG_DIR_VER);
    lv_obj_set_y(main_cont, -15);
    //ancestor_signal_cb = lv_obj_get_signal_cb(main_cont);
#endif


    /* Create an intro from a logo + label */
	LV_IMG_DECLARE(img_lv_100ask_demo_logo);
	lv_obj_t * logo = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(logo, &img_lv_100ask_demo_logo);
	lv_obj_align(logo, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_move_foreground(logo);

    lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(title, "100ASK Lvgl Demo\nwww.100ask.net");
	lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
	lv_obj_set_style_local_text_font(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_30);
	lv_obj_set_style_local_text_line_space(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, 8);
	lv_obj_align(title, logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_obj_fade_out(title, 300, INTRO_TIME);
	lv_obj_fade_out(logo, 300, INTRO_TIME);


}





/**********************
 *   STATIC FUNCTIONS
 **********************/


 #endif

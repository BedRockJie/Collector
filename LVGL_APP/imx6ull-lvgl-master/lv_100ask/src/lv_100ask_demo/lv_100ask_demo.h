/**
 * @file lv_100ask_demo.h
 *
 */

#ifndef LV_100ASK_DEMO_H
#define LV_100ASK_DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_100ask.h"

/*********************
 *      DEFINES
 *********************/
/*Bg positions*/
#define LV_100ASK_DEMO_BG_NONE 					(-LV_VER_RES)
#define LV_100ASK_DEMO_BG_FULL 					(0)
#define LV_100ASK_DEMO_BG_NORMAL 				(-2 * (LV_VER_RES / 3))
#define LV_100ASK_DEMO_BG_SMALL 				(-5 * (LV_VER_RES / 6))


/*Animations*/
#define LV_100ASK_DEMO_ANIM_X 					(LV_HOR_RES / 40)
#define LV_100ASK_DEMO_ANIM_Y 					(LV_VER_RES / 40)
#define LV_100ASK_DEMO_ANIM_DELAY 				(40)
#define LV_100ASK_DEMO_ANIM_TIME  				(150)
#define LV_100ASK_DEMO_ANIM_TIME_HOME  			(500)
#define LV_100ASK_DEMO_ANIM_TIME_BG  			(150)


/**********************
 *      TYPEDEFS
 **********************/

/**********************LV_100ASK_DEMO_ANIM_TIME_HOME
 * GLOBAL PROTOTYPES
 **********************/
void home_open(uint32_t delay);
void home_open2(uint32_t delay);

void lv_100ask_printer(lv_indev_t * indev);
//void lv_100ask_demo_anim_in(lv_obj_t * obj, uint32_t delay);
void lv_100ask_demo_anim_out_all(lv_obj_t * obj, uint32_t delay);
void lv_100ask_demo_anim_bg(uint32_t delay, lv_color_t color, int32_t y_new);

void lv_100ask_demo_anim_top_to_bottom(lv_obj_t * obj, uint32_t delay);
void lv_100ask_demo_anim_left_to_right(lv_obj_t * obj, uint32_t delay);
void lv_100ask_demo_anim_right_to_left(lv_obj_t * obj, uint32_t delay);
void lv_100ask_demo_anim_path(lv_obj_t * obj, uint32_t delay);  // TODO



lv_obj_t * add_title(const char * txt);
lv_obj_t * add_back(lv_event_cb_t event_cb);
lv_obj_t * add_loader(void (*end_cb)(lv_anim_t *));

void info_bottom_create(const char * dsc, const char * btn_txt, lv_event_cb_t btn_event_cb, uint32_t delay);


void ask100_show_pic_stack(lv_task_t * task);
void ask100_update_time_task(lv_task_t * task);



/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_100ASK_DEMO*/

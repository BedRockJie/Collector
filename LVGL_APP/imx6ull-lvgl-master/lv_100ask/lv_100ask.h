/**
 * @file lv_100ask.h
 *
 */

#ifndef LV_EXAMPLES_H
#define LV_EXAMPLES_H

#define LV_USE_100ASK_DEMO 1

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#if defined(LV_EX_CONF_PATH)
#define __LV_TO_STR_AUX(x) #x
#define __LV_TO_STR(x) __LV_TO_STR_AUX(x)
#include __LV_TO_STR(LV_EX_CONF_PATH)
#undef __LV_TO_STR_AUX
#undef __LV_TO_STR
#elif defined(LV_EX_CONF_INCLUDE_SIMPLE)
#include "lv_100ask_conf.h"
#else
#include "../lv_100ask_conf.h"
#endif


//#include "src/lv_100ask_demo/lv_100ask_demo.h"
#include "src/lv_100ask_demo_subroutine/100ask_music_player/100ask_music.h"

/*********************
 *      DEFINES
 *********************/
/*Test  lvgl version*/
#if LV_VERSION_CHECK(7, 7, 1) == 0
#error "lv_100ask: Wrong lvgl version"
#endif


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_EXAMPLES_H*/

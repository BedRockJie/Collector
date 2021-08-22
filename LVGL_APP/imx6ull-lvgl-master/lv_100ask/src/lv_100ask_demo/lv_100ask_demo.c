/**
 * @file lv_100ask_demo.c
 *
 */

#define _GNU_SOURCE
# define __USE_MISC 1
#define _BSD_SOURCE  1
#define _SVID_SOURCE 1

/*********************
 *      INCLUDES
 *********************/
#include <time.h>
#include <stdio.h>
#include <dirent.h>

#include <string.h>

#include "../lv_100ask_demo_theme/lv_100ask_demo_theme.h"



// TODO lvgl内置图标
// #define LV_SYMBOL_WIFI            "\xef\x87\xab" /*61931, 0xF1EB*/

/*********************
 *      DEFINES
 *********************/

// 桌面画框颜色
#define LV_100ASK_COLOR_1 LV_COLOR_MAKE(0x00, 0x78, 0xD7)
#define LV_100ASK_COLOR_2 LV_COLOR_MAKE(0x55, 0x1C, 0x39)
#define LV_100ASK_COLOR_3 LV_COLOR_MAKE(0x00, 0x78, 0xD7)
#define LV_100ASK_COLOR_4 LV_COLOR_MAKE(0x16, 0x23, 0x4B)
#define LV_100ASK_COLOR_5 LV_COLOR_MAKE(0x00, 0x78, 0xD7)
#define LV_100ASK_COLOR_6 LV_COLOR_MAKE(0x00, 0x47, 0xDB)
#define LV_100ASK_COLOR_7 LV_COLOR_MAKE(0x00, 0x78, 0xD7)
#define LV_100ASK_COLOR_8 LV_COLOR_MAKE(0x17, 0x00, 0x53)
#define LV_100ASK_COLOR_9 LV_COLOR_MAKE(0x00, 0x78, 0xEE)
#define LV_100ASK_COLOR_10 LV_COLOR_MAKE(0x73, 0x2E, 0xFB)
#define LV_100ASK_COLOR_11 LV_COLOR_MAKE(0x73, 0x2E, 0xFB)


/*Bg positions*/
#define LV_100ASK_DEMO_BG_NONE 					(-LV_VER_RES)
#define LV_100ASK_DEMO_BG_FULL 					(0)
#define LV_100ASK_DEMO_BG_NORMAL 				(-2 * (LV_VER_RES / 3))
#define LV_100ASK_DEMO_BG_SMALL 				(-5 * (LV_VER_RES / 6))

/* Desktop Sizes */
#define LV_100ASK_DEMO_BTN_H   					(50)
#define LV_100ASK_DEMO_BTN_W   					(200)
#define LV_100ASK_DEMO_BIG_BOX 					(200)
#define LV_100ASK_DEMO_FRAME_H					(170)
#define LV_100ASK_DEMO_BIG_FRAME_W				(370)
#define LV_100ASK_DEMO_SMALL_FRAME_W 			(180)
#define LV_100ASK_DEMO_SMALL_LARGER_FRAME_W 	(183)

/*Padding*/
#define LV_100ASK_DEMO_TITLE_PAD 				(35)


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
// 触发事件回调函数       TODO
// #define LV_EVENT_CB_DECLARE(name) static void name(lv_obj_t * obj, lv_event_t e)

LV_EVENT_CB_DECLARE(back_to_home_event_cb);

LV_EVENT_CB_DECLARE(modify_picture_save_event_cb);
LV_EVENT_CB_DECLARE(scan_ready_event_cb);

LV_EVENT_CB_DECLARE(copy_open_icon_event_cb);
//LV_EVENT_CB_DECLARE(scan_next_event_cb);
//LV_EVENT_CB_DECLARE(hue_slider_event_cb);
//LV_EVENT_CB_DECLARE(lightness_slider_event_cb);
LV_EVENT_CB_DECLARE(usb_icon_event_cb);
LV_EVENT_CB_DECLARE(print_cnt_bnt_event_cb);
LV_EVENT_CB_DECLARE(print_start_event_cb);
LV_EVENT_CB_DECLARE(back_to_print_event_cb);
LV_EVENT_CB_DECLARE(mobile_icon_event_cb);
LV_EVENT_CB_DECLARE(internet_icon_event_cb);
LV_EVENT_CB_DECLARE(setup_icon_event_cb);
LV_EVENT_CB_DECLARE(icon_generic_event_cb);

LV_EVENT_CB_DECLARE(icon_100ask_card_test_event_cb);    //注册回调函数

LV_EVENT_CB_DECLARE(icon_100ask_modify_picture_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_game_2048_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_create_qrcode_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_file_browser_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_gallery_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_music_player_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_print_open_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_demo_widgets_event_cb);
LV_EVENT_CB_DECLARE(icon_100ask_demo_benchmark_event_cb);

lv_demo_benchmark();


static void print_open(uint32_t delay);
static void print_start_ready(lv_anim_t * a);
static lv_obj_t * add_canvas(lv_obj_t * parent, lv_color_t * cbuf, char * icon_file,\
								 uint16_t canvas_w, uint16_t canvas_h, \
								 uint16_t rect_dsc_x, uint16_t rect_dsc_y, \
								 lv_color_t rgb32);
static lv_obj_t * add_icon(lv_obj_t * parent, const void * src_bg, const void * src_icon, const char * txt) ;
static void loader_anim_cb(void * arc, lv_anim_value_t v);
static void scan_img_color_refr(void);
static lv_anim_value_t anim_path_triangle(const lv_anim_path_t * path, const lv_anim_t * a);
static void anim_bg_color_cb(lv_anim_t * a, lv_anim_value_t v);

static void lv_demo_printer_anim_out_all(lv_obj_t * obj, uint32_t delay);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_indev_t * my_indev;
static lv_obj_t * systime_task_label;
static lv_obj_t * sysdate_task_label;
static lv_obj_t * desktop_page;
static lv_obj_t * bg_top;
static lv_obj_t * bg_bottom;
static lv_obj_t * print_cnt_label;
static lv_color_t bg_color_prev;
static lv_color_t bg_color_act;
static lv_obj_t * ask100_show_pic;

static lv_obj_t * canvas_letf_big_window;
static lv_coord_t box_w = 200;;
static lv_obj_t * bar;

static uint16_t print_cnt;
static bool in_desktop_page;
static char * show_pic_dir[128];

static void lv_100ask_start_animation(void);




#define CANVAS_BOTTOM_MENU_WIDTH        700
#define CANVAS_BOTTOM_MENU_HEIGHT       80
#define CANVAS_BOTTOM_MENU_ITEM         60
#define CANVAS_LEFT_SMALL_WINDOW        120
#define CANVAS_LEFT_BIG_WINDOW_WIDTH    260
#define CANVAS_LEFT_BIG_WINDOW_HEIGHT   120
#define CANVAS_LEFT_LINE_WIDTH          260
#define CANVAS_LEFT_LINE_HEIGHT         40
#define CANVAS_MENU_ITEM                64

#define BOTTOM_MENU_ITEM_NUM            9
#define MENU_ITEM_NUM                   18
#define LV_100ASK_DEMO_ANIM_X 	        (LV_HOR_RES / 40)



/* 轮播图声明 */
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_0);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_1);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_2);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_3);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_4);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_5);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_6);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_7);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_8);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_9);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_10);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_11);
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_12);

/* 轮播图数组 */
static const lv_img_dsc_t *img_rotate_pictures[] = \
                                         {&img_lv_100ask_rotate_pictures_0, &img_lv_100ask_rotate_pictures_1,\
                                          &img_lv_100ask_rotate_pictures_2, &img_lv_100ask_rotate_pictures_3,\
                                          &img_lv_100ask_rotate_pictures_4, &img_lv_100ask_rotate_pictures_5,\
                                          &img_lv_100ask_rotate_pictures_6, &img_lv_100ask_rotate_pictures_7,\
                                          &img_lv_100ask_rotate_pictures_8, &img_lv_100ask_rotate_pictures_9,\
                                          &img_lv_100ask_rotate_pictures_10, &img_lv_100ask_rotate_pictures_11,\
                                          &img_lv_100ask_rotate_pictures_12
                                            };



#if 1
/* 桌标图标数组，与回调函数指针数组一一对应 */
LV_IMG_DECLARE(img_lv_100ask_icon_2048);
LV_IMG_DECLARE(img_lv_100ask_icon_about_us);
LV_IMG_DECLARE(img_lv_100ask_icon_camera);
LV_IMG_DECLARE(img_lv_100ask_icon_data);
LV_IMG_DECLARE(img_lv_100ask_icon_floder);
LV_IMG_DECLARE(img_lv_100ask_icon_gallery);
LV_IMG_DECLARE(img_lv_100ask_icon_modify_picture);
LV_IMG_DECLARE(img_lv_100ask_icon_music);
LV_IMG_DECLARE(img_lv_100ask_icon_paylater);
LV_IMG_DECLARE(img_lv_100ask_icon_QRcode);   // 10
LV_IMG_DECLARE(img_lv_100ask_icon_scanning);
LV_IMG_DECLARE(img_lv_100ask_icon_set);
LV_IMG_DECLARE(img_lv_100ask_icon_txt);
LV_IMG_DECLARE(img_lv_100ask_icon_video_play);
LV_IMG_DECLARE(img_lv_100ask_icon_calc);
LV_IMG_DECLARE(img_lv_100ask_icon_position);
LV_IMG_DECLARE(img_lv_100ask_icon_printer);
LV_IMG_DECLARE(img_lv_100ask_icon_widgets);   // 18

static const lv_img_dsc_t *app_icon[] = \
                                         {&img_lv_100ask_icon_modify_picture,  &img_lv_100ask_icon_floder,\
                                          &img_lv_100ask_icon_QRcode,          &img_lv_100ask_icon_gallery,\
                                          &img_lv_100ask_icon_music,           &img_lv_100ask_icon_2048,\
                                          &img_lv_100ask_icon_widgets,         &img_lv_100ask_icon_printer,\
                                          &img_lv_100ask_icon_calc,            &img_lv_100ask_icon_about_us,\
                                          &img_lv_100ask_icon_video_play,      &img_lv_100ask_icon_txt,\
                                          &img_lv_100ask_icon_camera,          &img_lv_100ask_icon_paylater,\
                                          &img_lv_100ask_icon_scanning,        &img_lv_100ask_icon_position,\
                                          &img_lv_100ask_icon_data,            &img_lv_100ask_icon_set
                                            };

#endif


/* 回调函数指针数组，与桌标图标数组一一对应 */
static void (*lv_100ask_event_cb[])(lv_obj_t * obj, lv_event_t e) =\
                                          {icon_100ask_modify_picture_event_cb, icon_100ask_file_browser_event_cb,\
                                           icon_100ask_create_qrcode_event_cb,  icon_100ask_gallery_event_cb,\
                                           icon_100ask_music_player_event_cb,   icon_100ask_game_2048_event_cb,\
                                           icon_100ask_demo_widgets_event_cb,   icon_100ask_print_open_event_cb,\
                                           icon_100ask_card_test_event_cb,      icon_100ask_demo_benchmark_event_cb,\
                                           icon_100ask_file_browser_event_cb,   icon_100ask_game_2048_event_cb,\
                                           icon_100ask_music_player_event_cb,   icon_100ask_game_2048_event_cb,\
                                           icon_100ask_game_2048_event_cb,      icon_100ask_game_2048_event_cb,\
                                           icon_100ask_game_2048_event_cb,      icon_100ask_game_2048_event_cb
                                          };

static const char * app_name[] =\
                                       {"ps",       "file",\
                                        "QRcode",   "gallery",\
                                        "music",    "2048",\
                                        "widgets",  "printer",\
                                        "calc",     "100ASK",\
                                        "video",    "reader",\
                                        "camera",   "Payment",\
                                        "QR scan",  "GPS",\
                                        "time",     "settings"
                                        };

#if 0
/* 桌标图标png文件名，与回调函数指针数组一一对应 */
static const char * icon_file[] = { "100ask_demo_png_icon/100ask_icon_modify_picture.png",  "100ask_demo_png_icon/100ask_icon_2048.png",\
                                      "100ask_demo_png_icon/100ask_icon_camera.png",          "100ask_demo_png_icon/100ask_icon_set.png",\
                                      "100ask_demo_png_icon/100ask_icon_music.png",           "100ask_demo_png_icon/100ask_icon_txt.png",\
                                      "100ask_demo_png_icon/100ask_icon_paylater.png",        "100ask_demo_png_icon/100ask_icon_video_play.png",\
                                      "100ask_demo_png_icon/100ask_icon_scanning.png",        "100ask_demo_png_icon/100ask_icon_QRcode.png",\
                                      "100ask_demo_png_icon/100ask_icon_floder.png",          "100ask_demo_png_icon/100ask_icon_gallery.png",\
                                      "100ask_demo_png_icon/100ask_icon_about_us.png"
                            };
#endif



void ask100_update_time_task(lv_task_t * task)
{
    if(in_desktop_page)
    {
        char buffer_time[10];
        char buffer_date[12];
        time_t rawtime;
        struct tm * timeinfo;

        /* Set timer. */
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        //strftime (buffer,sizeof(buffer),"%Y/%m/%d %H:%M:%S",timeinfo);
        strftime (buffer_time,sizeof(buffer_time),"%H:%M",timeinfo);
        //strftime (buffer_date,sizeof(buffer_date),"%Y年%m月%d日",timeinfo);
        strftime (buffer_date,sizeof(buffer_date),"%Y/%m/%d",timeinfo);

        lv_label_set_text(systime_task_label, buffer_time);
        lv_label_set_text(sysdate_task_label, buffer_date);

    }

}



void ask100_show_pic_stack(lv_task_t * task)
{
	if(in_desktop_page)
	{
	    static uint8_t file_count = 0;
        lv_img_set_src(ask100_show_pic, img_rotate_pictures[file_count++]);
        lv_obj_align(ask100_show_pic, NULL, LV_ALIGN_CENTER, 0, 0);
        
		/* 填充内容（仅装饰展示） */
		if(canvas_letf_big_window != NULL)
			lv_100ask_demo_anim_out_all(canvas_letf_big_window, 1300);
        bar = lv_bar_create(canvas_letf_big_window, NULL);
        lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, lv_color_hex(0x01d3d4));
        lv_obj_set_size(bar, 25, 50);
        lv_obj_align_origo(bar, NULL, LV_ALIGN_IN_LEFT_MID, 1 * (box_w - 20) / 8 + 15, 0);
        lv_bar_set_value(bar,  (rand() % 120), LV_ANIM_ON);

        bar = lv_bar_create(canvas_letf_big_window, NULL);
        lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT,
                lv_color_hex(0xe600e6));
        lv_obj_set_size(bar, 25, 50);
        lv_obj_align_origo(bar, NULL, LV_ALIGN_IN_LEFT_MID, 3 * (box_w - 20) / 8 + 15, 0);
        lv_bar_set_value(bar, (rand() % 120), LV_ANIM_ON);

        bar = lv_bar_create(canvas_letf_big_window, NULL);
        lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT,
                lv_color_hex(0xefef01));
        lv_obj_set_size(bar, 25, 50);
        lv_obj_align_origo(bar, NULL, LV_ALIGN_IN_LEFT_MID, 5 * (box_w - 20) / 8 + 15, 0);
        lv_bar_set_value(bar, (rand() % 120), LV_ANIM_ON);

        bar = lv_bar_create(canvas_letf_big_window, NULL);
        lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT,
                lv_color_hex(0x1d1d25));
        lv_obj_set_size(bar, 25, 50);
        lv_obj_align_origo(bar, NULL, LV_ALIGN_IN_LEFT_MID, 7 * (box_w - 20) / 8 + 15, 0);
        lv_bar_set_value(bar, (rand() % 120), LV_ANIM_ON);

        bar = lv_bar_create(canvas_letf_big_window, NULL);
        lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT,
                lv_color_hex(0x5aa909));
        lv_obj_set_size(bar, 25, 50);
        lv_obj_align_origo(bar, NULL, LV_ALIGN_IN_LEFT_MID, 9 * (box_w - 20) / 8 + 15, 0);
        lv_bar_set_value(bar, (rand() % 120), LV_ANIM_ON);
		


        if (file_count >= 13) file_count = 0;
		
#if 0
        /* 读取png图片展示 */
		struct dirent **namelist;
		static uint8_t file_count = 1;
		uint16_t file_n;

		file_n = scandir("100ask_demo_png_icon/images", &namelist, 0, alphasort);
	 	if (file_n < 0)
			printf("Not found!\n");

		if ((strcmp("./", namelist[file_count++]->d_name) != 0) || (strcmp("../", namelist[file_count++]->d_name) != 0))
		{
			memset(show_pic_dir, 0, strlen(show_pic_dir));
			strcat(show_pic_dir, "100ask_demo_png_icon/images/");
			strcat(show_pic_dir, namelist[file_count]->d_name);
			//printf("file_name is: %s\n", show_pic_dir);

			/* 图标 */
			//lv_img_set_src(ask100_show_pic, show_pic_dir);
			//lv_obj_align(ask100_show_pic, NULL, LV_ALIGN_CENTER, 0, 0);

            /* 图标 */
            lv_obj_t * icon = lv_img_create(ask100_show_pic, NULL);
            //lv_obj_set_click(icon, 1);
            lv_img_set_src(icon, show_pic_dir);
            lv_obj_align(icon, ask100_show_pic, LV_ALIGN_CENTER, 0, 0);


		}
		//printf("file_count is:%d\n", file_count);
		//printf("file_n is:%d\n", file_n);
		if (file_count >= file_n-1) file_count = 1;
#endif // 0
	}
}

static void lv_100ask_start_animation(void)
{    
	/* Create an intro from a logo + label */
    LV_IMG_DECLARE(img_lv_100ask_demo_logo);
    lv_obj_t * logo = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(logo, &img_lv_100ask_demo_logo);
    lv_obj_align(logo, NULL, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_move_foreground(logo);

    lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(title, "100ASK Lvgl Demo\nwww.100ask.net");
    lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_font(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_30);
    lv_obj_set_style_local_text_line_space(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_align(title, logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_fade_out(title, 600, 0);
    lv_obj_fade_out(logo, 600, 0);
	
#if 0
	/*Animate in the content after the intro time*/
    lv_anim_t a;
    lv_anim_path_t path;
    lv_anim_path_init(&path);

    lv_anim_path_set_cb(&path, lv_anim_path_bounce);

    start_anim = true;

    lv_task_t * task =  lv_task_create(stop_start_anim, INTRO_TIME + 6000, LV_TASK_PRIO_MID, NULL);
    lv_task_once(task);

    lv_anim_init(&a);
    lv_anim_set_path(&a, &path);

	uint32_t i;
    lv_anim_set_exec_cb(&a, lv_obj_set_x);
    for(i = 0; i < 20; i++) {
        lv_anim_set_values(&a, LV_HOR_RES, 5);
        lv_anim_set_delay(&a, INTRO_TIME - 200 + rnd_array[i] % 200);
        lv_anim_set_time(&a, 2500 + rnd_array[i] % 500);
        lv_anim_set_var(&a, &start_anim_values[i]);
        lv_anim_start(&a);
    }
#endif // 0


}



/**********************************************************************
 * 函数名称： lv_100ask_printer
 * 功能描述：
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期              版本号      修改人	      修改内容
 * -----------------------------------------------
 * 2020/11/11	     V1.0	  YobeZhou    创建
 ***********************************************************************/
void lv_100ask_printer(lv_indev_t * indev)
{
	my_indev = indev;

    bg_color_prev = LV_100ASK_DEMO_BLUE;
    bg_color_act = LV_100ASK_DEMO_BLUE;


#if 1 //设置主题
	lv_theme_t * th = lv_100ask_demo_theme_init(LV_COLOR_BLACK, LV_COLOR_BLACK,
            0, &lv_font_montserrat_14, &lv_font_montserrat_22,
            &lv_font_montserrat_28, &lv_font_montserrat_32);
    lv_theme_set_act(th);
#endif

#if 1 
    lv_obj_t * scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);

    bg_top = lv_obj_create(lv_scr_act(), NULL);  //获取当前屏幕
    lv_obj_clean_style_list(bg_top, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_opa(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
            LV_OPA_COVER);
    lv_obj_set_style_local_bg_color(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
            lv_color_hex(0xFFFFFF));
    lv_obj_set_size(bg_top, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_y(bg_top, LV_100ASK_DEMO_BG_FULL);

#if 1
    /* 系统时间 */
	systime_task_label = lv_label_create(bg_top, NULL); /* 创建标签 */
	lv_theme_apply(systime_task_label, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
	//lv_label_set_text(systime_task_label, "Welcome to www.100ask.net!");
	lv_label_set_text(systime_task_label, "00:00:00");
	lv_obj_align(systime_task_label, bg_top, LV_ALIGN_IN_TOP_RIGHT, 0, 0);   /* 设置相对位置 */

#endif // 0
#endif // 0

    lv_100ask_start_animation(); /* 开机动画 */


	home_open(610);
	
	lv_task_create(ask100_update_time_task, 3000, LV_TASK_PRIO_MID, NULL);	
	lv_task_create(ask100_show_pic_stack, 1500, LV_TASK_PRIO_MID, NULL);

}


void home_open(uint32_t delay)
{
    /* menu item style */
    static lv_style_t page_round_out;
    lv_style_init(&page_round_out);
    lv_style_set_bg_opa(&page_round_out, LV_STATE_DEFAULT, LV_OPA_80); // 设置色彩透明度
    lv_style_set_radius(&page_round_out, LV_STATE_DEFAULT, 100); // 设置圆角
    lv_style_set_bg_color(&page_round_out, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    static lv_style_t page_round_in;
    lv_style_init(&page_round_in);
    lv_style_set_bg_opa(&page_round_in, LV_STATE_DEFAULT, LV_OPA_30); // 设置色彩透明度
    lv_style_set_radius(&page_round_in, LV_STATE_DEFAULT, 100); // 设置圆角
    lv_style_set_bg_color(&page_round_in, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    /* label time style */
    static lv_style_t style_label_time;
    lv_style_init(&style_label_time);
    lv_style_set_text_font(&style_label_time, LV_STATE_DEFAULT, &lv_font_montserrat_48);
    lv_style_set_bg_opa(&style_label_time, LV_STATE_DEFAULT, LV_OPA_0); // 设置色彩透明度
    lv_style_set_text_color(&style_label_time, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    /* label date style */
    static lv_style_t style_label_time_date;
    lv_style_init(&style_label_time_date);
    lv_style_set_text_font(&style_label_time_date, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_bg_opa(&style_label_time_date, LV_STATE_DEFAULT, LV_OPA_0); // 设置色彩透明度
    lv_style_set_text_color(&style_label_time_date, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    /* label app name style */
    static lv_style_t style_label_app_name;
    lv_style_init(&style_label_app_name);
    lv_style_set_text_font(&style_label_app_name, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_bg_opa(&style_label_app_name, LV_STATE_DEFAULT, LV_OPA_0); // 设置色彩透明度
    lv_style_set_text_color(&style_label_app_name, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    /* bottom menu style */
    static lv_style_t style_bottom_menu;
    lv_style_init(&style_bottom_menu);
    lv_style_set_text_font(&style_label_app_name, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_bg_opa(&style_bottom_menu, LV_STATE_DEFAULT, LV_OPA_80); // 设置色彩透明度
    lv_style_set_radius(&style_bottom_menu, LV_STATE_DEFAULT, 10); // 设置圆角
    lv_style_set_bg_color(&style_bottom_menu, LV_STATE_DEFAULT, lv_color_hex(0xC9C9CE)); //设置颜色

    /* bottom menu item style */
    static lv_style_t style_bottom_menu_item;
    lv_style_init(&style_bottom_menu_item);
    lv_style_set_bg_opa(&style_bottom_menu_item, LV_STATE_DEFAULT, LV_OPA_80); // 设置色彩透明度
    lv_style_set_radius(&style_bottom_menu_item, LV_STATE_DEFAULT, 8); // 设置圆角
    lv_style_set_bg_color(&style_bottom_menu_item, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    /* menu item style */
    static lv_style_t style_menu_item;
    lv_style_init(&style_menu_item);
    lv_style_set_bg_opa(&style_menu_item, LV_STATE_DEFAULT, LV_OPA_80); // 设置色彩透明度
    lv_style_set_radius(&style_menu_item, LV_STATE_DEFAULT, 12); // 设置圆角
    lv_style_set_bg_color(&style_menu_item, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); //设置颜色

    /* desktop style */
    static lv_style_t style_tabview_desktop;
    lv_style_init(&style_tabview_desktop);
    lv_style_set_bg_opa(&style_tabview_desktop, LV_STATE_DEFAULT, LV_OPA_0); // 设置色彩透明度

#if 0
    /* 使用png作为背景 */
	lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
	//lv_img_set_src(img, "100ask_demo_png_icon/100ask_background.png");
	lv_img_set_src(img, "100ask_demo_png_icon/1024x600_bg.png");
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_fade_in(img,0, delay);
#endif

#if 1
    /* 使用数组 */
    LV_IMG_DECLARE(img_lv_100ask_bg);
	lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &img_lv_100ask_bg);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_fade_in(img,0, delay);
#endif

    LV_IMG_DECLARE(lv_100ask_demo_icon_battery);
    LV_IMG_DECLARE(lv_100ask_demo_icon_wifi);
    LV_IMG_DECLARE(lv_100ask_demo_icon_tel);
    LV_IMG_DECLARE(lv_100ask_demo_icon_eco);
    LV_IMG_DECLARE(lv_100ask_demo_icon_pc);

    lv_obj_t *tabview_desktop = lv_tabview_create(img, NULL);
    lv_obj_add_style(tabview_desktop, LV_BTN_PART_MAIN, &style_tabview_desktop);
    lv_tabview_set_btns_pos(tabview_desktop, LV_TABVIEW_TAB_POS_NONE);
    lv_obj_t *tab_left  = lv_tabview_add_tab(tabview_desktop, "left_desktop");
    lv_obj_t *tab_main  = lv_tabview_add_tab(tabview_desktop, "main_desktop");
    lv_obj_t *tab_right = lv_tabview_add_tab(tabview_desktop, "right_desktop");
    lv_tabview_set_tab_act(tabview_desktop, 1, LV_ANIM_ON);

    lv_obj_t * icon;
    lv_obj_t * cont = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(cont, 180, 40);
    lv_obj_clean_style_list(cont, LV_CONT_PART_MAIN);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_RIGHT, 62,-12);

    icon = lv_img_create(cont, NULL);
    lv_img_set_src(icon, &lv_100ask_demo_icon_wifi);
	lv_img_set_zoom(icon, 128);
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 25);
    lv_100ask_demo_anim_top_to_bottom(icon, delay);

    icon = lv_img_create(cont, NULL);
    lv_img_set_src(icon, &lv_100ask_demo_icon_pc);
	lv_img_set_zoom(icon, 128);
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 40, 25);
    lv_100ask_demo_anim_top_to_bottom(icon, delay);

    icon = lv_img_create(cont, NULL);
    lv_img_set_src(icon, &lv_100ask_demo_icon_eco);
	lv_img_set_zoom(icon, 128);
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 25);
    lv_100ask_demo_anim_top_to_bottom(icon, delay);

    icon = lv_img_create(cont, NULL);
    lv_img_set_src(icon, &lv_100ask_demo_icon_battery);
	lv_img_set_zoom(icon, 128);
    lv_obj_align_origo(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 25);
    lv_100ask_demo_anim_top_to_bottom(icon, delay);

	/* 系统时间标签 */
    systime_task_label = lv_label_create(tab_main, NULL); /* 创建标签 */
    lv_obj_add_style(systime_task_label, LV_LABEL_PART_MAIN, &style_label_time);
    lv_label_set_text(systime_task_label, "00:00");
    lv_obj_align(systime_task_label, NULL, LV_ALIGN_IN_TOP_LEFT, 55, 40);
    lv_obj_fade_in(systime_task_label, 0 , delay);

    /* 日期标签 */
    sysdate_task_label = lv_label_create(tab_main, NULL); /* 创建标签 */
    lv_obj_add_style(sysdate_task_label, LV_LABEL_PART_MAIN, &style_label_time_date);
    //lv_label_set_text(sysdate_task_label, "Tuesday,December 22");
    lv_label_set_text(sysdate_task_label, "2020/12/23");
    lv_obj_align(sysdate_task_label, systime_task_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_fade_in(sysdate_task_label, 0 , delay);


    /* 创建底部导航栏 */
    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(CANVAS_BOTTOM_MENU_WIDTH, CANVAS_BOTTOM_MENU_HEIGHT)];
    lv_obj_t * canvas_bottom_menu = lv_canvas_create(lv_scr_act(), NULL);
    lv_canvas_set_buffer(canvas_bottom_menu, cbuf, CANVAS_BOTTOM_MENU_WIDTH, CANVAS_BOTTOM_MENU_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    //lv_obj_align(canvas_bottom_menu, NULL, LV_ALIGN_IN_TOP_MID, 0, 505);   //对象与父对象的对齐样式 
    lv_obj_align(canvas_bottom_menu, NULL, LV_ALIGN_IN_TOP_MID, 0, 605);   //对齐后的坐标偏移
    //LV_ALIGN_IN_BOTTOM_MID
    lv_obj_add_style(canvas_bottom_menu, LV_BTN_PART_MAIN, &style_bottom_menu);
    lv_obj_fade_in(canvas_bottom_menu, 0 , delay);

      /* 左菜单指示 */
    lv_obj_t * left_round1 = lv_cont_create(tab_left, NULL);         // 中间
    lv_obj_add_style(left_round1, LV_BTN_PART_MAIN, &page_round_out);
    lv_obj_set_size(left_round1, 8, 8);
    lv_obj_align(left_round1, NULL, LV_ALIGN_CENTER, 0, 185);

    lv_obj_t * left_round2 = lv_cont_create(tab_left, left_round1);  // 右边
    lv_obj_add_style(left_round2, LV_BTN_PART_MAIN, &page_round_out);
    lv_obj_align(left_round2, NULL, LV_ALIGN_CENTER, 20, 185);

    lv_obj_t * left_round3 = lv_cont_create(tab_left, left_round1);  // 左边
    lv_obj_add_style(left_round3, LV_BTN_PART_MAIN, &page_round_in);
    lv_obj_align(left_round3, NULL, LV_ALIGN_CENTER, -20, 185);

    /* 主菜单指示 */
    lv_obj_t * main_round1 = lv_cont_create(tab_main, NULL);         // 中间
    lv_obj_add_style(main_round1, LV_BTN_PART_MAIN, &page_round_in);
    lv_obj_set_size(main_round1, 8, 8);
    lv_obj_align(main_round1, NULL, LV_ALIGN_CENTER, 0, 185);

    lv_obj_t * main_round2 = lv_cont_create(tab_main, main_round1);  // 右边
    lv_obj_add_style(main_round2, LV_BTN_PART_MAIN, &page_round_out);
    lv_obj_align(main_round2, NULL, LV_ALIGN_CENTER, 20, 185);

    lv_obj_t * main_round3 = lv_cont_create(tab_main, main_round1);  // 左边
    lv_obj_add_style(main_round3, LV_BTN_PART_MAIN, &page_round_out);
    lv_obj_align(main_round3, NULL, LV_ALIGN_CENTER, -20, 185);


    /* 右菜单指示 */
    lv_obj_t * right_round1 = lv_cont_create(tab_right, NULL);         // 中间
    lv_obj_add_style(right_round1, LV_BTN_PART_MAIN, &page_round_out);
    lv_obj_set_size(right_round1, 8, 8);
    lv_obj_align(right_round1, NULL, LV_ALIGN_CENTER, 0, 185);

    lv_obj_t * right_round2 = lv_cont_create(tab_right, right_round1);  // 右边
    lv_obj_add_style(right_round2, LV_BTN_PART_MAIN, &page_round_in);
    lv_obj_align(right_round2, NULL, LV_ALIGN_CENTER, 20, 185);

    lv_obj_t * right_round3 = lv_cont_create(tab_right, right_round1);  // 左边
    lv_obj_add_style(right_round3, LV_BTN_PART_MAIN, &page_round_out);
    lv_obj_align(right_round3, NULL, LV_ALIGN_CENTER, -20, 185);

#if 1
    /* 创建左侧的两个小窗口 */
    static lv_color_t cbuf_letf_small_window1[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(CANVAS_LEFT_SMALL_WINDOW, CANVAS_LEFT_SMALL_WINDOW)];
    lv_obj_t * canvas_letf_small_window1  = lv_canvas_create(tab_main, NULL);
    lv_canvas_set_buffer(canvas_letf_small_window1, cbuf_letf_small_window1, CANVAS_LEFT_SMALL_WINDOW, CANVAS_LEFT_SMALL_WINDOW, LV_IMG_CF_INDEXED_1BIT);
    lv_obj_align(canvas_letf_small_window1, NULL, LV_ALIGN_IN_TOP_LEFT, 48, 140);
    lv_obj_add_style(canvas_letf_small_window1, LV_BTN_PART_MAIN, &style_bottom_menu);
    lv_obj_fade_in(canvas_letf_small_window1, 0 , delay);

    /* 设置轮播图片  13 张图片 */
    ask100_show_pic = lv_img_create(canvas_letf_small_window1, NULL);
    lv_obj_set_event_cb(ask100_show_pic, icon_100ask_gallery_event_cb);
    lv_img_set_src(ask100_show_pic, img_rotate_pictures[3]);
    lv_obj_set_click(ask100_show_pic, 1);
    lv_obj_align(ask100_show_pic, canvas_letf_small_window1, 0, 0, 0);

    // 显示了字符串
    static lv_color_t cbuf_letf_small_window2[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(CANVAS_LEFT_SMALL_WINDOW, CANVAS_LEFT_SMALL_WINDOW)];
    lv_obj_t * canvas_letf_small_window2 = lv_canvas_create(tab_main, NULL);
    lv_canvas_set_buffer(canvas_letf_small_window2, cbuf_letf_small_window2, CANVAS_LEFT_SMALL_WINDOW, CANVAS_LEFT_SMALL_WINDOW, LV_IMG_CF_INDEXED_1BIT);
    lv_obj_align(canvas_letf_small_window2, NULL, LV_ALIGN_IN_TOP_LEFT, 186, 140);
    lv_obj_add_style(canvas_letf_small_window2, LV_BTN_PART_MAIN, &style_bottom_menu);
    lv_obj_fade_in(canvas_letf_small_window2, 0 , delay);

    lv_obj_t *  label2 = lv_label_create(canvas_letf_small_window2, NULL);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC);     /*Circular scroll*/
    lv_obj_set_width(label2, 120);
    lv_obj_add_style(label2, LV_LABEL_PART_MAIN, &style_label_app_name);
    lv_label_set_text(label2, "For more information, please visit:");
    lv_obj_align(label2, canvas_letf_small_window2, LV_ALIGN_IN_TOP_LEFT, 0, 30);
    label2 = lv_label_create(canvas_letf_small_window2, NULL);
    lv_obj_add_style(label2, LV_LABEL_PART_MAIN, &style_label_app_name);
    lv_label_set_text(label2, "www.100ask.net");
    lv_obj_align(label2, canvas_letf_small_window2, LV_ALIGN_IN_TOP_LEFT, 0, 60);


    // 创建的动态矩形
    static lv_color_t cbuf_letf_big_window[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(CANVAS_LEFT_BIG_WINDOW_WIDTH, CANVAS_LEFT_BIG_WINDOW_HEIGHT)];
    //lv_obj_t * canvas_letf_big_window = lv_canvas_create(tab_main, NULL);
    canvas_letf_big_window = lv_canvas_create(tab_main, NULL);
    lv_canvas_set_buffer(canvas_letf_big_window, cbuf_letf_big_window, CANVAS_LEFT_BIG_WINDOW_WIDTH, CANVAS_LEFT_BIG_WINDOW_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_obj_align(canvas_letf_big_window, NULL, LV_ALIGN_IN_TOP_LEFT, 46, 275);
    lv_obj_add_style(canvas_letf_big_window, LV_BTN_PART_MAIN, &style_bottom_menu);
    lv_obj_fade_in(canvas_letf_big_window, 0 , delay);


    //文本框加文本
    static lv_color_t cbuf_letf_line[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(CANVAS_LEFT_LINE_WIDTH, CANVAS_LEFT_LINE_HEIGHT)];
    lv_obj_t * canvas_letf_tip_window = lv_canvas_create(tab_main, NULL);
    lv_canvas_set_buffer(canvas_letf_tip_window, cbuf_letf_line, CANVAS_LEFT_LINE_WIDTH, CANVAS_LEFT_LINE_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_obj_align(canvas_letf_tip_window, NULL, LV_ALIGN_IN_TOP_LEFT, 46, 410);
    lv_obj_add_style(canvas_letf_tip_window, LV_OBJ_PART_MAIN, &style_bottom_menu);
    lv_obj_fade_in(canvas_letf_tip_window, 0 , delay);
    //lv_obj_set_style_local_value_str(canvas_letf_tip_window, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,\
            "What is your main focus for today?"); // What do you want to do today?
    
    lv_obj_t *  label_tip = lv_label_create(canvas_letf_tip_window, NULL);
    lv_obj_add_style(label_tip, LV_LABEL_PART_MAIN, &style_label_app_name);
    lv_label_set_text(label_tip, "What is your main focus for today?");
    lv_obj_align(label_tip, canvas_letf_tip_window, LV_ALIGN_CENTER, 0, 0);

#endif // 0

#if 1
  // 显示 APP  循环 数组 
    for (int i = 0; i < BOTTOM_MENU_ITEM_NUM; i++)
    {
        /* 图标 */
        lv_obj_t * icon_bottom_menu_item = lv_img_create(lv_scr_act(), NULL);

        if (i == (BOTTOM_MENU_ITEM_NUM-1))
        {
            //lv_img_set_src(icon_bottom_menu_item, icon_file[MENU_ITEM_NUM-1]); /* 使用png文件 */
            lv_img_set_src(icon_bottom_menu_item, app_icon[MENU_ITEM_NUM-1]);    /* 使用数组文件 */
            lv_obj_set_event_cb(icon_bottom_menu_item, lv_100ask_event_cb[MENU_ITEM_NUM-1]);    /* 分配回调处理函数 */
        }
        else
        {
            //lv_img_set_src(icon_bottom_menu_item, icon_file[i]);    /* 使用png文件 */
            lv_img_set_src(icon_bottom_menu_item, app_icon[i]);       /* 使用数组文件 */
            lv_obj_set_event_cb(icon_bottom_menu_item, lv_100ask_event_cb[i]);    /* 分配回调处理函数 */
        }

        lv_obj_align(icon_bottom_menu_item, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_click(icon_bottom_menu_item, true);                        // 启用点击
        lv_obj_set_drag(icon_bottom_menu_item, true);                         // 启用拖动
        lv_obj_fade_in(icon_bottom_menu_item,0, delay);                       // 延迟delayms后再展示（等待开机动画播放完毕）


        if(i == 0)
            lv_obj_align(icon_bottom_menu_item, canvas_bottom_menu, LV_ALIGN_IN_LEFT_MID, 14, 0);  // 设置摆放位置
        else
        {
            lv_obj_align(icon_bottom_menu_item, canvas_bottom_menu, LV_ALIGN_IN_LEFT_MID, 14+16*i+CANVAS_BOTTOM_MENU_ITEM*i, 0);  // 设置摆放位置

        }
    }


    int row_space = 66;
    int column_space = -135;
    int row = 0;
    int count = 1;
    for (int i = 0; i < MENU_ITEM_NUM; i++)
    {

        /* 图标 */
        lv_obj_t * icon;
        lv_obj_t * label_app_name;
        if((i >= 0) && (i <= 9))
        {
            icon = lv_img_create(tab_main, NULL);
            label_app_name = lv_label_create(tab_main, NULL);
        }
        else if((i >= 10) && (i <= 12))
        {
            icon = lv_img_create(tab_left, NULL);
            label_app_name = lv_label_create(tab_left, NULL);
            if (i == 8)
            {
                row_space = 66;
                column_space = -135;
                row = 0;
                count = 1;
            }
        }
        else if((i >= 13) && (i <= 17))
        {
            icon = lv_img_create(tab_right, NULL);
            label_app_name = lv_label_create(tab_right, NULL);
            if (i == 13)
            {
                row_space = 66;
                column_space = -135;
                row = 0;
                count = 1;
            }
        }

        lv_obj_set_click(icon, 1);
        //lv_img_set_src(icon, icon_file[i]);  /* 使用png文件 */
        lv_img_set_src(icon, app_icon[i]);    /* 使用数组文件 */
        lv_obj_align(icon, NULL, LV_ALIGN_CENTER, 0, 0);

        lv_obj_set_drag(icon, true); // 启用拖动
        lv_obj_fade_in(icon,0, delay);

        if(i == 0)
            lv_obj_align(icon, NULL, LV_ALIGN_OUT_RIGHT_TOP, -135, row_space);   // 设置摆放位置
        else
        {

            if ((i%5)==0)
            {
                row_space = 66+143*count;
                column_space = -135;
                row = 0;
                count++;
            }
            lv_obj_align(icon, NULL, LV_ALIGN_OUT_RIGHT_TOP,  -(135+55*row+CANVAS_MENU_ITEM*row), row_space);   // 设置摆放位置

        }
        lv_obj_set_event_cb(icon, lv_100ask_event_cb[i]);  /* 分配回调处理函数 */


        /* 创建 app name 标签 */
        lv_obj_add_style(label_app_name, LV_LABEL_PART_MAIN, &style_label_app_name);
        lv_label_set_text(label_app_name, app_name[i]);
        lv_obj_align(label_app_name, icon, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
        lv_obj_fade_in(label_app_name, 0 , delay);
        //lv_obj_set_parent(label_app_name, icon);

        row++;
    }

    in_desktop_page = true;
    //lv_obj_move_foreground(systime_task_label);
    //lv_obj_move_foreground(sysdate_task_label);

#endif // 0
}


// 图标的回调  打印demo
LV_EVENT_CB_DECLARE(icon_100ask_print_open_event_cb)  //printer
{
    if (e == LV_EVENT_CLICKED) {
        in_desktop_page = false;
        lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_printer_share(200);
    }
	//icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(icon_100ask_demo_widgets_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        in_desktop_page = false;
        lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_demo_widgets();
    }
	//icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(icon_100ask_demo_benchmark_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        in_desktop_page = false;
        lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_demo_benchmark();
    }
	//icon_generic_event_cb(obj, e);
}



LV_EVENT_CB_DECLARE(icon_100ask_modify_picture_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        in_desktop_page = false;
        lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_modify_picture();
    }
	//icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(icon_100ask_game_2048_event_cb)
{
	if (e == LV_EVENT_CLICKED)
	{
		in_desktop_page = false;
		lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_game_2048(my_indev);
	}
	//icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(icon_100ask_card_test_event_cb)
{
    if (e == LV_EVENT_CLICKED)
	{
		in_desktop_page = false;
        lv_demo_printer_anim_out_all(lv_scr_act(), 0);
        lv_demo_cardtest();
    }
}
LV_EVENT_CB_DECLARE(icon_100ask_create_qrcode_event_cb)
{
	if (e == LV_EVENT_CLICKED)
	{
		in_desktop_page = false;
		lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_create_qrcode();
	}
	//icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(icon_100ask_file_browser_event_cb)
{
	if (e == LV_EVENT_CLICKED)
	{
		in_desktop_page = false;
		lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_open_file_browser();
	}
	//icon_generic_event_cb(obj, e);
}


LV_EVENT_CB_DECLARE(icon_100ask_gallery_event_cb)
{
	if (e == LV_EVENT_CLICKED)
	{
		in_desktop_page = false;
		lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_open_gallery();
	}
	//icon_generic_event_cb(obj, e);

}

LV_EVENT_CB_DECLARE(icon_100ask_music_player_event_cb)
{
	if (e == LV_EVENT_CLICKED)
	{
		in_desktop_page = false;
		lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		lv_100ask_music_player();
	}
	//icon_generic_event_cb(obj, e);

}


static void lv_demo_printer_anim_out_all(lv_obj_t * obj, uint32_t delay) {
    lv_obj_t * child = lv_obj_get_child_back(obj, NULL);
    while(child) {
        if(child != bg_top && child != bg_bottom && child != lv_scr_act()) {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, child);
            lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME);
            lv_anim_set_delay(&a, delay);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
            if(lv_obj_get_y(child) < 80) {
                lv_anim_set_values(&a, lv_obj_get_y(child),
                        lv_obj_get_y(child) - LV_100ASK_DEMO_ANIM_Y);
            } else {
                lv_anim_set_values(&a, lv_obj_get_y(child),
                        lv_obj_get_y(child) + LV_100ASK_DEMO_ANIM_Y);

                delay += LV_100ASK_DEMO_ANIM_DELAY;
            }
            lv_anim_set_ready_cb(&a, lv_obj_del_anim_ready_cb);
            lv_anim_start(&a);
        }
        child = lv_obj_get_child_back(obj, child);
    }
}



void lv_100ask_demo_anim_top_to_bottom(lv_obj_t * obj, uint32_t delay)
{
    if (obj != bg_top && obj != lv_scr_act()) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_HOME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(obj) - LV_100ASK_DEMO_ANIM_Y,
               lv_obj_get_y(obj));
        lv_anim_start(&a);

        lv_obj_fade_in(obj, LV_100ASK_DEMO_ANIM_TIME - 50, delay);
    }
}



void lv_100ask_demo_anim_left_to_right(lv_obj_t * obj, uint32_t delay)
{
    if (obj != bg_top && obj != lv_scr_act()) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_HOME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_x);
        //lv_anim_set_values(&a, lv_obj_get_y(obj) - LV_100ASK_DEMO_ANIM_Y,
        //       lv_obj_get_y(obj));
        lv_anim_set_values(&a, lv_obj_get_x(obj) - LV_100ASK_DEMO_ANIM_X,
               lv_obj_get_x(obj));
        lv_anim_start(&a);

        lv_obj_fade_in(obj, LV_100ASK_DEMO_ANIM_TIME_HOME - 50, delay);
    }
}


void lv_100ask_demo_anim_right_to_left(lv_obj_t * obj, uint32_t delay)
{
    if (obj != bg_top && obj != lv_scr_act()) {
         lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_HOME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_x);
        //lv_anim_set_values(&a, lv_obj_get_y(obj) - LV_100ASK_DEMO_ANIM_Y,
        //       lv_obj_get_y(obj));
        lv_anim_set_values(&a, lv_obj_get_x(obj) + LV_100ASK_DEMO_ANIM_X,
               lv_obj_get_x(obj));
        lv_anim_start(&a);

        lv_obj_fade_in(obj, LV_100ASK_DEMO_ANIM_TIME_HOME - 50, delay);
    }
}


// TODO
void lv_100ask_demo_anim_path(lv_obj_t * obj, uint32_t delay)
{

}


void lv_100ask_demo_anim_bg(uint32_t delay, lv_color_t color, int32_t y_new)
{
    lv_coord_t y_act = lv_obj_get_y(bg_top);
    lv_color_t act_color = lv_obj_get_style_bg_color(bg_top, LV_OBJ_PART_MAIN);
    if(y_new != LV_100ASK_DEMO_BG_NORMAL && y_new == y_act && act_color.full == color.full) return;


    lv_anim_t a;
    if((y_new == LV_100ASK_DEMO_BG_NORMAL && y_new == y_act) ||
            (y_new == LV_100ASK_DEMO_BG_NORMAL && y_act == LV_100ASK_DEMO_BG_FULL)) {
        lv_anim_path_t path;
        lv_anim_path_init(&path);
        lv_anim_path_set_cb(&path, anim_path_triangle);

        lv_anim_init(&a);
        lv_anim_set_var(&a, bg_top);
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_BG + 200);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, y_act, y_new);
        lv_anim_set_path(&a, &path);
        lv_anim_start(&a);
    } else {
        lv_anim_init(&a);
        lv_anim_set_var(&a, bg_top);
        lv_anim_set_time(&a, LV_100ASK_DEMO_ANIM_TIME_BG);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(bg_top), y_new);
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


lv_obj_t * add_back(lv_event_cb_t event_cb)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    lv_theme_apply(btn, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_BACK);

    lv_obj_set_size(btn, 80, 80);
    lv_obj_set_pos(btn, 30, 10);
    lv_obj_set_event_cb(btn, event_cb);

    return btn;
}



void info_bottom_create(const char * dsc, const char * btn_txt, lv_event_cb_t btn_event_cb, uint32_t delay)
{

    lv_obj_t * txt = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(txt, dsc);
    lv_theme_apply(txt, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
    lv_obj_align(txt, NULL, LV_ALIGN_CENTER, 0, 100);

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    lv_theme_apply(btn, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_BORDER);
    lv_obj_set_size(btn, LV_100ASK_DEMO_BTN_W, LV_100ASK_DEMO_BTN_H);
    lv_obj_align(btn, txt, LV_ALIGN_OUT_BOTTOM_MID, 0, 60);
    lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, btn_txt);
    lv_obj_set_event_cb(btn, btn_event_cb);

    lv_100ask_demo_anim_top_to_bottom(txt, delay);
    delay += LV_100ASK_DEMO_ANIM_DELAY;

    lv_100ask_demo_anim_top_to_bottom(btn, delay);
    delay += LV_100ASK_DEMO_ANIM_DELAY;

    lv_100ask_demo_anim_top_to_bottom(btn, delay);
}

#if 0

static void print_open(uint32_t delay)
{
    lv_obj_t * back = add_back(back_to_home_event_cb);
    lv_100ask_demo_anim_top_to_bottom(back, delay);

    lv_obj_t * title = add_title("PRINT MENU");
    lv_100ask_demo_anim_top_to_bottom(title, delay);

    lv_coord_t box_w = 720;
    lv_obj_t * box = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(box, box_w, 260);
    lv_obj_align(box, NULL, LV_ALIGN_IN_TOP_MID, 0, 100);

    delay += LV_100ASK_DEMO_ANIM_DELAY;
    lv_100ask_demo_anim_top_to_bottom(box, delay);

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
    lv_obj_align(box, NULL, LV_ALIGN_IN_BOTTOM_LEFT, LV_HOR_RES / 20,
            - LV_HOR_RES / 40);
    lv_obj_set_style_local_value_str(box, LV_CONT_PART_MAIN, LV_STATE_DEFAULT,
            "From where do you want to print?");

    delay += LV_100ASK_DEMO_ANIM_DELAY;
    lv_100ask_demo_anim_top_to_bottom(box, delay);

    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_NORMAL);
}

LV_EVENT_CB_DECLARE(usb_icon_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

        uint32_t delay = 200;

        lv_obj_t * back = add_back(back_to_print_event_cb);
        lv_100ask_demo_anim_top_to_bottom(back, delay);

         lv_obj_t * title = add_title("PRINTING FROM USB DRIVE");
         lv_100ask_demo_anim_top_to_bottom(title, delay);

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
         lv_100ask_demo_anim_top_to_bottom(list, delay);

         delay += LV_100ASK_DEMO_ANIM_DELAY;
         lv_100ask_demo_anim_top_to_bottom(settings_box, delay);

         delay += LV_100ASK_DEMO_ANIM_DELAY;
         lv_100ask_demo_anim_top_to_bottom(dropdown_box, delay);

         delay += LV_100ASK_DEMO_ANIM_DELAY;
         lv_100ask_demo_anim_top_to_bottom(print_btn, delay);

         lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_BLUE, LV_100ASK_DEMO_BG_NORMAL);

    }
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

        lv_100ask_demo_anim_top_to_bottom(arc, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;
        lv_100ask_demo_anim_top_to_bottom(txt, delay);
    }
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
    lv_100ask_demo_anim_top_to_bottom(img, delay);
    delay += LV_100ASK_DEMO_ANIM_DELAY;

    info_bottom_create("Printing finished", "CONTINUE", back_to_home_event_cb, delay);
}


LV_EVENT_CB_DECLARE(back_to_print_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        print_open(150);
    }
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

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_wave);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_wave);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_phone);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_phone);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 80, 0);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        info_bottom_create("Put you phone near to the printer", "BACK", back_to_print_event_cb, delay);
    }

    icon_generic_event_cb(obj, e);
}

#endif // 0
#if 0
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

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_no_internet);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_no_internet);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -40);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_cloud);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_cloud);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 80, -80);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        info_bottom_create("No internet connection", "BACK", back_to_print_event_cb, delay);
    }

    icon_generic_event_cb(obj, e);
}
#endif // 0

#if 0
LV_EVENT_CB_DECLARE(setup_icon_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_RED, LV_100ASK_DEMO_BG_FULL);

        uint32_t delay = 200;

        LV_IMG_DECLARE(lv_100ask_demo_img_printer2);

        lv_obj_t * img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_printer2);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, -90, 0);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_no_internet);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_no_internet);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -40);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        LV_IMG_DECLARE(lv_100ask_demo_img_cloud);
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, &lv_100ask_demo_img_cloud);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 80, -80);

        lv_100ask_demo_anim_top_to_bottom(img, delay);
        delay += LV_100ASK_DEMO_ANIM_DELAY;

        info_bottom_create("You have no permission to change the settings.", "BACK", back_to_home_event_cb, delay);
    }

    icon_generic_event_cb(obj, e);
}



LV_EVENT_CB_DECLARE(print_open_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        print_open(200);
    }
    icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        //scan_img = NULL;
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
		//in_desktop_page = true;
        home_open(200);
    }
}
#endif // 0

#if 0
static lv_obj_t * add_canvas(lv_obj_t * parent, lv_color_t * cbuf, char * icon_file, \
							 uint16_t canvas_w, uint16_t canvas_h, \
							 uint16_t rect_dsc_x, uint16_t rect_dsc_y, \
							 lv_color_t rgb32)
{
	/* 创建画布 */
	lv_obj_t * canvas = lv_canvas_create(lv_scr_act(), NULL);
	lv_obj_set_click(canvas, true);
	lv_img_set_antialias(canvas, false); // 禁用抗锯齿
	lv_canvas_set_buffer(canvas, cbuf, canvas_w, canvas_h, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(canvas, NULL, LV_ALIGN_IN_TOP_LEFT, rect_dsc_x, rect_dsc_y);
	lv_canvas_fill_bg(canvas, LV_COLOR_SILVER, LV_OPA_COVER);

	/* 画矩形 */
	lv_draw_rect_dsc_t rect_dsc;
	lv_draw_rect_dsc_init(&rect_dsc);
	rect_dsc.bg_color = rgb32;
	lv_canvas_draw_rect(canvas, 0, 0, canvas_w, canvas_h, &rect_dsc);

	/* 图标 */
	lv_obj_t * icon = lv_img_create(canvas, NULL);
	lv_img_set_src(icon, icon_file);
	//lv_img_set_zoom(icon, 512);
	lv_obj_align(icon, NULL, LV_ALIGN_CENTER, 0, 0);
	//lv_canvas_draw_img(icon, 0, 0, LV_SYMBOL_VIDEO, &img_draw_dsc);
	lv_obj_set_parent(canvas, parent);

	//lv_obj_set_click(canvas, 1);
	//lv_obj_set_click(icon, 1);
#if 1
	if(strcmp(icon_file, "100ask_demo_png_icon/100ask_icon_gallery.png") == 0)
	{
		lv_obj_set_click(icon, 1);
		lv_100ask_demo_anim_right_to_left(icon, 200);
		return icon;
	}
	else return canvas;
#endif


}
#endif // 0


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


lv_obj_t * add_title(const char * txt)
{
    lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
    lv_theme_apply(title, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
    lv_label_set_text(title, txt);
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0,
            LV_100ASK_DEMO_TITLE_PAD);
    return title;
}

lv_obj_t * add_loader(void (*end_cb)(lv_anim_t *))
{
    lv_obj_t * arc = lv_arc_create(lv_scr_act(), NULL);
    lv_arc_set_bg_angles(arc, 0, 0);
    lv_arc_set_start_angle(arc, 270);
    lv_obj_set_size(arc, 180, 180);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, loader_anim_cb);
    lv_anim_set_ready_cb(&a, end_cb);
    lv_anim_set_values(&a, 0, 110);
    lv_anim_set_time(&a, 500);
    lv_anim_set_var(&a, arc);
    lv_anim_start(&a);

    return arc;
}

static void loader_anim_cb(void * arc, lv_anim_value_t v)
{
    if(v > 100) v = 100;
    lv_arc_set_end_angle(arc, v * 360 / 100 + 270);

    static char buf[32];
    lv_snprintf(buf, sizeof(buf), "%d %%", v);
    lv_obj_set_style_local_value_str(arc, LV_ARC_PART_BG, LV_STATE_DEFAULT, buf);
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

#if 0
LV_EVENT_CB_DECLARE(icon_generic_event_cb)
{
    if (e == LV_EVENT_PRESSED) {
        lv_obj_t * img = lv_obj_get_child_back(obj, NULL);
        lv_obj_t * txt = lv_obj_get_child(obj, NULL);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_time(&a, 100);


        lv_anim_set_var(&a, img);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, lv_obj_get_x(img), lv_obj_get_width(obj) - lv_obj_get_width(img));
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(img), 0);
        lv_anim_start(&a);

        lv_anim_set_var(&a, txt);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, lv_obj_get_x(txt), 0);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(txt), lv_obj_get_height(obj) - lv_obj_get_height(txt));
        lv_anim_start(&a);



    }
    else if(e == LV_EVENT_PRESS_LOST || e == LV_EVENT_RELEASED) {
        lv_obj_t * img = lv_obj_get_child_back(obj, NULL);
        lv_obj_t * txt = lv_obj_get_child(obj, NULL);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_time(&a, 100);
        lv_anim_set_var(&a, img);

        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, lv_obj_get_x(img), lv_obj_get_width(obj) - lv_obj_get_width(img));
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(img), 0);
        lv_anim_start(&a);

        lv_anim_set_var(&a, txt);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, lv_obj_get_x(txt), 0);
        lv_anim_start(&a);

        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(txt), lv_obj_get_height(obj) - lv_obj_get_height(txt));
        lv_anim_start(&a);
    }
}
#endif // 0


void lv_100ask_demo_anim_out_all(lv_obj_t * obj, uint32_t delay) {

    lv_obj_t * child = lv_obj_get_child_back(obj, NULL);
    while(child) {
        if(child != bg_top && child != lv_scr_act()) {
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, child);
            lv_anim_set_time(&a, 10);
            lv_anim_set_delay(&a, 100);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_y);
            if(lv_obj_get_y(child) < 80) {
                lv_anim_set_values(&a, lv_obj_get_y(child),
                        lv_obj_get_y(child) - LV_100ASK_DEMO_ANIM_Y);
            } else {
                lv_anim_set_values(&a, lv_obj_get_y(child),
                        lv_obj_get_y(child) + LV_100ASK_DEMO_ANIM_Y);

                delay += LV_100ASK_DEMO_ANIM_DELAY;
            }
            lv_anim_set_ready_cb(&a, lv_obj_del_anim_ready_cb);
            lv_anim_start(&a);
        }
        child = lv_obj_get_child_back(obj, child);
    }
}



static void anim_bg_color_cb(lv_anim_t * a, lv_anim_value_t v)
{
    lv_color_t c = lv_color_mix(bg_color_act, bg_color_prev, v);
    lv_obj_set_style_local_bg_color(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, c);
}




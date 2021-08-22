/**
 * @file 100ask_file_browser.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <dirent.h>
#include <sys/stat.h>

#include "../../lv_100ask_demo_theme/lv_100ask_demo_theme.h"

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * file_browser_list;
static char * file_browser_dir[256] = {"./"};
static char * file_name[128];

/**********************
 *  STATIC PROTOTYPES
 **********************/
LV_EVENT_CB_DECLARE(file_browser_open_file_event_cb);
LV_EVENT_CB_DECLARE(back_to_file_browser_event_cb);
LV_EVENT_CB_DECLARE(open_gallery_icon_event_cb);
LV_EVENT_CB_DECLARE(back_to_home_event_cb);


static void file_browser_open(const char * btn_txt);
static void file_browser_anim_ready(lv_anim_t * a);
static void open_file(const char * name);
static void open_file_anim_ready(lv_anim_t * a);



void lv_100ask_open_file_browser(void)
{
		strcpy(file_browser_dir, "./");

		lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
		uint32_t delay = 200;
		lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GREEN, LV_100ASK_DEMO_BG_FULL);

		lv_obj_t * arc = add_loader(file_browser_anim_ready);
		lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, -40);

		lv_obj_t * txt = lv_label_create(lv_scr_act(), NULL);
		lv_label_set_text(txt, "Opening, please wait...");
		lv_theme_apply(txt, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
		lv_obj_align(txt, arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 60);

		lv_100ask_demo_anim_top_to_bottom(arc, delay);
		delay += LV_100ASK_DEMO_ANIM_DELAY;
		lv_100ask_demo_anim_top_to_bottom(txt, delay);

}

void lv_100ask_open_gallery(void)
{
	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    //file_browser_open("100ask_demo_png_icon/images/");  // 页面处理
    file_browser_open("./");  // 页面处理
}



static void file_browser_anim_ready(lv_anim_t * a)
{

    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    file_browser_open(file_browser_dir);  // 页面处理
}

static void file_browser_open(const char * dir)
{
	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GRAY, LV_100ASK_DEMO_BG_SMALL);  // 调整页面背景

    uint32_t delay = 200;

    lv_obj_t * back = add_back(back_to_home_event_cb);
    lv_100ask_demo_anim_top_to_bottom(back, delay);

    lv_obj_t * title = add_title("FILE BROWSER");
    lv_100ask_demo_anim_top_to_bottom(title, delay);

#if 1
	/* 记录路径 */
	//printf("chasr_is: %s\n", dir);
	if (file_browser_dir != NULL)
	{

	    if (strlen(file_browser_dir) == 0){
			printf("file_browser_dir strlen is 0: %s\n", file_browser_dir);
			strcpy(file_browser_dir, "./");
	    	}
		else strcpy(file_browser_dir, dir);
	}
	printf("dir: %s\n", file_browser_dir);

	struct dirent **namelist;
	uint16_t file_n;
	file_n = scandir(file_browser_dir, &namelist, 0, alphasort);
 	if (file_n < 0) printf("Not found!\n");
	//printf("file_n is:%d\n", file_n);

	lv_coord_t box_w = LV_HOR_RES;
	lv_obj_t * file_browser_list = lv_list_create(lv_scr_act(), NULL);
	lv_obj_set_size(file_browser_list, box_w, LV_VER_RES-(LV_VER_RES/9));
	lv_obj_align(file_browser_list, NULL, LV_ALIGN_CENTER, 0, LV_VER_RES/9);
	uint32_t i;

	for(uint16_t i = 0; i < file_n; i++) {
		//printf("i:%d file_n:%d\n", i, file_n);
		lv_obj_t * btn = lv_list_add_btn(file_browser_list, LV_SYMBOL_FILE, namelist[i]->d_name);
		//printf("file_name:%s\n",namelist[i]->d_name);
		//printf("btn: %s\n",lv_list_get_btn_text(btn));
		//lv_btn_set_checkable(btn, true);
		//lv_obj_set_click(btn, true);
		lv_obj_set_event_cb(btn, file_browser_open_file_event_cb);

	}
	lv_obj_t * btn = lv_list_add_btn(file_browser_list, LV_SYMBOL_FILE, " ");

#endif
}


LV_EVENT_CB_DECLARE(file_browser_open_file_event_cb)
{
	if (e == LV_EVENT_CLICKED) {
		memset(file_name, 0, sizeof(file_name));
		strcat(file_name, file_browser_dir);
		strcat(file_name, lv_list_get_btn_text(obj));
		printf("file_is: %s\n", file_name);

		printf("file_browser_open_file_event_cb dir_is: %s\n", file_browser_dir);

		struct stat stat_buf;
		if((stat(file_name,&stat_buf) == -1))
		{
			printf("stat error");
		}
		if(S_ISDIR(stat_buf.st_mode))
		{
			strcat(file_name, "/");
			file_browser_open(file_name);

		}
		else
		{
			lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
	        uint32_t delay = 200;
	        lv_100ask_demo_anim_bg(150, LV_100ASK_DEMO_GREEN, LV_100ASK_DEMO_BG_SMALL);

	        lv_obj_t * arc = add_loader(open_file_anim_ready);
	        lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, -40);

	        lv_obj_t * txt = lv_label_create(lv_scr_act(), NULL);
	        lv_label_set_text(txt, "Opening, please wait...");
	        lv_theme_apply(txt, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
	        lv_obj_align(txt, arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 60);

	        lv_100ask_demo_anim_top_to_bottom(arc, delay);
	        delay += LV_100ASK_DEMO_ANIM_DELAY;
	        lv_100ask_demo_anim_top_to_bottom(txt, delay);
		}
	}
}


static void open_file_anim_ready(lv_anim_t * a)
{

    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    open_file(file_name);  // 页面处理
}


static void open_file(const char * name)
{

	printf("open_file_name:%s\n\n", name);

	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_WHITE, LV_100ASK_DEMO_BG_NONE);  // 调整页面背景

    uint32_t delay = 200;

    lv_obj_t * back = add_back(back_to_file_browser_event_cb);
	//lv_obj_set_top(back, true);
    lv_100ask_demo_anim_top_to_bottom(back, delay);

    lv_obj_t * title = add_title("OPEN FILE");
    lv_100ask_demo_anim_top_to_bottom(title, delay);

	lv_obj_t * image = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(image, name);
	lv_obj_align(image, NULL, LV_ALIGN_CENTER, 0, 0);

	lv_obj_move_foreground(back);

}


LV_EVENT_CB_DECLARE(back_to_file_browser_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        file_browser_open(file_browser_dir);  // 页面处理
    }
}


LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_WHITE, LV_100ASK_DEMO_BG_FULL);
        home_open(200);
    }
}



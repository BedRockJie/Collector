/**
 * @file main.c
 *
 */
#include <unistd.h>
#include <sys/time.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/sunxifb.h"
#include "lv_drivers/indev/evdev.h"
#include "lv_100ask/lv_100ask.h"
#include "lv_lib_png/lv_png.h"


extern void lv_100ask_printer(lv_indev_t * indev);
extern void ask100_show_pic_stack(lv_task_t * task);
extern void ask100_update_time_task(lv_task_t * task);

// Display buffer size
#define DISP_BUF_SIZE (600 * LV_HOR_RES_MAX)


/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void);

#if 0 // TODO: 键盘功能
void lv_ex_keyboard_1(void)
{
    /*Create a keyboard and apply the styles*/
    lv_obj_t *kb = lv_keyboard_create(lv_scr_act(), NULL);
    lv_keyboard_set_cursor_manage(kb, true);

    /*Create a text area. The keyboard will write here*/
    lv_obj_t *ta = lv_textarea_create(lv_scr_act(), NULL);
	lv_obj_set_size(ta, 1024, 300);
	lv_obj_set_click(ta, true);
    //lv_obj_align(ta, NULL, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 16);
	lv_obj_align(ta, NULL, LV_ALIGN_IN_TOP_MID, 200, 120);
    lv_textarea_set_text(ta, "ls");
    lv_coord_t max_h = LV_VER_RES / 2 - LV_DPI / 8;
    if(lv_obj_get_height(ta) > max_h) lv_obj_set_height(ta, max_h);

    /*Assign the text area to the keyboard*/
    lv_keyboard_set_textarea(kb, ta);

		
	char ret[1024]={0};
    FILE *fp;

    fp = popen(lv_textarea_get_text(ta),"r");
    int nread = fread(ret,1,1024,fp);
    lv_textarea_set_text(ta, ret);
	lv_textarea_del_char(ta);
    

}
#endif



// TODO lv_indev_t * lv_indev_get_act(void);
int main(void)
{
    lv_disp_t * disp;
    uint32_t rotated = LV_DISP_ROT_90;
    /* LittlevGL init */
    lv_init();
	
    /* Linux frame buffer device init */
    //fbdev_init();
    sunxifb_init(rotated);
    /* A small buffer for LittlevGL to draw the screen's content */
    static lv_color_t buf[DISP_BUF_SIZE];

    /* Initialize a descriptor for the buffer */
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /* Initialize and register a display driver */
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    // disp_drv.sw_rotate = 1;
    disp_drv.flush_cb = sunxifb_flush;
    disp_drv.rotated = rotated;
    //注册显示设备
    disp = lv_disp_drv_register(&disp_drv);
	
	//hal_init
	//lv_disp_set_default lv_windows_disp


	/* Linux input device init */
    evdev_init();
	
    /* Initialize and register a display input driver */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/

    indev_drv.type = LV_INDEV_TYPE_POINTER;  //触摸板或鼠标
    indev_drv.read_cb = evdev_read;   //lv_gesture_dir_t lv_indev_get_gesture_dir(const lv_indev_t * indev)
   
    /* 为触摸屏绑定显示设备 */
    indev_drv.disp = disp;
    //注册触摸设备
    lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv); 

	 /* Initialize PNG decoder for LVGL */
	lv_png_init();
	
	
#if 1
	lv_100ask_printer(my_indev);
	//lv_task_t * systime_task = lv_task_create(ask100_update_time_task, 1000, _LV_TASK_PRIO_NUM, NULL);

	
	//lv_ex_keyboard_1();  // TODO
#endif

	//lv_demo_music();
	

	
    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_task_handler();
        usleep(5000);
		lv_tick_inc(5*1000);
    }

    return 0;
}


/**********************************************************************
 * 函数名称： custom_tick_get
 * 功能描述： 以毫秒为单位计算当前系统时间
 * 输入参数： 无
 * 输出参数： 系统运行了多少毫秒
 * 返 回 值： 无
 * 修改日期              版本号      修改人	      修改内容
 * -----------------------------------------------
 * 2020/11/11	     V1.0	  YobeZhou	  创建
 ***********************************************************************/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}




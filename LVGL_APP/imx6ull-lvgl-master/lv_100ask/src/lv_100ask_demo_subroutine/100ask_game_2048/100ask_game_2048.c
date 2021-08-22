/**
 * @file 100ask_game_2048.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../../lv_100ask_demo_theme/lv_100ask_demo_theme.h"


/*********************
 *      DEFINES
 *********************/
#define SIZE 4

/* 2048 Sizes */
#define LV_100ASK_2048_GAME_BOX_W				(500)
#define LV_100ASK_2048_GAME_BOX_H			    (500)
#define LV_100ASK_2048_NUMBER_BOX_W				(105)
#define LV_100ASK_2048_NUMBER_BOX_H				(105)
#define LV_100ASK_2048_CURRENT_SCORE_W			(135)
#define LV_100ASK_2048_CURRENT_SCORE_H			(75)
#define LV_100ASK_2048_BEST_SCORE_W				(135)
#define LV_100ASK_2048_BEST_SCORE_H				(75)


// 2048画框颜色
#define LV_100ASK_2048_GAME_BG_COLOR 		lv_color_hex(0xe8e5db) //LV_COLOR_MAKE(0xCD, 0xC1, 0xB4)
#define LV_100ASK_2048_GAME_BOX_COLOR 		lv_color_hex(0xBBADA0)
#define LV_100ASK_2048_NUMBER_BOX_COLOR 	lv_color_hex(0xCDC1B4)
#define LV_100ASK_2048_NUMBER_2_COLOR 		lv_color_hex(0xeee4da)
#define LV_100ASK_2048_NUMBER_4_COLOR 		lv_color_hex(0xeee1c9)
#define LV_100ASK_2048_NUMBER_8_COLOR 		lv_color_hex(0xf3b27a)
#define LV_100ASK_2048_NUMBER_16_COLOR 		lv_color_hex(0xf69664)
#define LV_100ASK_2048_NUMBER_32_COLOR 		lv_color_hex(0xf77c5f)
#define LV_100ASK_2048_NUMBER_64_COLOR 		lv_color_hex(0xf75f3b)
#define LV_100ASK_2048_NUMBER_128_COLOR 	lv_color_hex(0xedd073)
#define LV_100ASK_2048_NUMBER_256_COLOR 	lv_color_hex(0xEDCC61)
#define LV_100ASK_2048_NUMBER_512_COLOR 	lv_color_hex(0xEDCC61)
#define LV_100ASK_2048_NUMBER_1024_COLOR 	lv_color_hex(0xEDCC61)
#define LV_100ASK_2048_NUMBER_2048_COLOR 	lv_color_hex(0xEDC22E)



/**********************
 *  STATIC VARIABLES
 **********************/
bool play_game = true;
static uint8_t game_2048_board[SIZE][SIZE];
static uint32_t game_2048_current_score = 0;
static lv_obj_t * game_2048_canvas;
static lv_obj_t * game_2048_best_score;
static lv_obj_t * game_2048_current_score_text;
static lv_indev_t * play_2048_indev;


struct NUMBER_BOX
{
	lv_color_t number_cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(LV_100ASK_2048_NUMBER_BOX_W, LV_100ASK_2048_NUMBER_BOX_H)];
};


/**********************
 *  STATIC PROTOTYPES
 **********************/
LV_EVENT_CB_DECLARE(play_2048_event_cb);
LV_EVENT_CB_DECLARE(event_2048_game_up);
LV_EVENT_CB_DECLARE(event_2048_game_down);
LV_EVENT_CB_DECLARE(event_2048_game_left);
LV_EVENT_CB_DECLARE(event_2048_game_right);

LV_EVENT_CB_DECLARE(back_to_home_event_cb);

static void initBoard(lv_obj_t * parent, uint8_t board[SIZE][SIZE]);
static void initBoardNumber(uint8_t board[SIZE][SIZE]);
static void drawBoard(lv_obj_t * parent, lv_color_t * cbuf, char * number, \
							 uint16_t canvas_w, uint16_t canvas_h, \
							 uint16_t rect_dsc_x, uint16_t rect_dsc_y, \
							 lv_color_t rgb32);
static void addRandom(uint8_t board[SIZE][SIZE]);
static lv_color_t Num2Color(uint32_t num);
static uint8_t findTarget(uint8_t array[SIZE],uint8_t x,uint8_t stop);
static bool slideArray(uint8_t array[SIZE]);
static void rotateBoard(uint8_t board[SIZE][SIZE]);
static bool moveUp(uint8_t board[SIZE][SIZE]);
static bool moveLeft(uint8_t board[SIZE][SIZE]);
static bool moveDown(uint8_t board[SIZE][SIZE]);
static bool moveRight(uint8_t board[SIZE][SIZE]);
static bool findPairDown(uint8_t board[SIZE][SIZE]);
static uint8_t countEmpty(uint8_t board[SIZE][SIZE]);
static bool gameEnded(uint8_t board[SIZE][SIZE]);
static char* Int2String(int num,char *str);



#if 1
void lv_100ask_game_2048(lv_indev_t * indev)
//void lv_100ask_game_2048(void)
{
	play_2048_indev = indev;
	//play_2048_indev = lv_indev_get_act();

	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

	//lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_100ASK_2048_GAME_BG_COLOR);
#if 1
	lv_obj_t * bg_2048_game = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_clean_style_list(bg_2048_game, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_2048_game, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
			LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_2048_game, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
			LV_100ASK_2048_GAME_BG_COLOR);
	lv_obj_set_size(bg_2048_game, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_y(bg_2048_game, LV_100ASK_DEMO_BG_FULL);
#endif // 0
    //lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    //lv_100ask_demo_anim_bg(0, LV_100ASK_2048_GAME_BG_COLOR, LV_100ASK_DEMO_BG_FULL);  // 调整页面背景


	/* 创建画布 */
	static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(LV_100ASK_2048_GAME_BOX_W, LV_100ASK_2048_GAME_BOX_H)];
	game_2048_canvas = lv_canvas_create(lv_scr_act(), NULL);
	lv_obj_set_click(game_2048_canvas, true);
	lv_img_set_antialias(game_2048_canvas, false); // 禁用抗锯齿
	lv_canvas_set_buffer(game_2048_canvas, cbuf, LV_100ASK_2048_GAME_BOX_W, LV_100ASK_2048_GAME_BOX_H, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(game_2048_canvas, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_canvas_fill_bg(game_2048_canvas, LV_100ASK_2048_GAME_BOX_COLOR, LV_OPA_COVER);
	lv_obj_set_event_cb(game_2048_canvas, play_2048_event_cb);
	//lv_obj_set_event_cb(lv_scr_act(), play_2048_event_cb);
	lv_obj_set_event_cb(bg_2048_game, play_2048_event_cb);


	// lv_indev_t * lv_indev_get_act(void);

	initBoardNumber(game_2048_board);

	initBoard(game_2048_canvas, game_2048_board);

	add_back(back_to_home_event_cb);
	//lv_obj_t * back = add_back(back_to_home_event_cb);
	//lv_100ask_demo_anim_in(back, delay);

#if 0
	/* 按键控制 */
    lv_obj_t * btn_up = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_up, event_2048_game_left);
    lv_obj_align(btn_up, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 80*5);

    lv_obj_t * label_up = lv_label_create(btn_up, NULL);
    lv_label_set_text(label_up, "UP");


	//
	lv_obj_t * btn_down = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_down, event_2048_game_right);
    lv_obj_align(btn_down, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 80*6);

    lv_obj_t * label_down = lv_label_create(btn_down, NULL);
    lv_label_set_text(label_down, "DOWN");

	//
	lv_obj_t * btn_left = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_left, event_2048_game_up);
    lv_obj_align(btn_left, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 80*5);

    lv_obj_t * label_left = lv_label_create(btn_left, NULL);
    lv_label_set_text(label_left, "LEFT");

	//
	lv_obj_t * btn_right = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_right, event_2048_game_down);
    lv_obj_align(btn_right, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 80*6);

    lv_obj_t * label_right = lv_label_create(btn_right, NULL);
    lv_label_set_text(label_right, "RIGHT");




#endif

}
#endif



LV_EVENT_CB_DECLARE(play_2048_event_cb)
{
	//if (e == LV_EVENT_GESTURE)
	if (e == LV_EVENT_CLICKED)
	{
	    //static lv_indev_t * my_indev = lv_indev_get_act();
		// TODO 线程处理
		bool success = false;
		//printf("		 GAME BEGIN!			\n");

		switch(lv_indev_get_gesture_dir(play_2048_indev))
					{
						case LV_GESTURE_DIR_TOP:
							success = moveLeft(game_2048_board); break;
						case LV_GESTURE_DIR_BOTTOM:
							success = moveRight(game_2048_board); break;
						case LV_GESTURE_DIR_RIGHT:
							success = moveDown(game_2048_board); break;
						case LV_GESTURE_DIR_LEFT:
							success = moveUp(game_2048_board); break;
						default: success = false;
					}
					if (success)
					{
						initBoard(game_2048_canvas, game_2048_board);
						//usleep(150000);
						addRandom(game_2048_board);
						initBoard(game_2048_canvas, game_2048_board);
						if (gameEnded(game_2048_board))
						{
							printf("		 GAME OVER			\n");
							play_game = false;
						}
					}
	}


}


/* 按键方式 */
LV_EVENT_CB_DECLARE(event_2048_game_up)
{
	if (e == LV_EVENT_CLICKED) {
		if (moveUp(game_2048_board))
		{
			initBoard(game_2048_canvas, game_2048_board);
			//usleep(150000);
			addRandom(game_2048_board);
			initBoard(game_2048_canvas, game_2048_board);
			if (gameEnded(game_2048_board)) {
				printf("         GAME OVER          \n");
			}
		}
	}

}

LV_EVENT_CB_DECLARE(event_2048_game_down)
{
	if (e == LV_EVENT_CLICKED) {
		if (moveDown(game_2048_board))
		{
			initBoard(game_2048_canvas, game_2048_board);
			//usleep(150000);
			addRandom(game_2048_board);
			initBoard(game_2048_canvas, game_2048_board);
			if (gameEnded(game_2048_board)) {
				printf("         GAME OVER          \n");
			}
		}
	}
}

LV_EVENT_CB_DECLARE(event_2048_game_left)
{
	if (e == LV_EVENT_CLICKED) {
		if (moveLeft(game_2048_board))
		{
			initBoard(game_2048_canvas, game_2048_board);
			//usleep(150000);
			addRandom(game_2048_board);
			initBoard(game_2048_canvas, game_2048_board);
			if (gameEnded(game_2048_board)) {
				printf("         GAME OVER          \n");
			}
		}
	}
}

LV_EVENT_CB_DECLARE(event_2048_game_right)
{
	if (e == LV_EVENT_CLICKED) {
		if (moveRight(game_2048_board))
		{
			initBoard(game_2048_canvas, game_2048_board);
			//usleep(150000);
			addRandom(game_2048_board);
			initBoard(game_2048_canvas, game_2048_board);
			if (gameEnded(game_2048_board)) {
				printf("         GAME OVER          \n");
			}
		}
	}

}



static void initBoard(lv_obj_t * parent, uint8_t board[SIZE][SIZE])
{
	static struct NUMBER_BOX s_number_cbuf[SIZE][SIZE];


	/* 创建画布 */
	static lv_color_t best_source_cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(135, 75)];
	lv_obj_t * best_source_canvas = lv_canvas_create(lv_scr_act(), NULL);
	lv_obj_set_click(best_source_canvas, true);
	lv_img_set_antialias(best_source_canvas, false); // 禁用抗锯齿
	lv_canvas_set_buffer(best_source_canvas, best_source_cbuf, 135, 75, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(best_source_canvas, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 52);
	lv_canvas_fill_bg(best_source_canvas, LV_COLOR_SILVER, LV_OPA_COVER);

	/* 画矩形 */
	lv_draw_rect_dsc_t best_source_rect_dsc;
	lv_draw_rect_dsc_init(&best_source_rect_dsc);
	best_source_rect_dsc.bg_color = LV_100ASK_2048_GAME_BOX_COLOR;
	lv_canvas_draw_rect(best_source_canvas, 0, 0, 135, 75, &best_source_rect_dsc);

	/* 最高分提示 */
	lv_obj_t * best_source_tip_text  = lv_label_create(best_source_canvas, NULL); /* 创建标签 */
	lv_theme_apply(best_source_tip_text, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
	lv_label_set_text(best_source_tip_text, "BEST");
	lv_obj_align(best_source_tip_text, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);  /* 当前画布居中 */

	/* 最高分数 */
	game_2048_best_score = lv_label_create(best_source_canvas, NULL); /* 创建标签 */
	lv_theme_apply(game_2048_best_score, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
	lv_label_set_text(game_2048_best_score, "4096");
	lv_obj_align(game_2048_best_score, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);  /* 当前画布居中 */

	/* 创建画布 */
	static lv_color_t current_source_cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(135, 75)];
	lv_obj_t *  current_source_canvas = lv_canvas_create(lv_scr_act(), NULL);
	lv_obj_set_click(current_source_canvas, true);
	lv_img_set_antialias(current_source_canvas, false); // 禁用抗锯齿
	lv_canvas_set_buffer(current_source_canvas, current_source_cbuf, 135, 75, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(current_source_canvas, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 70+75);
	lv_canvas_fill_bg(current_source_canvas, LV_COLOR_SILVER, LV_OPA_COVER);

	/* 画矩形 */
	lv_draw_rect_dsc_t  current_source_rect_dsc;
	lv_draw_rect_dsc_init(&current_source_rect_dsc);
	current_source_rect_dsc.bg_color = LV_100ASK_2048_GAME_BOX_COLOR;
	lv_canvas_draw_rect(current_source_canvas, 0, 0, 135, 75, &current_source_rect_dsc);

	/* 当前分提示 */
	lv_obj_t *  current_source_tip_text  = lv_label_create(current_source_canvas, NULL); /* 创建标签 */
	lv_theme_apply(current_source_tip_text, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
	lv_label_set_text(current_source_tip_text, "SORCE");
	lv_obj_align(current_source_tip_text, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);  /* 当前画布居中 */

	/* 当前分数 */
	char str[5] = {0};
	game_2048_current_score_text = lv_label_create(current_source_canvas, NULL); /* 创建标签 */
	lv_theme_apply(game_2048_current_score_text, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
	lv_label_set_text(game_2048_current_score_text, Int2String(game_2048_current_score, str));
	lv_obj_align(game_2048_current_score_text, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);  /* 当前画布居中 */


	lv_obj_t * game_title = lv_label_create(lv_scr_act(), NULL);
	lv_theme_apply(game_title, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
	//lv_theme_apply(game_title, (lv_theme_style_t)LV_THEME_ARC);
	lv_label_set_long_mode(game_title, LV_LABEL_LONG_BREAK);	 /*Break the long lines*/
	lv_label_set_recolor(game_title, true); 					 /*Enable re-coloring by commands in the text*/
	lv_label_set_align(game_title, LV_ALIGN_IN_TOP_LEFT);		 /*Center aligned lines*/
	lv_obj_align(game_title, NULL, LV_ALIGN_IN_TOP_LEFT, -5, 80);
	lv_label_set_text(game_title, "#776e65 2048# ");
	lv_obj_set_width(game_title, 150);


#if 1
	lv_obj_t * game_tutorial = lv_label_create(lv_scr_act(), NULL);
	//lv_theme_apply(game_tutorial, (lv_theme_style_t)LV_100ASK_DEMO_THEME_CHINESE_CHARACTER);
	lv_label_set_long_mode(game_tutorial, LV_LABEL_LONG_BREAK);	 /*Break the long lines*/
	lv_label_set_recolor(game_tutorial, true); 					 /*Enable re-coloring by commands in the text*/
	lv_label_set_align(game_tutorial, LV_ALIGN_IN_TOP_LEFT);		 /*Center aligned lines*/
	lv_obj_align(game_tutorial, NULL, LV_ALIGN_IN_TOP_LEFT, 22, 120);
	lv_label_set_text(game_tutorial, "\nJoin the tiles, \nget to 2048!");
	//lv_label_set_text(game_tutorial, "合并色块，得出更大的数字！");
	lv_obj_set_width(game_tutorial, 200);
#endif

	for(int y = 0; y < SIZE; y++)
	{
		for(int x = 0; x < SIZE; x++)
		{
			if (board[x][y]!=0) {
				//sprintf(tmp_char, "%d", board[x][y]);
				char tmp_char[8];
				snprintf(tmp_char, 8, "%u", (uint32_t)1<<board[x][y]);
				drawBoard(parent, s_number_cbuf[x][y].number_cbuf, tmp_char,\
						  LV_100ASK_2048_NUMBER_BOX_W, LV_100ASK_2048_NUMBER_BOX_H, \
						  (16*(y+1))+(LV_100ASK_2048_NUMBER_BOX_W*(y+1-1)), (16*(x+1))+(LV_100ASK_2048_NUMBER_BOX_W*(x+1-1)), Num2Color((uint32_t)1<<board[x][y]));
			}
			else {
				drawBoard(parent, s_number_cbuf[x][y].number_cbuf, "",\
						  LV_100ASK_2048_NUMBER_BOX_W, LV_100ASK_2048_NUMBER_BOX_H, \
						  (16*(y+1))+(LV_100ASK_2048_NUMBER_BOX_W*(y+1-1)), (16*(x+1))+(LV_100ASK_2048_NUMBER_BOX_W*(x+1-1)), LV_100ASK_2048_NUMBER_BOX_COLOR);
			}
		}
	}
}


//10进制
static char* Int2String(int num,char *str)
{
	int i = 0;//指示填充str
	if(num<0)//如果num为负数，将num变正
	{
		num = -num;
		str[i++] = '-';
	}
	//转换
	do
	{
		str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0'
		num /= 10;//去掉最低位
	}while(num);//num不为0继续循环

	str[i] = '\0';

	//确定开始调整的位置
	int j = 0;
	if(str[0]=='-')//如果有负号，负号不用调整
	{
		j = 1;//从第二位开始调整
		++i;//由于有负号，所以交换的对称轴也要后移1位
	}
	//对称交换
	for(;j<i/2;j++)
	{
		//对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b;
		str[j] = str[j] + str[i-1-j];
		str[i-1-j] = str[j] - str[i-1-j];
		str[j] = str[j] - str[i-1-j];
	}

	return str;//返回转换后的值
}


static void drawBoard(lv_obj_t * parent, lv_color_t * cbuf, char * number, \
							 uint16_t canvas_w, uint16_t canvas_h, \
							 uint16_t rect_dsc_x, uint16_t rect_dsc_y, \
							 lv_color_t rgb32)
{

	/* 创建画布 */
	lv_obj_t * canvas = lv_canvas_create(parent, NULL);
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

	/* 数字 */
	lv_obj_t * icon  = lv_label_create(canvas, NULL); /* 创建标签 */
	lv_theme_apply(icon, (lv_theme_style_t)LV_100ASK_DEMO_THEME_TITLE);
	//lv_label_set_text(systime_task_label, "Welcome to www.100ask.net!");
	lv_label_set_text(icon, number);
	lv_obj_align(icon, NULL, LV_ALIGN_CENTER, 0, 0);  /* 当前画布居中 */

	lv_obj_set_event_cb(canvas, play_2048_event_cb);

}


static void initBoardNumber(uint8_t board[SIZE][SIZE]) {
	uint8_t x,y;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			board[x][y]=0;
		}
	}

	/* 初始化两个随机位置的随机数 */
	addRandom(board);
	addRandom(board);

	game_2048_current_score = 0;
}


static void addRandom(uint8_t board[SIZE][SIZE]) {
	static bool initialized = false;
	uint8_t x,y;
	uint8_t r,len=0;
	uint8_t n,list[SIZE*SIZE][2];

	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}

	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			if (board[x][y]==0) {
				list[len][0]=x;
				list[len][1]=y;
				len++;
			}
		}
	}

	if (len>0) {
		r = rand()%len;
		x = list[r][0];
		y = list[r][1];
		n = (rand()%10)/9+1;
		board[x][y]=n;
	}
}


static lv_color_t Num2Color(uint32_t num){
	switch (num)
	{
	case 2:		return LV_100ASK_2048_NUMBER_2_COLOR;	break;
	case 4:		return LV_100ASK_2048_NUMBER_4_COLOR;	break;
	case 8:		return LV_100ASK_2048_NUMBER_8_COLOR;	break;
	case 16:	return LV_100ASK_2048_NUMBER_16_COLOR;	break;
	case 32:	return LV_100ASK_2048_NUMBER_32_COLOR;	break;
	case 64:	return LV_100ASK_2048_NUMBER_64_COLOR;	break;
	case 128:	return LV_100ASK_2048_NUMBER_128_COLOR;	break;
	case 256:	return LV_100ASK_2048_NUMBER_256_COLOR;	break;
	case 512:	return LV_100ASK_2048_NUMBER_512_COLOR;	break;
	case 1024:	return LV_100ASK_2048_NUMBER_1024_COLOR;break;
	case 2048:	return LV_100ASK_2048_NUMBER_2048_COLOR;break;
	default:	return LV_100ASK_2048_NUMBER_2048_COLOR;break;
	}

}


static uint8_t findTarget(uint8_t array[SIZE],uint8_t x,uint8_t stop) {
	uint8_t t;
	// if the position is already on the first, don't evaluate
	if (x==0) {
		return x;
	}
	for(t=x-1;;t--) {
		if (array[t]!=0) {
			if (array[t]!=array[x]) {
				// merge is not possible, take next position
				return t+1;
			}
			return t;
		} else {
			// we should not slide further, return this one
			if (t==stop) {
				return t;
			}
		}
	}
	// we did not find a
	return x;
}

static bool slideArray(uint8_t array[SIZE]) {
	bool success = false;
	uint8_t x,t,stop=0;

	for (x=0;x<SIZE;x++) {
		if (array[x]!=0) {
			t = findTarget(array,x,stop);
			// if target is not original position, then move or merge
			if (t!=x) {
				// if target is zero, this is a move
				if (array[t]==0) {
					array[t]=array[x];
				} else if (array[t]==array[x]) {
					// merge (increase power of two)
					array[t]++;
					// increase score
					game_2048_current_score+=(uint32_t)1<<array[t];
					// set stop to avoid double merge
					stop = t+1;
				}
				array[x]=0;
				success = true;
			}
		}
	}
	return success;
}

static void rotateBoard(uint8_t board[SIZE][SIZE]) {
	uint8_t i,j,n=SIZE;
	uint8_t tmp;
	for (i=0; i<n/2; i++) {
		for (j=i; j<n-i-1; j++) {
			tmp = board[i][j];
			board[i][j] = board[j][n-i-1];
			board[j][n-i-1] = board[n-i-1][n-j-1];
			board[n-i-1][n-j-1] = board[n-j-1][i];
			board[n-j-1][i] = tmp;
		}
	}
}

static bool moveUp(uint8_t board[SIZE][SIZE]) {
	bool success = false;
	uint8_t x;
	for (x=0;x<SIZE;x++) {
		success |= slideArray(board[x]);
	}
	return success;
}

static bool moveLeft(uint8_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

static bool moveDown(uint8_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

static bool moveRight(uint8_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	return success;
}

static bool findPairDown(uint8_t board[SIZE][SIZE]) {
	bool success = false;
	uint8_t x,y;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE-1;y++) {
			if (board[x][y]==board[x][y+1]) return true;
		}
	}
	return success;
}

static uint8_t countEmpty(uint8_t board[SIZE][SIZE]) {
	uint8_t x,y;
	uint8_t count=0;
	for (x=0;x<SIZE;x++) {
		for (y=0;y<SIZE;y++) {
			if (board[x][y]==0) {
				count++;
			}
		}
	}
	return count;
}

static bool gameEnded(uint8_t board[SIZE][SIZE]) {
	bool ended = true;
	if (countEmpty(board)>0) return false;
	if (findPairDown(board)) return false;
	rotateBoard(board);
	if (findPairDown(board)) ended = false;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return ended;
}



LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
		play_game = false;
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_WHITE, LV_100ASK_DEMO_BG_FULL);
        home_open(0);
    }
}





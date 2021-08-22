#include "lv_100ask_cardtest.h"
#include "../../lv_100ask_demo_theme/lv_100ask_demo_theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <setjmp.h>
#include <errno.h>
#include <linux/input.h> 
#include <pthread.h>


//static camera data 
#define CAM_LOG_DEF 
#define CAM_DEV "/dev/video0"
#define JPG "/tmp/card_img/card_img.jpg"

#define   WIDTH   1280                      // 图片的宽度
#define   HEIGHT  720                       // 图片的高度
#define  NB_BUFFER  4   //memory block number
struct pic_data 
{
    unsigned char *tmpbuffer[NB_BUFFER];
    unsigned int tmpbytesused[NB_BUFFER];
}pic;
 
static int cam_fd;

static int device_init();
static int v4l2_init(void);
static int save_jpeg_img(void *addr,int length);
static int v4l2Grab(unsigned char save);
static int v4l2_close(void);

//static void my_event_cb(lv_obj_t * obj, lv_event_t event);

static lv_obj_t *keyboard_data;
static lv_obj_t * qrcode_create;
static char * qrcode_data[128];
//static lv_obj_t * ask100_show_pic;

static unsigned char card_img;

//socket dev
static int serv_sock ;
static struct sockaddr_in serv_addr;
static struct sockaddr_in clnt_addr;
static int clnt_sock;

//code run list example
static char card_run_mode = 0;
static char card_img_list;
//other thread run detect card
static void * updateCounter(void);
// 互斥锁
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static int rc1;
static pthread_t thread1;
static char buffer[40] = {0};
// 头文件  #include "../lv_100ask_demo.h"

//卡牌图片声明
LV_IMG_DECLARE(img_lv_100ask_rotate_pictures_1); // 狗头

LV_IMG_DECLARE(img_lv_100ask_card_heimoshushi);  //1
LV_IMG_DECLARE(img_lv_100ask_card_heimoshushi1);  //2
LV_IMG_DECLARE(img_lv_100ask_card_aoxitiankonglong); //3
LV_IMG_DECLARE(img_lv_100ask_card_fengyindeerkezuodiya); //4
LV_IMG_DECLARE(img_lv_100ask_card_fengyinyouwan); //5
LV_IMG_DECLARE(img_lv_100ask_card_fengyinyouzu);  //6
LV_IMG_DECLARE(img_lv_100ask_card_fengyinzuowan);  //7
LV_IMG_DECLARE(img_lv_100ask_card_fengyinzuozu);  //8
LV_IMG_DECLARE(img_lv_100ask_card_qngyanbailong);  //9
LV_IMG_DECLARE(img_lv_100ask_card_qngyanbailong1);  //10
LV_IMG_DECLARE(img_lv_100ask_card_taiyangshen); //11
LV_IMG_DECLARE(img_lv_100ask_card_yuyiliziqiu);  //12
LV_IMG_DECLARE(img_lv_100ask_card_qngyanjiujilong);  //13

LV_IMG_DECLARE(img_lv_100ask_card_jiantoushang);
LV_IMG_DECLARE(img_lv_100ask_card_jiantouxia);


LV_EVENT_CB_DECLARE(creat_btn_down_cb);
LV_EVENT_CB_DECLARE(creat_btn_up_cb);
LV_EVENT_CB_DECLARE(creat_qrcode_event_cb);
LV_EVENT_CB_DECLARE(back_to_home_event_cb);
LV_EVENT_CB_DECLARE(creat_find_event_cb);

/* 卡牌图数组 + 索引  */
static const lv_img_dsc_t *img_card_pictures[] = \
                                {&img_lv_100ask_card_heimoshushi,      &img_lv_100ask_card_heimoshushi1,\
                                 &img_lv_100ask_card_aoxitiankonglong, &img_lv_100ask_card_fengyindeerkezuodiya,\
                                 &img_lv_100ask_card_fengyinyouwan,    &img_lv_100ask_card_fengyinyouzu,\
                                 &img_lv_100ask_card_fengyinzuowan,    &img_lv_100ask_card_fengyinzuozu,\
                                 &img_lv_100ask_card_qngyanbailong,    &img_lv_100ask_card_qngyanbailong1,\
                                 &img_lv_100ask_card_taiyangshen,      &img_lv_100ask_card_yuyiliziqiu,\
                                 &img_lv_100ask_card_qngyanjiujilong
                                };

static void qrcode_open(void);
static void qrcode_anim_ready(lv_anim_t * a);
static int socket_init();
static void img_update_task(lv_task_t * task)
{
    v4l2Grab(0);
}
static void card_display_task(lv_task_t * task) 
{
    if(card_run_mode){
        if(!strcmp(buffer,"taiyangshen")){card_img_list = 10;}
        else if(!strcmp(buffer,"yuyiliziqiu")){card_img_list = 11;}
        else if(!strcmp(buffer,"fengyinzuowan")){card_img_list = 6;}
        else if(!strcmp(buffer,"fengyinzuozu")){card_img_list = 7;}
        else if(!strcmp(buffer,"fengyinyouwan")){card_img_list = 4;}
        else if(!strcmp(buffer,"fengyinyouzu")){card_img_list = 5;}
        else if(!strcmp(buffer,"qngyanjiujilong")){card_img_list = 12;}
        else if(!strcmp(buffer,"qngyanbailong")){card_img_list = 8;}
        else if(!strcmp(buffer,"fengyindeerkezuodiya")){card_img_list = 3;}
        else if(!strcmp(buffer,"aoxitiankonglong")){card_img_list = 2;}
        memset(buffer,0,sizeof(buffer)-1);
        printf("img list %d \n",card_img_list);
        lv_img_set_src(qrcode_create, img_card_pictures[card_img_list]);
        lv_obj_align(qrcode_create, NULL, LV_ALIGN_CENTER,-400,50);
        card_run_mode = 0;
    } 
}
void lv_demo_cardtest(void)
{
    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    
	uint32_t delay = 100;
	lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GREEN, LV_100ASK_DEMO_BG_FULL);

    lv_obj_t * arc = add_loader(qrcode_anim_ready);
	lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * txt = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(txt, "Opening, please wait...");
	lv_theme_apply(txt, (lv_theme_style_t)LV_100ASK_DEMO_THEME_LABEL_WHITE);
	lv_obj_align(txt, arc, LV_ALIGN_OUT_BOTTOM_MID, 0, 60);

	lv_100ask_demo_anim_top_to_bottom(arc, delay);
	delay += 2;
	lv_100ask_demo_anim_top_to_bottom(txt, delay);
    lv_task_create(img_update_task, 200, LV_TASK_PRIO_MID, NULL);
    lv_task_create(card_display_task, 1000, LV_TASK_PRIO_MID, NULL);
    
}


static void qrcode_anim_ready(lv_anim_t * a)
{

    device_init();
    v4l2_init();
    socket_init();
    lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
    qrcode_open();  // 页面处理
}
static int socket_init(){
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("socke init1\n");
    //进入监听状态，等待用户发起请求   理论你上最多有20个客户端可以建立连接
    listen(serv_sock, 20);

    socklen_t clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    printf("socke init2\n");
    return 0;
}

static void qrcode_open(void)
{
	lv_100ask_demo_anim_out_all(lv_scr_act(), 0);

    lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_GRAY, LV_100ASK_DEMO_BG_SMALL);  // 调整页面背景

    uint32_t delay = 200;

    lv_obj_t * back = add_back(back_to_home_event_cb);
    lv_100ask_demo_anim_top_to_bottom(back, delay);

    lv_obj_t * title = add_title("CARD DETECT");
    lv_100ask_demo_anim_top_to_bottom(title, delay);

	/* 二维码 */
	//strcpy(qrcode_data, "https://www.100ask.net");
	/*Create a 100x100 QR code*/
	//qrcode_create = lv_qrcode_create(lv_scr_act(), 200, lv_color_hex3(0x33f), lv_color_hex3(0xeef));
	//lv_obj_align(qrcode_create, NULL, LV_ALIGN_CENTER, -300, 0);
	//lv_qrcode_update(qrcode_create, "https://www.100ask.net", strlen("https://www.100ask.net"));

    
    qrcode_create = lv_img_create(lv_scr_act(), NULL);  //创建一个 img 句柄
    lv_img_set_src(qrcode_create, img_card_pictures[3]);
    lv_obj_align(qrcode_create, NULL, LV_ALIGN_CENTER,-400,50);    // 图片对位

#if 1
	/*Create a keyboard and apply the styles*/

    //lv_obj_t *kb = lv_keyboard_create(lv_scr_act(), NULL);
    //lv_keyboard_set_cursor_manage(kb, true);

    /*Create a text area. The keyboard will write here*/
    keyboard_data = lv_textarea_create(lv_scr_act(), NULL);
	//lv_obj_align(keyboard_data, NULL, LV_ALIGN_IN_TOP_MID, 100, 150);
    lv_obj_align(keyboard_data, qrcode_create, LV_ALIGN_IN_TOP_MID, 0, -25);
    lv_textarea_set_text(keyboard_data, "King Longdui YYDS");


    //lv_coord_t max_h = LV_VER_RES / 2 - LV_DPI / 8;
    //if(lv_obj_get_height(keyboard_data) > max_h) lv_obj_set_height(keyboard_data, max_h);

    /*Assign the text area to the keyboard*/
    //lv_keyboard_set_textarea(kb, keyboard_data);

#endif


	lv_obj_t * next_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_theme_apply(next_btn, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_CIRCLE);
    lv_obj_set_size(next_btn, 160, 60);
    lv_obj_align(next_btn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -200, -100);

    lv_obj_set_event_cb(next_btn, creat_qrcode_event_cb);
    lv_obj_set_style_local_value_str(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "DETECT");
    lv_obj_set_style_local_value_font(next_btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());

    lv_obj_t * next_btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_theme_apply(next_btn1, (lv_theme_style_t)LV_100ASK_DEMO_THEME_BTN_CIRCLE);
    lv_obj_set_size(next_btn1, 160, 60);
    lv_obj_align(next_btn1, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -400, -100);

    lv_obj_set_event_cb(next_btn1, creat_find_event_cb);
    lv_obj_set_style_local_value_str(next_btn1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "FIND");
    lv_obj_set_style_local_value_font(next_btn1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_subtitle());

    lv_obj_t * up_img_btn = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(up_img_btn, &img_lv_100ask_card_jiantoushang);
    lv_obj_align(up_img_btn, NULL, LV_ALIGN_IN_RIGHT_MID,-600,0);  // 位置
    lv_obj_set_event_cb(up_img_btn, creat_btn_up_cb);
    lv_obj_set_click(up_img_btn, true);

    lv_obj_t * down_img_btn = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(down_img_btn, &img_lv_100ask_card_jiantouxia);
    lv_obj_align(down_img_btn, NULL, LV_ALIGN_IN_RIGHT_MID,-200,0);   // 位置
    lv_obj_set_event_cb(down_img_btn, creat_btn_down_cb);
    lv_obj_set_click(down_img_btn, true);
}

LV_EVENT_CB_DECLARE(creat_qrcode_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
           
			//lv_qrcode_update(qrcode_create, lv_textarea_get_text(keyboard_data), strlen(lv_textarea_get_text(keyboard_data)));
            printf("Longdui YYDS!!\n");
            // creat img for /tmp/card_img/card_img.jpg
            v4l2Grab(1);
            //system("ls -al /mnt/SDCARD/ncnn_model_detect");
    	}
}


LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
    if (e == LV_EVENT_CLICKED) {
        close(clnt_sock);
        close(serv_sock);
        pthread_join( thread1, NULL);
        v4l2_close();
        lv_100ask_demo_anim_out_all(lv_scr_act(), 0);
        lv_100ask_demo_anim_bg(0, LV_100ASK_DEMO_WHITE, LV_100ASK_DEMO_BG_FULL);
        home_open(0);
    }
}


LV_EVENT_CB_DECLARE(creat_btn_up_cb)
{
    if(e == LV_EVENT_CLICKED){
        printf("creat_btn_up222_cb!!\n");
    }
}


LV_EVENT_CB_DECLARE(creat_btn_down_cb)
{
    if(e == LV_EVENT_CLICKED){
        printf("creat_btn_222down_cb!!\n");
    }
}

LV_EVENT_CB_DECLARE(creat_find_event_cb)
{
    if(e == LV_EVENT_CLICKED){
        printf("creat_find_222down_cb!!\n");
        if( (rc1=pthread_create( &thread1, NULL, &updateCounter, NULL)) )
        {
            printf("Thread creation failed: %d\n", rc1);
        }
    }
}
static void * updateCounter(void){
   pthread_mutex_lock( &mutex1 );

    char str[] = "/tmp/card_img/card_img.jpg";
    
    printf("Send Message : %s\n",str);
    write(clnt_sock, str, sizeof(str));
    read(clnt_sock,buffer,sizeof(buffer));
    printf("%s\n",buffer);
    card_run_mode = 1;
   pthread_mutex_unlock( &mutex1 );
}



static int device_init(){
    #ifdef CAM_LOG_DEF
    struct v4l2_capability 	cap;	/* decive fuction, such as video input */
	struct v4l2_format 		tv_fmt; /* frame format */  
	struct v4l2_fmtdesc 	fmtdesc;  	/* detail control value */
	struct v4l2_control 	ctrl;
	int ret;
    #endif
    // camera init
    if((cam_fd = open(CAM_DEV,O_RDWR)) == -1)
    {
        perror("ERROR opening V4L interface.");
        return -1;
    }

    #ifdef CAM_LOG_DEF
    /*show all the support format*/
	memset(&fmtdesc, 0, sizeof(fmtdesc));
	fmtdesc.index = 0 ;                 /* the number to check */
	fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
 
	/* check video decive driver capability */
	if(ret=ioctl(cam_fd, VIDIOC_QUERYCAP, &cap)<0)
	{
		fprintf(stderr, "fail to ioctl VIDEO_QUERYCAP \n");
		exit(EXIT_FAILURE);
	}
	
	/*judge wherher or not to be a video-get device*/
	if(!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))
	{
		fprintf(stderr, "The Current device is not a video capture device \n");
		exit(EXIT_FAILURE);
	}
 
	/*judge whether or not to supply the form of video stream*/
	if(!(cap.capabilities & V4L2_CAP_STREAMING))
	{
		printf("The Current device does not support streaming i/o\n");
		exit(EXIT_FAILURE);
	}
	
	printf("\ncamera driver name is : %s\n",cap.driver);
	printf("camera device name is : %s\n",cap.card);
	printf("camera bus information: %s\n",cap.bus_info);
 
		/*display the format device support*/
	while(ioctl(cam_fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
	{	
		printf("\nsupport device %d.%s\n\n",fmtdesc.index+1,fmtdesc.description);
		fmtdesc.index++;
	}

    #endif
    
}

static int v4l2_init(void)
{
    int i;
    int ret = 0;
 
    // 1、Open camera device
    // if((cam_fd = open(CAM_DEV,O_RDWR)) == -1)
    // {
    //     perror("ERROR opening V4L interface.");
    //     return -1;
    // }
 
    // 2、Judge if the device is a camera device 
    struct v4l2_capability cam_cap;
    if(ioctl(cam_fd,VIDIOC_QUERYCAP,&cam_cap) == -1)
    {
        perror("Error opening device %s: unable to query device.");
        return -1;
    }
    //判断是否视频捕获设备
    if((cam_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) 
    {
        perror("ERROR video capture not supported.");
        return -1;
    }
 
    // 3、Setting output parameter.
    struct v4l2_format v4l2_fmt;
    v4l2_fmt.type = V4L2_CAP_VIDEO_CAPTURE;
    v4l2_fmt.fmt.pix.width = WIDTH;
    v4l2_fmt.fmt.pix.height = HEIGHT;
    v4l2_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    if (ioctl (cam_fd, VIDIOC_S_FMT, &v4l2_fmt) == -1) 
    {   
        perror("ERROR camera VIDIOC_S_FMT Failed.");
        return -1;
    }
    // 4、Check whether the parameters are set successfully 
    if (ioctl (cam_fd, VIDIOC_G_FMT, &v4l2_fmt) == -1) 
    {
        perror("ERROR camera VIDIOC_G_FMT Failed.");
        return -1;
    }
    if (v4l2_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
    {
        printf("Set VIDIOC_S_FMT sucessful\n");
    }
    // 5、Require buffer to store image data
    struct v4l2_requestbuffers v4l2_req;
    v4l2_req.count = NB_BUFFER;
    v4l2_req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_req.memory = V4L2_MEMORY_MMAP;
    if (ioctl (cam_fd, VIDIOC_REQBUFS, &v4l2_req) == -1) 
    {
        perror("ERROR camera VIDIOC_REQBUFS Failed.");
        return -1;
    } 
    // 6、Start memory map
    struct v4l2_buffer v4l2_buf;
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
      for(i = 0; i < NB_BUFFER; i++) 
   {
        v4l2_buf.index = i;
        if(ioctl(cam_fd, VIDIOC_QUERYBUF, &v4l2_buf) < 0)
        {
            perror("Unable to query buffer.");
            return -1;
        }
 
        pic.tmpbuffer[i] = (unsigned char*)mmap(NULL, v4l2_buf.length, PROT_READ, MAP_SHARED, cam_fd, v4l2_buf.m.offset);
        if(pic.tmpbuffer[i] == MAP_FAILED)
        {
             perror("Unable to map buffer.");
             return -1;
        }
        if(ioctl(cam_fd, VIDIOC_QBUF, &v4l2_buf) < 0)
        {
            perror("Unable to queue buffer.");
            return -1;
        }
   }
    //7、Open stream input 
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(cam_fd, VIDIOC_STREAMON, &type) < 0)
    {
        perror("Unable to start capture.");
        return -1;
    }
    return 0;
}
static int v4l2_close(void)
{
    // Remove mmap.
    int i;
    for(i=0; i<NB_BUFFER; i++)
        munmap(pic.tmpbuffer[i],pic.tmpbytesused[i]);
    close(cam_fd);
    return 0;

}
//  input foldname
static int v4l2Grab(unsigned char save)
{
    //8、Get a image 
    struct v4l2_buffer buff;
    buff.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buff.memory = V4L2_MEMORY_MMAP;
    if(ioctl(cam_fd, VIDIOC_DQBUF, &buff) < 0)  //VIDIOC_DQBUF// 从队列中取出帧 
    {
        printf("camera VIDIOC_DQBUF Failed.\n");
        usleep(1000*1000);
        return -1;
    }
 
    pic.tmpbytesused[buff.index] = buff.bytesused;
    //printf("size : %d",pic.tmpbytesused[buff.index]);           
 
    //9、Save image.
    if(save == 1)
        save_jpeg_img(pic.tmpbuffer[buff.index], pic.tmpbytesused[buff.index]);
    // int jpg_fd = open("1.jpeg",O_RDWR|O_CREAT,00700);
    // if(jpg_fd == -1)
    // {
    //     printf("open ipg Failed!\n ");
    //     return -1;      
    // }
    // int writesize = write(jpg_fd, pic.tmpbuffer[buff.index], pic.tmpbytesused[buff.index]);
    // printf("Write successfully size : %d\n",writesize);
    // close(jpg_fd);
    
    //10、Queue the buffers.  将取出的帧放回缓存区
    if(ioctl(cam_fd, VIDIOC_QBUF, &buff) < 0)// VIDIOC_QBUF 把帧放入队列
    {
        printf("camera VIDIOC_QBUF Failed.");
        usleep(1000*1000);
        return -1;
    }
    return 0;
}

static int save_jpeg_img(void *addr,int length)
{
    FILE *fp;
	//sprintf(image_name, JPG , flodname , num++);
    printf("img path is %s \n",JPG);
	if((fp = fopen(JPG, "w")) == NULL)
	{
		perror("Fail to fopen");
		exit(EXIT_FAILURE);
	}
	fwrite(addr, length, 1, fp);
	
	fclose(fp);
	return 0;
}
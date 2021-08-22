#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <setjmp.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h> 
#include <sys/types.h> 


#define CAM_LOG_DEF 

#define CAM_DEV "/dev/video0"
#define DEV_PATH "/dev/input/event0"   //difference is possible 
// define format foldname 
#define JPG "./%s/img%d.jpg"


#define   WIDTH   1280                      // 图片的宽度
#define   HEIGHT  720                       // 图片的高度
#define  NB_BUFFER  4   //memory block number
struct pic_data 
{
    unsigned char *tmpbuffer[NB_BUFFER];
    unsigned int tmpbytesused[NB_BUFFER];
}pic;
 
int cam_fd;
int keys_fd; 
struct input_event t;  

static int device_init(){
    #ifdef CAM_LOG_DEF
    struct v4l2_capability 	cap;	/* decive fuction, such as video input */
	struct v4l2_format 		tv_fmt; /* frame format */  
	struct v4l2_fmtdesc 	fmtdesc;  	/* detail control value */
	struct v4l2_control 	ctrl;
	int ret;
    #endif
    // key init 
    keys_fd=open(DEV_PATH, O_RDONLY);  
    if(keys_fd <= 0)  
    {  
        printf("open /dev/input/event2 device error!\n");  
        return -1;  
    }
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
/*
Description.:Initalize V4L2 driver.
*/
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
// save one jpeg img function 
static int save_jpeg_img(void *addr,int length,char *flodname)
{
    FILE *fp;
 
	static int num = 0;
 
	char image_name[30];
    if(num == 0)
        if(mkdir(flodname,0755) != 0){
            printf("flod search error\n");
            return 0 ;
        }
	sprintf(image_name, JPG , flodname , num++);
    printf("img path is %s \n",image_name);
	if((fp = fopen(image_name, "w")) == NULL)
	{
		perror("Fail to fopen");
		exit(EXIT_FAILURE);
	}
	fwrite(addr, length, 1, fp);
	usleep(500);
	fclose(fp);
	return 0;
}
/*
Description.:Get a jpeg image and save.
*/
static int v4l2Grab(char * foldname)
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
    printf("size : %d\n",pic.tmpbytesused[buff.index]);           
 
    //9、Save image.
    save_jpeg_img(pic.tmpbuffer[buff.index], pic.tmpbytesused[buff.index],foldname);
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
 
/*
Description.:Release resource
*/

static int v4l2_close(void)
{
    // Remove mmap.
    int i;
    for(i=0; i<NB_BUFFER; i++)
        munmap(pic.tmpbuffer[i],pic.tmpbytesused[i]);
    close(cam_fd);
    return 0;

}
/*
Description.:main
*/
int main(int argc, char* argv[])
{
    if(argc < 2){
        printf("    Please input foldname(of picture name)    \n");
    }
    else{
        device_init();
        v4l2_init();
        while(1)
        {     
            if(read(keys_fd, &t, sizeof(t)) == sizeof(t))  
            { 
                if(t.type==EV_KEY)
                    if( t.value==1) {//按键按下为1 松开为0  
                        v4l2Grab(argv[1]);
                    }  
                        
            }
        }
        v4l2_close();            
        return 0;
    }
    
}

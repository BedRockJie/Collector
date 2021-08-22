#include <stdio.h>  
#include <linux/input.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>

#define DEV_PATH "/dev/input/event0"   //difference is possible  

int main(int argc, char *argv[])  
{  
    int keys_fd;  
    char ret[2];  
    struct input_event t;
    if(argc <2)
    {
    printf("input device error\n");
    return -1;
    }  
    keys_fd=open(DEV_PATH, O_RDONLY);  
    if(keys_fd <= 0)  
    {  
        printf("open /dev/input/event0 device error!\n");  
        return -1;  
    }
    // if(read(keys_fd, &t, sizeof(t)) == sizeof(t)) 
    //     printf("t.type:%d",t.type);
    while(1)  
    {  
        if(read(keys_fd, &t, sizeof(t)) == sizeof(t))  
        { 
            //表示当前设备为  键盘设备 
            if(t.type==EV_KEY )  
        //    if(t.type==EV_MSC )  
                //if(t.value==0 || t.value==1)  
                if( t.value==1)  //按键按下为1 松开为0
                {  
                    printf("key %d \n", t.code);  
                    if(t.code == KEY_ESC)  
                        break;  
                }  

       }  
    }  
    close(keys_fd);  
    return 0;  
} 

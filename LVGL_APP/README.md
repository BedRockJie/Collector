# 说明
此文件夹下为客户端APP源码

## APP 开发说明
APP 程序 主要为界面显示，及构建说明。

### 程序入口
```
LVGL_APP\imx6ull-lvgl-master\lv_100ask\src\lv_100ask_demo\lv_100ask_demo.c
```
该文件为程序入口，调用 lv_100ask_printer 函数。

该文件为系统桌面构建文件，如要添加app，一般分为如下几个步骤：
- LV_IMG_DECLARE( ) 注册app图标
- LV_EVENT_CB_DECLARE（）注册回调函数
- lv_100ask_event_cb 添加回调函数至数组中
- LV_EVENT_CB_DECLARE 写回调函数入口
  - 回调函数中应关闭 in_desktop_page = false;
  - 然后进入app内部回调函数
  - 将头文件声明进LVGL_APP\imx6ull-lvgl-master\lv_100ask\lv_100ask.h中
- 在app函数中构建自己的app


图片文件应当使用lvgl官方进行解码，但是官方的工具比较繁琐一次只能转换一张。
借鉴pengzhihui大佬 的转换方法：https://github.com/peng-zhihui/HoloCubic/tree/main/3.Software

转换后的文件保存在：LVGL_APP\imx6ull-lvgl-master\lv_100ask\src\lv_100ask_demo\assets中


以上就是APP开发具体说明。
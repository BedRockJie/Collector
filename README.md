# Collector 收集者
- Card_detect and show animation  在全志D1上玩卡牌识别播放动画
   ![collector](./Docs/image/1_App_contol.png)
   项目来源于Kirin。其项目创意及初衷见帖下方评论：https://bbs.aw-ol.com/topic/223/

   具体实现过程也在帖子中😁😁😁


# 名字由来
   卡牌识别游戏机历史起源不做探究，由于其识别加栈实的效果，所以为它起名为Collertor收集者。并非英雄联盟中的收集者，具有着斩杀效果，而是漫威中的收集者，其拥有者长生不老与遇见未来的能力，我们的卡牌也寓意着这样的意义。



# 硬件准备
- 哪吒开发板一块
- MIPI屏幕一块
- USB摄像头一块
- 32G SD卡一张

连接如图：
![card_line_img](./Docs/image/2_Card_Mement.png)

# 构建
## 1、首先安装ncnn

参考：https://github.com/Tencent/ncnn/wiki/how-to-build#build-for-allwinner-d1

## 2、源码下载
确认ncnn安装目录。更改`ncnn_card_detect\CMakeLists.txt` **20**行 `ncnn_DIR` 变量为ncnn安装目录。

```shell
git clone https://github.com/jie2311260561/Collector.git
# 服务端编译
cd ncnn_card_detect
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../c906.toolchain.cmake ..
build -j4
# 客户端编译
cd LVGL_APP/imx6ull-lvgl-master
make -j4
```

服务端生成文件为： `build/carddetect`  
客户端生成文件为： `LVGL_APP\imx6ull-lvgl-master\output\card22222`


# 运行
## 1、拷贝模型文件至小机端SD卡

```shell
/mnt/SDCARD/yolo_model/best-opt.param
/mnt/SDCARD/yolo_model/best-opt.bin
```

## 2、创建临时文件路径
```Shell
mkdir /tmp/card_img
```
## 3、运行可执行文件

# 源码目录

``` shell
├─device_test_demo #外设测试demo 目录
│  ├─Camer_test
│  └─Key_test
├─Docs # 文档目录
│  ├─Dos
│  └─image
├─LVGL_APP  # 应用端程序
│  └─imx6ull-lvgl-master
│      ├─build # cmake 编译目录
│      ├─cmakebuildtools # cmake 配置目录
│      ├─lvgl # LVGl主目录
│      ├─lv_100ask # app目录
│      │  ├─lib
│      │  └─src
│      │      ├─100ask_demo_c_array_images # 图片文件转码文件入口
...
│      │      ├─lv_100ask_demo # app 桌面入口程序
│      │      ├─lv_100ask_demo_subroutine
...
│      │      │  ├─100ask_cardtest # app开发入口程序
...
│      │      ├─lv_100ask_demo_theme
│      │      ├─lv_100ask_examples
│      │      └─lv_100ask_font
│      ├─lv_drivers  #
│      ├─lv_examples
│      ├─lv_lib_png
│      └─output  #可执行文件目录
├─ncnn_card_detect # 服务端程序
├─ncnn_module # ncnn训练模型及转换好的模型
├─script # 开发过程中用到的脚本
├─yolov5_module # yolov5训练出来的模型
└─yolov5_script # yolov5训练模型中用到的脚本
```



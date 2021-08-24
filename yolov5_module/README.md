# 说明
此文件夹中存放的是 yolov5 模型训练出来和转换后的模型文件


# 模型转化指引

参考博客：[YOLOv5转NCNN过程](https://blog.csdn.net/woshicver/article/details/109699322?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0.control&spm=1001.2101.3001.4242)


## 需要注意
 
YOLO需要下载Relase版本的，目前正在维护的版本可能会出现问题。


```shell
python models/export.py --weights yolov5s.pt --img 640 --batch 1
python -m onnxsim yolov5s.onnx yolov5s-sim.onnx
```
 
这两句执行后可能会不会执行结束，但不会保存，如果出现了，需要更改 export 源码。


```
onnx2ncnn yolov5s-sim.onnx yolov5s.param yolov5s.bin
```
出现错误了不要惊慌，直接改模型文件。这个错误主要是输入ncnn不认识，把它该成自定义层，然后在ncnn中实现这个层就好了。
```
Unsupported slice step !
Unsupported slice step !
Unsupported slice step !
Unsupported slice step !
Unsupported slice step !
Unsupported slice step !
Unsupported slice step !
Unsupported slice step !
```

然后按照源码中的实现就好了！！
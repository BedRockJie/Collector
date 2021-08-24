# YOLOv5 模型训练指引

## 训练环境搭建

## 制作数据集
- 方法一：数据集制作参考 拍照 demo。
- 方法二：拍视频，可以使用全志官方xpalyerdemo。然后使用 ffmpeg 进行帧切图片处理。 
   
其目录如下：
- …images # 存放图片
- …Annotations # 存放图片对应的xml文件
- …ImageSets/Main #之后会在Main文件夹内自动生成train.txt，val.txt，test.txt和- - trainval.txt四个文件，存放训练集、验证集、测试集图片的名字（无后缀.jpg）



## 数据集label脚本制作
- 使用 labelImage 标记，制作训练数据集。  
当然，labelImage 标记的数据集是 xml 格式，而YOLOv5是按照txt格式来读取的，所以就需要我们对数据集进行处理。
1. split_train_val.py  进行处理
2. voc_label.py 转换成 txt 文件


目录可以参考：[yolov5训练自己的数据集](https://blog.csdn.net/qq_36756866/article/details/109111065)



## 执行训练脚本
1. 下载预训练模型
源码中在yolov5目录下的weights文件夹下提供了下载四种预训练模型的脚本----download_weights.sh
2. 在train.py进行以下几个修改：
```
epochs：指的就是训练过程中整个数据集将被迭代多少次,显卡不行你就调小点。
batch-size：一次看完多少张图片才进行权重更新，梯度下降的mini-batch,显卡不行你就调小点。
cfg：存储模型结构的配置文件
data：存储训练、测试数据的文件
img-size：输入图片宽高,显卡不行你就调小点。
rect：进行矩形训练
resume：恢复最近保存的模型开始训练
nosave：仅保存最终checkpoint
notest：仅测试最后的epoch
evolve：进化超参数
bucket：gsutil bucket
cache-images：缓存图像以加快训练速度
weights：权重文件路径
name： 重命名results.txt to results_name.txt
device：cuda device, i.e. 0 or 0,1,2,3 or cpu
adam：使用adam优化
multi-scale：多尺度训练，img-size +/- 50%
single-cls：单类别的训练集
```
```shell
python train.py --img 640 --batch 16 --epoch 300 --data data/ab.yaml --cfg models/yolov5s.yaml --weights weights/yolov5s.pt --device 'cpu'     # 0号GPU
```

## 训练结果可视化
使用如下工具
``` shell
tensorboard --logdir=runs
```




## 模型测试
使用如下命令
```shell
 python detect.py --weights runs/exp1/weights/best.pt --source inference/images/ --device 0 --save-txt
```

运行结果就在 run/detect/ 中。


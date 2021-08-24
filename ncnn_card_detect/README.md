# 说明
此文件夹为服务端源码


## 服务端开发说明(ncnn 部署推理说明)

服务端 ncnn 主要实现自定义层，修改模型中的文件格式和参数。

### 定义了模型推理的结果：

```
static const char* class_names[] = {
        "yuyiliziqiu", "taiyangshen","fengyinzuowan","fengyinzuozu",
        "fengyinyouwan","fengyinyouzu","qngyanjiujilong",
        "qngyanbailong","fengyindeerkezuodiya","aoxitiankonglong"
    };

```

### 加入进程间socket通信
将结果写如sock通信，这样做的好处是可以实现多机通信，接入网络，打开端口就可以实现多机通信。   
```c
write(sock,text,strlen(text));
```

同时也为后面开发多人联机提供思路。  

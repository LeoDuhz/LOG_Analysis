# newVisionModule

## 编译

删除share/proto，把medusa编译的proto拉进来

在Qt中编译

## 图形化界面

把main.qml文件放进Qtbuild的文件夹中

Qt报错Plugin initialization failed: Cannot create OpenGL context:

因为ubuntu下qt无法加载qmldesigner qmlprofiler

打开qt->help->about plugins

把qmldesigner qmlprofiler上的 √ 去掉

重启电脑,再点 √ ,重新编译项目

# ssl-logtools

## 安装需求库

sudo apt --fix-broken install

sudo apt-get -qq -y install cmake g++ git libprotobuf-dev protobuf-compiler zlib1g-dev libboost-progra\m-options-dev

## 编译（auto_linux.sh）

mkdir build

cd build

cmake ..

make

## 读取log文件（run_logplayer.sh）

进入build的bin中运行

./logplayer



# Pytorch Geometric

## 环境配置

先看

https://blog.csdn.net/weixin_44502066/article/details/97809006

再看

https://pytorch-geometric.readthedocs.io/en/latest/notes/installation.html

## GAT例程

介绍

https://baijiahao.baidu.com/s?id=1671028964544884749&wfr=spider&for=pc

论文

https://blog.csdn.net/weixin_36474809/article/details/89401552

代码

https://codechina.csdn.net/mirrors/diego999/pygat

报错

CUDA error: CUBLAS_STATUS_INTERNAL_ERROR when calling `cublasCreate(handle)

解决

是GPU配置出错,在import torch之前添加

import os

os.environ['CUDA_VISIBLE_DEVICES'] = "2"
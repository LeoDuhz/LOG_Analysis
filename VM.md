# newVisionModule-for-ssl

## 在Qt中编译

删除share/proto，把medusa编译的proto拉进来

## 图形化界面

把main.qml文件放进Qtbuild的文件夹中



# ssl-logtools

## 安装需求库

sudo apt --fix-broken install

sudo apt-get -qq -y install cmake g++ git libprotobuf-dev protobuf-compiler zlib1g-dev libboost-progra\m-options-dev

## 编译

mkdir build

cd build

cmake ..

make

## 读取log文件

进入build的bin中运行

./logplayer
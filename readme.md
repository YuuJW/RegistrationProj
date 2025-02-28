1.必须安装视频解码库
sudo apt install ubuntu-restricted-extras

2.环境
ubuntu+QT5.9.9+OpenCV3.4
采用CMakeLists作为项目文件
导入方法，直接用QTcreator打开CmakeLists文件
或者
mkdir build
cd build
cmake ..
make
./RegistrationProj

3.功能：
获得读取的RGB和Infra视频地址
获得保存配准文件视频地址
播放视频文件
进度条和状态接口

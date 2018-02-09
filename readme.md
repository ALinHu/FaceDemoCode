Linux下使用Qt5.9.2编译
1. genFaceFeature用于生成一个目录下的所有人脸feature。xxxx.jpg检测识别到人脸后，生成xxxx.dat
2. 程序启动时加载faceDB目录下的所有xxxx.dat文件
3. 程序有三个部分：
(1) registerCamera, 尚有一些小bug，未完全实现
     从camera注册人脸，同时可以进行识别
(2) registerVideoFiel, 尚未完全实现
(3) faceRecognition
    九安相机将识别的人脸图像文件上传到ftp，或者其他相机通过http上传
    通过线程检测图像文件变化。在配置文件config.ini中设置监测目录路径
    在另一线程对文件进行人脸检测和识别，人脸数据库通过genFaceFeature生成，通过文件存储
    界面显示识别检索结果




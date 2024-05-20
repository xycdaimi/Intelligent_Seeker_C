# 视力障碍智能家居寻物系统(C++版本)工程说明

## 工程文件结构

1. *.vscode文件夹用于保存vscode工程配置(不使用vscode的话，可以直接删除)*
2. *build文件夹用于保存编译结果*
3. *include文件夹用于保存头文件*
4. install文件夹用于分发软件（保存了运行环境）
5. *libs文件夹用于保存库文件*
6. *src文件夹用于保存cpp代码文件*
7. *test文件夹用于保存模块测试文件(项目编译时,该文件夹不参与编译)*

## 工程编译

1. *按照工程文件结构将文件放置在对应文件夹内*
2. *通过 bash 直接运行 compile.sh 即可*
3. 可执行文件保存为./install/Seeker , 可直接在pc端启动运行

## 环境配置

1. 配置OpenCV4.5.0(运行EnvironmentalConfiguration.sh)
2. 安装snowboy（教程： [https://blog.csdn.net/qq_38113006/article/details/105745564](https://blog.csdn.net/qq_38113006/article/details/105745564))
3. 在install/SpeechRecognition.py里加上自己的百度api接口密钥

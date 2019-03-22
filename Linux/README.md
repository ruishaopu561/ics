# Linux
这篇md主要记录一些比较零碎的、Linux里经常用到的命令、指令、文件的配置等等。。。
## Contents
- [GitHub](#github)
- [GDB](#gdb)
- [VS Code](#vscode)
- [Windows](#exe)
- [dpkg](#dpkg)
- [ssr](#ssr)
<span id="github"></span>
## GitHub
### md文件插入图片
在github中创建一个文件夹，名字随意。然后将需要用到的图片push到该文件夹中，然后点开图片，copy地址。
在md文件中写入：
```
![visible words when picture is invislble](copy path)
```
<span id="gdb"></span>
## GDB
GDB Commands:
```
gdb <file>  
break FUNC | *ADDR  
run  
print</?> $REG | ADDR
continue | stepi| nexti
quit
```
<span id="vscode"></span>
## vscode配置C++
其中Launch.json中的"preLaunchTask"值要与tasks.json中的"Label"值对上，然后Launch.json中主要修改"program"，tasks.json中修改"command"。
式子中具体命令或字符串的含义，自己去查吧。  
- [Launch.json](https://github.com/ruishaopu561/ics/blob/produce/Linux/Launch.json)
- [tasks.json](https://github.com/ruishaopu561/ics/blob/produce/Linux/tasks.json)  
- [c_cpp_properties.json](https://github.com/ruishaopu561/ics/blob/produce/Linux/c_cpp_properties.json)
<span id="exe"></span>
## 安装windows程序
[2018年wine QQ最完美解决方案(多Linux发行版通过测试并稳定运行)](https://www.lulinux.com/archives/1319)
<span id="dpkg"></span>
## dpkg使用
安装命令：
```
sudo dpkg -i deb文件名
```
如果出现问题，可以用以下命令修复安装：
 ```
 sudo apt-get install -f
 ```
 卸载  
 用以下命令查找已安装:  
 ```
 sudo dpkg -l
 ```
 用以下命令进行卸载：  
 ```
 sudo dpkg -r软件名
 ```
<span id="ssr"></span>
## 配置ssr代理
参考网页：(search "ubuntu ssr 客户端")  
-[ubuntu下ssr安装与使用](http://cache.baiducontent.com/c?m=9d78d513d98411e804abd3690d679627594380122ba7a4020ea28438e3732844506793ac57260775a3d13b275fa0131aacb2776536703daade8dcd5dddccca737cd4666e370b8636438e46b2895b73c522c35dbaae19e3baf137&p=9e759a4ed1d817e60be296271156&newp=86759a4ed18518b905f5c7710f0c92695d0fc20e3cd0c44324b9d71fd325001c1b69e7bf23211701d4c0796207ad4859e1f53170301766dada9fca458ae7c46e769e7a2c&user=baidu&fm=sc&query=ubuntu+ssr+%BF%CD%BB%A7%B6%CB&qid=da660653000f057e&p1=8)  
-[参考github](https://github.com/FelisCatus/SwitchyOmega/releases)  
PS:记得本地的SwitchyOmega选项

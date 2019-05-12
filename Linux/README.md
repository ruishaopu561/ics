# Linux
这篇md主要记录一些比较零碎的、Linux里经常用到的命令、指令、文件的配置等等。。。
## Contents
- [Ubuntu install](#ubuntu)
- [GitHub](#github)
- [GDB](#gdb)
- [VS Code](#vscode)
- [Windows](#exe)
- [dpkg](#dpkg)
- [ssr](#ssr)
<span id="ubuntu"></span>
## Ubuntu Install
以宏基电脑为例，安装ubuntu18.04系统
### 找不到boot manager
boot manager用f12打开，但f12选项可能被禁用了，需要手动打开。  
具体步骤：
> * 1，宏基快捷键是F12，有的电脑启用了F12，但也有的电脑没有启用F12快捷键，所以我们先要到BIOS中去打开快捷键F12，启用
它。开机进入标志画面后，按F2键进入BIOS。
> * 2，进入BIOS主界面后，在Main项下，找到F12 Boot Menu，其中文意思是“F12启动菜单”，它现在的设置是Disabled(关闭)，
我们要把它打开，先把光标移到这一项上来，然后按Enter键。
> * 3.当按了Enter键以后，会弹出一个小窗口，有两个选项，一个为Disabled(关闭)，另一个为Enabled(打开)，我们要选择
Enabled这一项，然后按Enter键确定，这样就打开了F12快捷键，然后按F10保存并退出。
> * 4.重新启动电脑后，就可按F12快捷键，立即进入启动菜单Boot Manager，在这里有四个选项启动，1硬盘，2光盘，3网卡，4U
盘，如果你想用U盘启动，可以用上下方向键把光标移到第4项(USB HDD)，然后按Enter键，就能立即从U盘启动。
### 分区分配推荐
#### linux分区设置：  
方法一：
```
Swap（相当于电脑内存）：逻辑分区、大小设置为电脑内存大小，2G，4G；

/boot（引导分区）：主分区：大小设置为200M；

/home（用户存储数据用）：逻辑分区，要尽可能大，100G空间可以设置为85G，留10G给主分区即可。

/.（主分区）：主分区，用于存放系统，相当于win7的C盘，10G即可。
```
方法二：
```
/.（主分区）：主分区，只分这一个区，将所有空闲空间（free space）都分给主分区。
```
方法三：
```
Swap（相当于电脑内存）：逻辑分区、大小设置为电脑内存大小，2G，4G；

/.（主分区）：主分区，用于存放系统，相当于win7的C盘，其他剩余空间都分给主分区。
```
#### 具体大小安排参考
```
挂载点/；主分区；安装系统和软件；大小为150G；分区格式为ext4； 

挂载点/home；逻辑分区；相当于“我的文档”；150G大小为硬盘剩下的; 分区格式ext4； 

 swap；逻辑分区；充当虚拟内存；大小等于内存大小（本人8G）；分区格式为swap 

挂载点/boot ；引导分区；逻辑分区； 大小为1G ；分区格式为ext4；
```
#### 未发现wifi适配器
reference里说的很真实了，明明没有发现wifi适配器，还一堆全是联网后的教程，wtm...当然也是可以通过USB共享一下的。  
具体过程如下：  
应该大多数人都是下载ubuntu-16.04-desktop-amd64.iso 这种iso包用u盘安装的嘛，咱们把iso包解压：按照这个路径找到这个文件
```
ubuntu-16.04-desktop-amd64 -> pool -> restricted -> b -> bcmwl -> bcmwl-kernel-source_6.30.223.248+bdcom-0ubuntu8_amd64.deb
```
，这就是无线网卡的驱动安装包了。

把它用命令通过终端安装就行了

先用 cd 命令到这个文件的目录下```sudo dpkg -i``` [文件名]，比如我的是：
```
sudo dpkg -i bcmwl-kernel-source_6.30.223.248+bdcom-0ubuntu8_amd64.deb
```
一般会报个错告诉你少一个依赖，同样按照路径找到如下文件：
```
pool -> main -> d -> dkms -> dkms_2.2.0.3-2ubuntu11.1_all.deb
```
同样cd 到这个文件的路径下用同样的命令安装这个包：
```
sudo dpkg -i dkms_2.2.0.3-2ubuntu11.1_all.deb
```
然后再重新安装一遍那个网卡驱动包：
```
sudo dpkg -i bcmwl-kernel-source_6.30.223.248+bdcom-0ubuntu8_amd64.deb
```
然后还有最后一步，点击 wifi 图标,勾掉启动Wi-Fi,在重启Wi-Fi，你就会神奇的发现可以搜索到 WIFI了。
#### Reference
- [宏碁电脑版本安装ubuntu](https://www.jianshu.com/p/1b66f5c78025)
- [ubuntu16.04分区设置](https://blog.csdn.net/zhangxiangweide/article/details/74779652)
- [Ubuntu 16.04安装手动分区配置](https://blog.csdn.net/qq_27623521/article/details/78836988)
- [Ubuntu离线安装网卡驱动](https://blog.csdn.net/ifmvo/article/details/54023628)
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

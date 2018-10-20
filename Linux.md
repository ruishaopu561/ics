# Linux
这篇md主要记录一些比较零碎的、Linux里经常用到的命令、指令、文件的配置等等。。。
## Contents
- [GDB](#gdb)
- [vscode](#vscode)
- [wechat](#wechat)
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
话不多说，上代码  
Launch.json
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build",
        }
    ]
}
```
tasks.json
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++ ${file} -o ${fileBasenameNoExtension} -g"
        }
    ]
}
```
<span id="wechat"></span>
## 安装electronic-wechat
前提：git,npm  
命令：
```
git clone https://github.com/geeeeeeeeek/electronic-wechat.git 
```
Go into the repository 
```
cd electronic-wechat 
```
Install dependencies and run the app 
```
npm install && npm start
```
编译成app
```
npm run build:linux
```

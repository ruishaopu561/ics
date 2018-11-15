# Linux
这篇md主要记录一些比较零碎的、Linux里经常用到的命令、指令、文件的配置等等。。。
## Contents
- [GitHub](#github)
- [GDB](#gdb)
- [VS Code](#vscode)
- [Windows](#exe)
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
Linux下：  
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
windows下:  
c_cpp_properties.json
```json
{
    "configurations":[
        {
            "name":"Win32",
            "intelliSenseMode": "clang-x64",
            "includePath": [
                "${workspaceFolder}",
                "path..."
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "__GNUC__=7",
                "__cdecl=__attribute__((__cdecl__))"
            ],
            "browse": {
                "path": [
                    "${workspaceFolder}",
                    "path..."
                ],
                "limitSymbolsToIncludedHeaders": true,
                "databaseFilename": ""
            },
            "cStandard": "c11",
            "cppStandard": "c++17",
            "compilerPath": "..."
        }
    ],
    "version": 4
}
```
launch.json
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/${fileBasenameNoExtension}.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "internalConsoleOptions":"neverOpen",
            "MIMode": "gdb",
            "miDebuggerPath": "gdb.exe",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": false
                }
            ],
            "preLaunchTask": "Compile"
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
            "label": "Compile",
            "type": "shell",
            "command": "g++",
            "args": [
                "${file}",
                "-o",
                "${fileNDirname}/${fileBasenameNoExtension}.exe",
                "-g",
                "-Wall",
                "-static-libgcc",
                "-std=c++17"
           ],
           "type":"shell",
           "group":{
               "kind":"build",
               "isDefault": true
           },
           "presentation":{
               "echo":true,
               "reveal":"always",
               "focus":false,
               "panel": "shared",
           }
        }
    ]
}
```
<span id="exe"></span>
## 安装windows程序
[2018年wine QQ最完美解决方案(多Linux发行版通过测试并稳定运行)](https://www.lulinux.com/archives/1319)

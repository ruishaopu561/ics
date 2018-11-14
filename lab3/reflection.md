# Reflections about lab3
## some tools
### Assembly and disassembly
create a file called touch.s and write some assembly codes in it, then:
``` 
gcc -c touch.s
objdump -d touch.o > touch.d
```
Now we get the real assembly codes, with each line's relative destination and their Hex orders.
### run in Linux
```
./filename < <inputfile> ...
```
即用“<”连接文件作为输入，用">"作为输出文件。
### 涉及的gdb命令
```
(gdb) r run的简写，运行被调试的程序。若有断点，则程序暂停在第一个可用断点处。
(gdb) c continue的简写，继续执行被调试程序，直至下一个断点或程序结束。
(gdb) print <指定变量> 显示指定变量的值。
(gdb) break *<代码地址> 设置断点。
(gdb) x/<n/f/u> <addr> examine的简写，查看内存地址中的值。
```
## About This Lab
这个Lab是利用代码的漏洞进行进攻，所有的操作均在栈上完成。  
共分为5个phase，前三个是利用注入的代码，即修改栈上原有的返回值之后，将返回值改为我注入代码的开始位置，运行完注入代码后
返回应该返回的地址，在这之中可以达到修改一定值的作用，比如函数的参数、内存中的值等。  
后两个是利用函数中本来就有的代码进行攻击。因为所有的操作都会被转化成16进制表示的二进制指令，在较长指令中可能会蕴含有我们
需要的攻击指令，在本lab中，每个函数都比较短，所以可以做到在每个函数中执行一段代码后就返回，然后再去执行下一个我们需要的操
作。比较值得思考的是，这里我们会先让缓冲区溢出，然后将原有的存返回值的区域改存第一个攻击函数的地址，因为它会自动ret，所以
栈上直接紧随其后写第二个攻击函数的地址，于是一步步靠近我们要的结果。
### 对栈和内存、函数关系的理解
首先，栈是建立在内存里的，%rsp只是负责管理和记录栈，栈本身并不是一个独立的存在。  
在实际上运行代码的过程中，所有的代码都是按二进制写在一起的，不出现跳转时，就会按照顺序一一运行。所以栈上面也可以堆指令代码，
并且跳转过来也可以正常运行，这个时候，栈就失去了栈的特点，而是一个非常普通的内存的角色。  
更多情况下，栈还是栈，它里面放的是函数地址，value或value地址。尤其是函数地址时，就要考虑最一般的退栈、压栈在栈上的运行，这
也就是第2种攻击方式的由来————堆的只是地址，虽然不一定是函数开头，但完全没有区别，在结束时同样会退栈会到%rsp，然后继续运行
下一个指令。
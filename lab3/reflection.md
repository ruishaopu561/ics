# Reflections about lab3
## some tools
### Assembly and disassembly
create a file called touch.s and write some assembly codes in it, then:
``` 
gcc -c touch.s
objdump -d touch.o > touch.d
```
Now we get the real assembly codes, with each line's relative destination and their Hex orders.

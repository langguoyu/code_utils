使用LD_PRELOAD加载malloc free wrapper库

## 1.测试程序编译运行
compile & run:
```bash
    $gcc -g -O0 -o test test.c
    $./test
```

output:
```log
main func addr:0x55582f4ff1a9
start malloc
start free
```
## 2.编写实现mymalloc，编译运行
compile & run:
```bash
    $gcc -g -O0 -fPIC -shared mymalloc.c -o libmymalloc.so -ldl
    $LD_PRELOAD=./libmymalloc.so ./test
```

output:
```log
    malloc call 1024 from 0x7f4da2cead04
    free call 0x560e64ead2a0 from 0x7f4da2cfae75
    main func addr:0x560e6462f1a9
    start malloc
    malloc call 10 from 0x560e6462f1e3
    malloc call 10 from 0x560e6462f1f1
    start free
    free call 0x560e64eadac0 from 0x560e6462f20d
```

## 3.进一步分析调用栈
 malloc call 10 from 0x560e6462f1e3，其中0x560e6462f1e3代表哪个函数?

 我们先看test中main函数地址是：
```bash
    $nm test | grep main            
    00000000000011a9 T main
```

我们先看main函数运行时地址：main func addr:0x560e6462f1a9
那么编译地址到运行地址的转换则是：0x560e6462f1a9-00000000000011a9=0x560E6462E000

那么0x560e6462f1e3的调用栈是什么呢？
0x560e6462f1e3-0x560E6462E000=0x11E3
```bash
    $addr2line -e test -afsp 0x11E3
    0x00000000000011e3: main at test.c:7
```
可以看出0x560e6462f1e3的调用栈是：main at test.c:7

## 4.reference
    https://www.jianshu.com/p/bd819ce58c60
    https://www.cnblogs.com/net66/p/5609026.html
    https://stackoverflow.com/questions/42336616/fixing-the-recursive-call-to-malloc-with-ld-preload

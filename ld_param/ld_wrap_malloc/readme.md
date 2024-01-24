## ld --wrap参数
```
https://docs.oracle.com/cd/E56344_01/html/E54075/ld-1.html

–zwrap=symbol
–wrap= symbol
–-wrap= symbol
重命名对 symbol 的未定义引用以允许将包装代码链接到输出目标文件而不必修改源代码。如果指定 –z wrap，将修改对 symbol 的所有未定义引用以引用 __wrap_symbol，并修改对 __real_symbol 的所有引用以引用 symbol。您应提供包含 __wrap_symbol 函数的目标文件。此包装函数可以调用 __real_symbol 以引用要包装的实际函数。

以下是适用于 malloc(3C) 函数的包装示例。

void *
__wrap_malloc(size_t c)
{
        (void) printf("malloc called with %zu\n", c);
        return (__real_malloc(c));
}
如果使用 –z wrap=malloc 将其他代码链接到此文件以编译所有目标文件，则对 malloc 的所有调用将改为调用函数 __wrap_malloc。对 __real_malloc 的调用将调用 malloc 实函数。

应在单独的源文件中维护实函数和包装函数。否则，编译器或汇编程序可能会解析调用而不是将该操作留给链接编辑器执行，并且可能阻止进行包装。

```

## 编译&测试
```
mkdir build
cd build
cmake ..
make
./test
```

## reference
```
https://blog.csdn.net/fengbingchun/article/details/82947673

https://blog.csdn.net/hejinjing_tom_com/article/details/124084708
```
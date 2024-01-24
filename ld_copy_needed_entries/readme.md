## ld_copy_needed_entries参数
```
   --copy-dt-needed-entries
   --no-copy-dt-needed-entries
       This option affects the treatment of dynamic libraries referred to by DT_NEEDED tags inside ELF dynamic libraries mentioned on the command line.  Normally the linker won't add a DT_NEEDED
       tag to the output binary for each library mentioned in a DT_NEEDED tag in an input dynamic library.  With --copy-dt-needed-entries specified on the command line however any dynamic
       libraries that follow it will have their DT_NEEDED entries added.  The default behaviour can be restored with --no-copy-dt-needed-entries.

       This option also has an effect on the resolution of symbols in dynamic libraries.  With --copy-dt-needed-entries dynamic libraries mentioned on the command line will be recursively
       searched, following their DT_NEEDED tags to other libraries, in order to resolve symbols required by the output binary.  With the default setting however the searching of dynamic
       libraries that follow it will stop with the dynamic library itself.  No DT_NEEDED links will be traversed to resolve symbols.
```
大概意思就是，跟在--no-copy-dt-needed-entries它后面的库都不会遍历其依赖项，使用--copy-dt-needed-entries则相反。也就是使用下面的指令来编译mian.cpp就可以避免该问题了。


## 测试
```
gcc libC.cpp -fPIC -shared -o libC.so
gcc libB.cpp -fPIC -shared -o libB.so -Wl,-rpath=./ -L./ -lC
gcc libA.cpp -fPIC -shared -o libA.so -Wl,-rpath=./ -L./ -lB

ldd *.so
```
得到的结果如下：
```
libA.so:
        linux-vdso.so.1 (0x00007ffe15744000)
        libB.so => ./libB.so (0x00007fcb56bf6000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fcb569c9000)
        libC.so => ./libC.so (0x00007fcb569c4000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fcb56c02000)
libB.so:
        linux-vdso.so.1 (0x00007ffdb2326000)
        libC.so => ./libC.so (0x00007f2eff935000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f2eff708000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f2eff941000)
libC.so:
        linux-vdso.so.1 (0x00007fff79716000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f414c37a000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f414c5ae000)
```

```
gcc main.cpp -L./ -lA
/usr/bin/ld: /tmp/ccJFI6Tb.o: undefined reference to symbol '_Z5funB2v'
/usr/bin/ld: .//libB.so: error adding symbols: DSO missing from command line
collect2: error: ld returned 1 exit status
➜  ld_copy_needed_entries git:(master) ✗ c++filt _Z5funB2v                              
funB2()
```

解决方法：
```
gcc main.cpp -L./ -Wl,--copy-dt-needed-entries -lA
```

## reference
https://cloud.tencent.com/developer/article/2124829
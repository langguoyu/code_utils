reference
    https://www.jianshu.com/p/bd819ce58c60
```bash
    gcc -fPIC -shared -o mwrap.so mwrap.c -ldl
    g++ -o test_wrap test_wrap.cpp
    LD_PRELOAD=./mwrap.so ./test_wrap
```
output:
```log
mwrap.so: successfully wrapped!
using wrapped malloc: size = 72704, pointer = 0x55da369372a0
using wrapped malloc: size = 16, pointer = 0x55da36948eb0
using wrapped malloc: size = 1024, pointer = 0x55da36948ed0
```


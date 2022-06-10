
[toc]

## 编译动态库
```
➜  dlopen_dlsym git:(master) ✗ gcc -g -O0 -shared -fPIC ./hello.c -o libhello.so
➜  dlopen_dlsym git:(master) ✗ file libhello.so   
libhello.so: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[sha1]=c9a4d55e2074c81c8fe4cbe9255dc16bd79520a9, with debug_info, not stripped
```

## 编译主程序
```
gcc -g -O0 ./test.c -o test -ldl
```

## 测试
```
➜  dlopen_dlsym git:(master) ✗ ./test                          
Hello World !
```

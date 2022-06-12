
[toc]
## C函数
### 编译动态库
```
➜  c git:(master) ✗ gcc -g -O0 -shared -fPIC ./hello.c -o libhello.so
➜  c git:(master) ✗ file libhello.so   
libhello.so: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[sha1]=c9a4d55e2074c81c8fe4cbe9255dc16bd79520a9, with debug_info, not stripped
```

### 编译主程序
```
gcc -g -O0 ./test.c -o test -ldl
```

### 测试
```
➜  c git:(master) ✗ ./test                          
Hello World !
```
## C++类
### 编译动态库
```
➜  c++ git:(master) ✗ g++ -g -O0 -shared -fPIC ./triangle.cpp -o triangle.so
➜  c++ git:(master) ✗ file triangle.so   
triangle.so: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[sha1]=cccc02fa81f2ffe2327269a62f5ab553b6f0e86f, with debug_info, not stripped
```

### 编译主程序
```
g++ -g -O0 ./main.cpp -o main -ldl
```

### 测试
```
➜  c++ git:(master) ✗ ./test                          
The area is: 42.4352
```

## reference

[使用dlopen和dlsym来使用C++中的类](https://www.cnblogs.com/soniclq/archive/2012/04/24/2467649.html)
[Dynamic Class Loading for C++ on Linux](https://www.linuxjournal.com/article/3687)

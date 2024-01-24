## gc-sections
　　当项目中使用了很多静态库文件(.a)，会导致编译生成的文件非常大。其实，这些静态库中的函数，并非所有都有调用，项目只使用了其中一小部分。使用"-gc-sections"参数，将不链接未用的函数，减少文件的大小。


## 编译
```
mkdir build
cd build
cmake ..
make
```
## 测试
### 静态库测试
```
✗ objdump -x libtest_1.a| grep test 
In archive libtest_1.a:
test_1.cpp.o:     file format elf64-x86-64
rw-r--r-- 0/0   2128 Jan  1 08:00 1970 test_1.cpp.o
  4 .text._Z6test_av 00000017  0000000000000000  0000000000000000  0000004e  2**0
  5 .text._Z6test_bv 00000017  0000000000000000  0000000000000000  00000065  2**0
0000000000000000 l    df *ABS*  0000000000000000 test_1.cpp
0000000000000000 l    d  .text._Z6test_av       0000000000000000 .text._Z6test_av
0000000000000000 l    d  .text._Z6test_bv       0000000000000000 .text._Z6test_bv
0000000000000000 g     F .text._Z6test_av       0000000000000017 _Z6test_av
0000000000000000 g     F .text._Z6test_bv       0000000000000017 _Z6test_bv
RELOCATION RECORDS FOR [.text._Z6test_av]:
RELOCATION RECORDS FOR [.text._Z6test_bv]:
0000000000000020 R_X86_64_PC32     .text._Z6test_av
0000000000000040 R_X86_64_PC32     .text._Z6test_bv

➜  build git:(master) ✗ objdump -x libtest_1_disable.a| grep test 
In archive libtest_1_disable.a:
test_1.cpp.o:     file format elf64-x86-64
rw-r--r-- 0/0   1848 Jan  1 08:00 1970 test_1.cpp.o
0000000000000000 l    df *ABS*  0000000000000000 test_1.cpp
0000000000000000 g     F .text  0000000000000017 _Z6test_av
0000000000000017 g     F .text  0000000000000017 _Z6test_bv
```
启用ffunction-sections -fdata-sections后多了很多段

### 动态库比对
```
➜  build git:(master) ✗ objdump -x libtest_2.so| grep test 
libtest_2.so:     file format elf64-x86-64
libtest_2.so
  SONAME               libtest_2.so
0000000000000000 l    df *ABS*  0000000000000000              test_2.cpp
0000000000000000 l    df *ABS*  0000000000000000              test_1.cpp
0000000000001158 l     F .text  0000000000000017              _Z6test_av
0000000000001141 g     F .text  0000000000000017              _Z8test_2_1v
0000000000001119 g     F .text  0000000000000028              _Z6test_2v

➜  build git:(master) ✗ objdump -x libtest_2_disable.so| grep test 
libtest_2_disable.so:     file format elf64-x86-64
libtest_2_disable.so
  SONAME               libtest_2_disable.so
0000000000000000 l    df *ABS*  0000000000000000              test_2.cpp
0000000000000000 l    df *ABS*  0000000000000000              test_1.cpp
0000000000001161 g     F .text  0000000000000017              _Z8test_2_1v
0000000000001178 g     F .text  0000000000000017              _Z6test_av
000000000000118f g     F .text  0000000000000017              _Z6test_bv
0000000000001139 g     F .text  0000000000000028              _Z6test_2v
```
使用gc-sections后减少了test_b符号

### bin文件比对
```
➜  build git:(master) ✗ objdump -x test_3| grep test 
test_3:     file format elf64-x86-64
test_3
0000000000000000 l    df *ABS*  0000000000000000              test_3.cpp
0000000000000000 l    df *ABS*  0000000000000000              test_1.cpp
000000000000115d l     F .text  0000000000000017              _Z6test_av

➜  build git:(master) ✗ objdump -x test_3_disable| grep test 
test_3_disable:     file format elf64-x86-64
test_3_disable
0000000000000000 l    df *ABS*  0000000000000000              test_3.cpp
0000000000000000 l    df *ABS*  0000000000000000              test_1.cpp
000000000000115d g     F .text  0000000000000017              _Z6test_av
0000000000001174 g     F .text  0000000000000017              _Z6test_bv
```
使用gc-sections后减少了test_b符号

## reference
https://www.cnblogs.com/dongc/p/17394115.html
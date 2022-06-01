breakpad简单使用
[toc]

## breakpad简介
breakpad是google开发的一个跨平台C/C++ dump捕获开源库，崩溃文件使用微软的minidump格式存储，也支持发送这个dump文件到你的服务器，breakpad可以在程序崩溃时触发dump写入操作，也可以在没有触发dump时主动写dump文件。breakpad支持windows、linux、macos、android、ios等。目前已有Google Chrome, Firefox, Google Picasa, Camino, Google Earth等项目使用。

## 源码下载

```
git clone git@github.com:google/breakpad.git
```

## 源码编译

```
cd breakpad
./configure && make -j`nproc`
sudo make install
```
### 编译遇到的问题
1. third_party/lss/linux_syscall_support.h

```
fatal error: third_party/lss/linux_syscall_support.h: No such file or directory
```

解决方法
```
git clone git@github.com:linux-on-ibm-z/linux-syscall-support.git src/third_party/lss
```
2. linux_syscall_support.h
```
./src/third_party/lss/linux_syscall_support.h:2379:75: error: listing the stack pointer register ‘rsp’ in a clobber list is deprecated [-Werror=deprecated]
 2379 |                              : "rsp", "memory", "r8", "r10", "r11", "rcx");
```
解决方法：
```
linux_syscall_support.h删除"rsp"即：
+++ b/linux_syscall_support.h
@@ -2376,7 +2376,7 @@ struct kernel_statfs {
                                "d"(LSS_SYSCALL_ARG(parent_tidptr)),
                                "r"(LSS_SYSCALL_ARG(newtls)),
                                "r"(LSS_SYSCALL_ARG(child_tidptr))
-                             : "rsp", "memory", "r8", "r10", "r11", "rcx");
+                             : "memory", "r8", "r10", "r11", "rcx");
       }
       LSS_RETURN(int, __res);
     }
```

## In-Process测试程序：

test.cc源码
```
#include "client/linux/handler/exception_handler.h"

static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
void* context, bool succeeded) {
  printf("Dump path: %s\n", descriptor.path());
  return succeeded;
}

void crash() { volatile int* a = (int*)(NULL); *a = 1; }

int main(int argc, char* argv[]) {
  google_breakpad::MinidumpDescriptor descriptor("/tmp");
  google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);
  crash();
  return 0;
}
```
### 测试程序编译
```
g++ -g -o test.cc test -I/usr/local/include/breakpad -lbreakpad_client -lpthread
```

### 生成sym文件
```
$ dump_syms ./test > test.sym
$ head -n1 test.sym
MODULE Linux x86_64 4665886B3016F300CC8655AF48A11AFE0 test
$ mkdir -p symbols/test/4665886B3016F300CC8655AF48A11AFE0 
$ mv test.sym symbols/test/4665886B3016F300CC8655AF48A11AFE0
```
### 产生minidump文件
```
$ ./test 
Dump path: /tmp/6164c367-f9b5-4bd4-20614abb-22b78a15.dmp
[1]    226680 segmentation fault (core dumped)  ./test
```
### 分析minidump文件

```
$ minidump_stackwalk /tmp/6164c367-f9b5-4bd4-20614abb-22b78a15.dmp ./symbols 
2022-06-01 18:20:55: minidump.cc:5093: INFO: Minidump opened minidump /tmp/6164c367-f9b5-4bd4-20614abb-22b78a15.dmp
2022-06-01 18:20:55: minidump.cc:5223: INFO: Minidump not byte-swapping minidump
2022-06-01 18:20:55: minidump.cc:5875: INFO: GetStream: type 15 not present
2022-06-01 18:20:55: minidump.cc:5875: INFO: GetStream: type 1197932545 not present
2022-06-01 18:20:55: minidump.cc:5875: INFO: GetStream: type 1197932546 not present
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /home/guoyu/work/opensource/breakpad/test
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libm.so.6
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libc.so.6
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libgcc_s.so.1
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /usr/lib/x86_64-linux-gnu/libstdc++.so.6
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libpthread.so.0
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for linux-gate.so
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /home/guoyu/work/opensource/breakpad/test
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libm.so.6
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libc.so.6
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libgcc_s.so.1
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /usr/lib/x86_64-linux-gnu/libstdc++.so.6
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/libpthread.so.0
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
2022-06-01 18:20:55: minidump.cc:2267: INFO: MinidumpModule could not determine version for linux-gate.so
2022-06-01 18:20:55: minidump.cc:5875: INFO: GetStream: type 14 not present
2022-06-01 18:20:55: minidump_processor.cc:174: INFO: Found 2 memory regions.
2022-06-01 18:20:55: minidump_processor.cc:184: INFO: Minidump /tmp/6164c367-f9b5-4bd4-20614abb-22b78a15.dmp has CPU info, OS info, no Breakpad info, exception, module list, thread list, no dump thread, requesting thread, and no process create time
2022-06-01 18:20:55: minidump_processor.cc:223: INFO: Looking at thread /tmp/6164c367-f9b5-4bd4-20614abb-22b78a15.dmp:0/1 id 0x37578
2022-06-01 18:20:55: minidump.cc:483: INFO: MinidumpContext: looks like AMD64 context
2022-06-01 18:20:55: minidump.cc:483: INFO: MinidumpContext: looks like AMD64 context
2022-06-01 18:20:55: source_line_resolver_base.cc:241: INFO: Loading symbols for module /home/guoyu/work/opensource/breakpad/test from memory buffer, size: 244302
2022-06-01 18:20:55: simple_symbol_supplier.cc:196: INFO: No symbol file at ./symbols/libc.so.6/B4E6781872757C0C51969E69AB2D276F0/libc.so.6.sym
2022-06-01 18:20:55: stackwalker.cc:103: INFO: Couldn't load symbols for: /lib/x86_64-linux-gnu/libc.so.6|B4E6781872757C0C51969E69AB2D276F0
2022-06-01 18:20:55: minidump.cc:1391: INFO: MinidumpMemoryRegion request out of range: 0x8+8/0x7ffd3367b000+0x3000
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffa0594ab7f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367c067
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x100011bff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xd2f00a1c903d9c68
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367c05f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x2d0a6cd3ef3d9c68
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x2d0400ecd0539c68
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x0
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367c067
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367c077
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffa05bc618f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367c05f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367c057
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x1b
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x0
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d0ac
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0xffffffffffffffff
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d0b3
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d0d4
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d0e8
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d0f3
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d106
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d11b
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d123
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d134
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d148
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d15f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d19e
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d1ba
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d1da
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d1f2
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d202
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d238
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d25d
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d26f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d27d
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d291
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d2bf
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d2d6
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d2ed
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d2fc
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d328
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d335
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d3b4
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d440
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d46f
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d486
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d4a5
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d4bc
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d4c7
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d4d8
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d4f1
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d50b
2022-06-01 18:20:55: basic_code_modules.cc:111: INFO: No module at 0x7ffd3367d534
2022-06-01 18:20:55: minidump_processor.cc:348: INFO: Processed /tmp/6164c367-f9b5-4bd4-20614abb-22b78a15.dmp
Operating system: Linux
                  0.0.0 Linux 5.4.0-113-generic #127-Ubuntu SMP Wed May 18 14:30:56 UTC 2022 x86_64
CPU: amd64
     family 6 model 142 stepping 12
     1 CPU

GPU: UNKNOWN

Crash reason:  SIGSEGV /SEGV_MAPERR
Crash address: 0x0
Process uptime: not available

Thread 0 (crashed)
 0  test!crash() [test.cc : 9 + 0x4]
    rax = 0x0000000000000000   rdx = 0x0000000000000000
    rcx = 0x000055e0f7f5af90   rbx = 0x000055e0f6895620
    rsi = 0x0000000000000000   rdi = 0x000055e0f689d6c0
    rbp = 0x00007ffd3367bdb0   rsp = 0x00007ffd3367bdb0
     r8 = 0x0000000000000000    r9 = 0x000055e0f7f5af88
    r10 = 0x0000000000000008   r11 = 0x00007ffa0594abe0
    r12 = 0x000055e0f6882e10   r13 = 0x00007ffd3367c060
    r14 = 0x0000000000000000   r15 = 0x0000000000000000
    rip = 0x000055e0f6882f4c
    Found by: given as instruction pointer in context
 1  test!main [test.cc : 14 + 0x5]
    rbx = 0x000055e0f6895620   rbp = 0x00007ffd3367bf70
    rsp = 0x00007ffd3367bdc0   r12 = 0x000055e0f6882e10
    r13 = 0x00007ffd3367c060   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x000055e0f6883021
    Found by: call frame info
 2  libc.so.6 + 0x24083
    rbx = 0x000055e0f6895620   rbp = 0x0000000000000000
    rsp = 0x00007ffd3367bf80   r12 = 0x000055e0f6882e10
    r13 = 0x00007ffd3367c060   r14 = 0x0000000000000000
    r15 = 0x0000000000000000   rip = 0x00007ffa05782083
    Found by: call frame info
 3  test!crash() [test.cc : 9 + 0x3]
    rsp = 0x00007ffd3367bfa0   rip = 0x000055e0f6882f55
    Found by: stack scanning
 4  test + 0x15620
    rbp = 0x000055e0f6882f55   rsp = 0x00007ffd3367bfa8
    rip = 0x000055e0f6895620
    Found by: call frame info
 5  test!_start + 0x2e
    rsp = 0x00007ffd3367c050   rip = 0x000055e0f6882e3e
    Found by: stack scanning
 6  0x7ffd3367c058
    rsp = 0x00007ffd3367c058   rip = 0x00007ffd3367c058
    Found by: call frame info

Loaded modules:
0x55e0f6880000 - 0x55e0f6895fff  test  ???  (main)
0x7ffa0560f000 - 0x7ffa056c2fff  libm.so.6  ???
0x7ffa0575e000 - 0x7ffa058f7fff  libc.so.6  ???  (WARNING: No symbols, libc.so.6, B4E6781872757C0C51969E69AB2D276F0)
0x7ffa05950000 - 0x7ffa05964fff  libgcc_s.so.1  ???
0x7ffa0596b000 - 0x7ffa05af1fff  libstdc++.so.6  ???
0x7ffa05b4d000 - 0x7ffa05b63fff  libpthread.so.0  ???
0x7ffa05b97000 - 0x7ffa05bbafff  ld-linux-x86-64.so.2  ???
0x7ffd3378a000 - 0x7ffd3378afff  linux-gate.so  ???
2022-06-01 18:20:55: minidump.cc:5065: INFO: Minidump closing minidump
```

## reference
[google-breakpad - 使用篇](https://juejin.cn/post/6899070041074073614)
[Google Breakpad 源码解析](https://juejin.cn/post/6991473315672129573)
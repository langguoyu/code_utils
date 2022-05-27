### compile & run
```bash
$ g++ -g -O0 -fno-omit-frame-pointer ./test.cc -o test -lpthread
$./test                       
before lock_guard
```
## 一、使用gdb排查
重新开启一个终端，打开gdb
```bash
$ sudo gdb attach $(pidof test)
GNU gdb (Ubuntu 9.2-0ubuntu1~20.04.1) 9.2
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
attach: No such file or directory.
Attaching to process 244184
[New LWP 244185]
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
__pthread_clockjoin_ex (threadid=140350036600576, thread_return=0x0, clockid=<optimized out>, 
    abstime=<optimized out>, block=<optimized out>) at pthread_join_common.c:145
145     pthread_join_common.c: No such file or directory.
(gdb) info threads
  Id   Target Id                                 Frame 
* 1    Thread 0x7fa5ca137740 (LWP 244184) "test" __pthread_clockjoin_ex (threadid=140350036600576, 
    thread_return=0x0, clockid=<optimized out>, abstime=<optimized out>, block=<optimized out>)
    at pthread_join_common.c:145
  2    Thread 0x7fa5ca136700 (LWP 244185) "test" __lll_lock_wait (futex=futex@entry=0x558701b87160 <m>, 
    private=0) at lowlevellock.c:52
(gdb) t 2 #1 切换到线程2
[Switching to thread 2 (Thread 0x7fa5ca136700 (LWP 244185))]
#0  __lll_lock_wait (futex=futex@entry=0x558701b87160 <m>, private=0) at lowlevellock.c:52
52      lowlevellock.c: No such file or directory.
(gdb) bt #2 查看栈追溯
#0  __lll_lock_wait (futex=futex@entry=0x558701b87160 <m>, private=0) at lowlevellock.c:52
#1  0x00007fa5ca6850a3 in __GI___pthread_mutex_lock (mutex=0x558701b87160 <m>) #3 发现在等锁m
    at ../nptl/pthread_mutex_lock.c:80
#2  0x0000558701b84547 in __gthread_mutex_lock (__mutex=0x558701b87160 <m>)
    at /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h:749
#3  0x0000558701b8459c in std::mutex::lock (this=0x558701b87160 <m>)
    at /usr/include/c++/9/bits/std_mutex.h:100
#4  0x0000558701b846ba in std::lock_guard<std::mutex>::lock_guard (this=0x7fa5ca135df0, __m=...)
    at /usr/include/c++/9/bits/std_mutex.h:159
#5  0x0000558701b84392 in test () at ./test.cc:13
#6  0x0000558701b84f18 in std::__invoke_impl<void, void (*)()> (
    __f=@0x558701d94eb8: 0x558701b84328 <test()>) at /usr/include/c++/9/bits/invoke.h:60
#7  0x0000558701b84eb0 in std::__invoke<void (*)()> (__fn=@0x558701d94eb8: 0x558701b84328 <test()>)
    at /usr/include/c++/9/bits/invoke.h:95
#8  0x0000558701b84e42 in std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul> (
    this=0x558701d94eb8) at /usr/include/c++/9/thread:244
#9  0x0000558701b84dff in std::thread::_Invoker<std::tuple<void (*)()> >::operator() (
    this=0x558701d94eb8) at /usr/include/c++/9/thread:251
#10 0x0000558701b84dd0 in std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run (this=0x558701d94eb0) at /usr/include/c++/9/thread:195
#11 0x00007fa5ca56ede4 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#12 0x00007fa5ca682609 in start_thread (arg=<optimized out>) at pthread_create.c:477
#13 0x00007fa5ca3aa133 in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
(gdb) c #4 运行一下再看
Continuing.
^C
Thread 1 "test" received signal SIGINT, Interrupt.
[Switching to Thread 0x7fa5ca137740 (LWP 244184)]
__pthread_clockjoin_ex (threadid=140350036600576, thread_return=0x0, clockid=<optimized out>, 
    abstime=<optimized out>, block=<optimized out>) at pthread_join_common.c:145
145     pthread_join_common.c: No such file or directory.
(gdb) info threads
  Id   Target Id                                 Frame 
* 1    Thread 0x7fa5ca137740 (LWP 244184) "test" __pthread_clockjoin_ex (threadid=140350036600576, 
    thread_return=0x0, clockid=<optimized out>, abstime=<optimized out>, block=<optimized out>)
    at pthread_join_common.c:145
  2    Thread 0x7fa5ca136700 (LWP 244185) "test" __lll_lock_wait (futex=futex@entry=0x558701b87160 <m>, 
    private=0) at lowlevellock.c:52
(gdb) t 2
[Switching to thread 2 (Thread 0x7fa5ca136700 (LWP 244185))]
#0  __lll_lock_wait (futex=futex@entry=0x558701b87160 <m>, private=0) at lowlevellock.c:52
52      lowlevellock.c: No such file or directory.
(gdb) bt
#0  __lll_lock_wait (futex=futex@entry=0x558701b87160 <m>, private=0) at lowlevellock.c:52
#1  0x00007fa5ca6850a3 in __GI___pthread_mutex_lock (mutex=0x558701b87160 <m>) #5 发现仍然在等锁m
    at ../nptl/pthread_mutex_lock.c:80
#2  0x0000558701b84547 in __gthread_mutex_lock (__mutex=0x558701b87160 <m>)
    at /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h:749
#3  0x0000558701b8459c in std::mutex::lock (this=0x558701b87160 <m>)
    at /usr/include/c++/9/bits/std_mutex.h:100
#4  0x0000558701b846ba in std::lock_guard<std::mutex>::lock_guard (this=0x7fa5ca135df0, __m=...)
    at /usr/include/c++/9/bits/std_mutex.h:159
#5  0x0000558701b84392 in test () at ./test.cc:13
#6  0x0000558701b84f18 in std::__invoke_impl<void, void (*)()> (
    __f=@0x558701d94eb8: 0x558701b84328 <test()>) at /usr/include/c++/9/bits/invoke.h:60
#7  0x0000558701b84eb0 in std::__invoke<void (*)()> (__fn=@0x558701d94eb8: 0x558701b84328 <test()>)
    at /usr/include/c++/9/bits/invoke.h:95
#8  0x0000558701b84e42 in std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul> (
    this=0x558701d94eb8) at /usr/include/c++/9/thread:244
#9  0x0000558701b84dff in std::thread::_Invoker<std::tuple<void (*)()> >::operator() (
    this=0x558701d94eb8) at /usr/include/c++/9/thread:251
#10 0x0000558701b84dd0 in std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run (this=0x558701d94eb0) at /usr/include/c++/9/thread:195
#11 0x00007fa5ca56ede4 in ?? () from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
#12 0x00007fa5ca682609 in start_thread (arg=<optimized out>) at pthread_create.c:477
#13 0x00007fa5ca3aa133 in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
(gdb) 
```
从上述gdb #1~#5步中可以看出死锁在m。

## 二.使用mutextrace 排查
ubuntu上安装mutextrace

```bash
$sudo apt-get install mutextrace
```
测试


```bash
➜  mutex_dead_lock git:(master) ✗ mutextrace ./test
[1] started
before lock_guard
[1] mutex_lock(1)
[1] mutex_lock(1) <waiting for thread 1> ...
```
### 2.1mutextrace原理
mutextrace源码: https://github.com/GyrosGeier/mutextrace
源码比较简单，简略分析如下：
```
int main(int argc, char **argv, char **envp)
{
    unsigned int const max_args = 512;
    unsigned int const max_envs = 512;
    unsigned int const max_preload = 512;

    char *new_argv[max_args];
    char *new_envp[max_envs];
    char new_preload[max_preload];

    char *path = 0;

    char **i, **o;
    char *ii, *oo;

    char added_preloadlib = 0;

    if(argc == 1)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    i = argv + 1;
    o = new_argv;

    for(; argc; --argc, ++i, ++o)
        *o = *i;
    *o = 0;

    i = envp;
    o = new_envp;

    for(; *i; ++i, ++o)
    {
        if(strncmp(*i, "LD_PRELOAD=", 11))
        {
            *o = *i;
            if(!strncmp(*i, "PATH=", 5))
                path = *i + 5;
        }
        else
        {
            ii = *i;
            oo = new_preload;

            for(; *ii; ++ii, ++oo)
                *oo = *ii;

            *oo++ = ' ';
            strcpy(oo, PRELOADLIB);
            *o = new_preload;

            added_preloadlib = 1;
        }
    }

    if(!added_preloadlib)
    {
        strcpy(new_preload, "LD_PRELOAD=" PRELOADLIB); //#1 设置LD_PRELOAD，其中-DPRELOADLIB=\"$(pkglibdir)/mutextrace.so\"
        *o++ = new_preload;
    }

    *o = 0;

    if(!path || strchr(argv[1], '/'))
    {
        execve(argv[1], new_argv, new_envp);
        fprintf(stderr, "Failed to run %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    else
    {
        ii = path;

        size_t cmdlen = strlen(argv[1]) + 2;   // plus slash and NUL

        size_t pathbuf_size = 1024;
        char *pathbuf = malloc(pathbuf_size);
        if(!pathbuf)
        {
                perror("malloc()");
                return 1;
        }

        while(*ii)
        {
            oo = pathbuf;
            for(; *ii && *ii != ':'; ++ii, ++oo)
            {
                if(oo == pathbuf + pathbuf_size - cmdlen)
                {
                    char *old_pathbuf = pathbuf;
                    pathbuf_size <<= 1;
                    pathbuf = realloc(pathbuf, pathbuf_size);
                    if(!pathbuf)
                    {
                            perror("realloc()");
                            return 1;
                    }
                    oo = (oo - old_pathbuf) + pathbuf;
                }
                *oo = *ii;
            }

            if(oo == pathbuf)
            {
                fprintf(stderr, "Empty element in PATH\n");
                free(pathbuf);
                return 1;
            }
            
            *oo++ = '/';

            strcpy(oo, argv[1]);
            if(!access(pathbuf, X_OK))
            {
                execve(pathbuf, new_argv, new_envp);
                fprintf(stderr, "Failed to run %s: %s\n", pathbuf, strerror(errno));
                free(pathbuf);
                return 1;
            }

            ++ii;
        }

        free(pathbuf);
    }

    fprintf(stderr, "Could not find %s in PATH\n", argv[1]);
    return 1;
}


void init(void)
{
    static int initialized = 0;
    if(initialized)
        return;

    initialized = 1;

    struct link_map *map = _r_debug.r_map;

    if(!map)
        abort();

    for(; map; map = map->l_next)
    {
        if(strstr(map->l_name, "libpthread.so"))
        {
            ElfW(Dyn) *dyn;
            ElfW(Sym) *symtab = 0;
            char const *strtab = 0;
            unsigned int nsymbols = 0;

            for(dyn = map->l_ld; dyn->d_tag != DT_NULL; ++dyn)
            {
                if(dyn->d_tag == DT_SYMTAB)
                    symtab = (ElfW(Sym) *)OFFSET(dyn->d_un.d_ptr);
                else if(dyn->d_tag == DT_STRTAB)
                    strtab = (char const *)OFFSET(dyn->d_un.d_ptr);
                else if(dyn->d_tag == DT_HASH)
                    nsymbols = ((unsigned int *)OFFSET(dyn->d_un.d_ptr))[1];
            }

            if(!symtab || !strtab || !nsymbols)
                abort();

            for(; nsymbols; ++symtab, --nsymbols)
            {
                char const *name = strtab + symtab->st_name;
                void *value = (void *)symtab->st_value + map->l_addr;
                if(!strcmp(name, "pthread_create"))
                    real_create = value;
                else if(!strcmp(name, "pthread_mutex_init"))
                    real_mutex_init = value;
                else if(!strcmp(name, "pthread_mutex_destroy"))
                    real_mutex_destroy = value;
                else if(!strcmp(name, "pthread_mutex_lock"))
                    real_mutex_lock = value;                    //#2 设置real_mutex_lock为libpthread.so中的pthread_mutex_lock
                else if(!strcmp(name, "pthread_mutex_unlock"))
                    real_mutex_unlock = value;
                else if(!strcmp(name, "pthread_cond_init"))
                    real_cond_init = value;
                else if(!strcmp(name, "pthread_cond_wait"))
                    real_cond_wait = value;
                else if(!strcmp(name, "pthread_cond_timedwait"))
                    real_cond_timedwait = value;
                else if(!strcmp(name, "pthread_cond_signal"))
                    real_cond_signal = value;
            }
        }
    }

    return;
}


int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    init();

    struct thread *t = find_thread(pthread_self());
    struct mutex *n = find_mutex(mutex);

    real_mutex_lock(&n->lock);                      //#3 调用real_mutex_lock
    struct thread *owner = n->owner;
    if(owner)
    {
        fprintf(stderr, "[%u] mutex_lock(%u) <waiting for thread %u> ...\n", t->num, n->num, owner->num);
        struct timeval starttime;
        gettimeofday(&starttime, 0);
        struct timespec timeout = { starttime.tv_sec, starttime.tv_usec * 1000 };
        timeout.tv_sec += 5;
        while(real_cond_timedwait(&n->cond, &n->lock, &timeout) == ETIMEDOUT)
        {
                fprintf(stderr, "[%u] ... mutex_lock(%u) <still waiting for thread %u> ...\n", t->num, n->num, owner->num);
                timeout.tv_sec += 5;
        }
    }
    int ret = real_mutex_lock(mutex);

    n->owner = t;
    t->owns = n;

    real_mutex_unlock(&n->lock);

    if(owner)
        fprintf(stderr, "[%u] ... mutex_lock(%u)\n", t->num, n->num);
    else
        fprintf(stderr, "[%u] mutex_lock(%u)\n", t->num, n->num);

    return ret;
}
```
从上述源码我们可以看出来：
1. mutextrace程序启动时设置LD_PRELOAD=mutextrace.so
2. 初始化时设置设置real_mutex_lock为libpthread.so中的pthread_mutex_lock
3. 由于第1步设置了LD_PRELOAD=mutextrace.so，在应用程序中调用pthread_mutex_lock时，会调用pthread_mutex_lock@mutextrace.so，进而调用real_mutex_lock(pthread_mutex_lock@libpthread.so)，并做死锁判断。

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>

static void* (*real_malloc)(size_t)         = NULL;

static void init()
{
    real_malloc  = dlsym(RTLD_NEXT, "malloc");
    if (!real_malloc) {
        fprintf(stderr, "unable to get malloc symbol!\n");
        exit(1);
    }
    fprintf(stderr, "mwrap.so: successfully wrapped!\n");
}

void *malloc(size_t size)
{
    if (!real_malloc) {
        init();
    }
    void *ret = real_malloc(size);
    fprintf(stderr, "using wrapped malloc: size = %ld, pointer = %p\n", size, ret); // 如果打印，一定要用fprintf(stderr)，否则会产生无限循环，因为fprintf(stdout)也会使用malloc！

    return ret;
}
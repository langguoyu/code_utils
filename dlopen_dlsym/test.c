#include <dlfcn.h>
#include <stdio.h>

int main()
{
    char *lib_so = "./libhello.so";
    char *sym_so = "hello";

    void *lib_handle = NULL;
    void (*hello_func)(const char *s);

    lib_handle = dlopen(lib_so, RTLD_LAZY);
    if (lib_handle == NULL)
    {
        printf("open failed: %s\n", lib_so);
        return -1;
    }

    hello_func = (void (*)(const char *s))dlsym(lib_handle, sym_so);
    if (hello_func == NULL)
    {
        printf("dlsym failed: %s\n", sym_so);
        return -2;
    }

    (*hello_func)("World !");

    return 0;
}
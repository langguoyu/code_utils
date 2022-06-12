#include <dlfcn.h>
#include <stdio.h>

int main()
{
    const char* lib_so = "./libhello.so";
   const char* sym_so = "hello";

    void *lib_handle = NULL;
    void (*hello_func)(const char *s);

    lib_handle = dlopen(lib_so, RTLD_LAZY);
    if (lib_handle == NULL)
    {
        printf("%s open failed\n",lib_so);
        return -1;
    }

    hello_func = (void (*)(const char*))dlsym(lib_handle, sym_so);
    if (hello_func == NULL)
    {
        printf("%s dlsym failed\n", sym_so);
        return -2;
    }

    (*hello_func)("World !");

    return 0;
}